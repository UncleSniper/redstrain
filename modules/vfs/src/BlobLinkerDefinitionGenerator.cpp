#include <redstrain/util/CPPUtils.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/MissingInputSeparatorError.hpp>

#include "BlobLinkerDefinitionGenerator.hpp"

using std::string;
using redengine::util::CPPUtils;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::MissingInputSeparatorError;

namespace redengine {
namespace vfs {

	// ======== IncludeResolver ========

	BlobLinkerDefinitionGenerator::IncludeResolver::IncludeResolver() {}

	BlobLinkerDefinitionGenerator::IncludeResolver::IncludeResolver(const IncludeResolver&) {}

	BlobLinkerDefinitionGenerator::IncludeResolver::~IncludeResolver() {}

	void BlobLinkerDefinitionGenerator::IncludeResolver::includeAliasesFrom(InputStream<char>& input,
			const string& inputStreamName, BlobLinkerDefinitionGenerator& outerGenerator,
			IncludeResolver& innerIncludeResolver) {
		BlobLinkerDefinitionGenerator innerGenerator(input, inputStreamName, outerGenerator, innerIncludeResolver);
		innerGenerator.defineBlobLinkers();
		outerGenerator.setNextID(innerGenerator.getNextID());
	}

	// ======== FileIncludeResolver ========

	BlobLinkerDefinitionGenerator::FileIncludeResolver::FileIncludeResolver(const string& directory)
			: directory(directory) {}

	BlobLinkerDefinitionGenerator::FileIncludeResolver::FileIncludeResolver(const FileIncludeResolver& resolver)
			: IncludeResolver(resolver), directory(resolver.directory) {}

	BlobLinkerDefinitionGenerator::FileIncludeResolver::~FileIncludeResolver() {}

	void BlobLinkerDefinitionGenerator::FileIncludeResolver::includeAliases(const string& reference,
			BlobLinkerDefinitionGenerator& outerGenerator) {
		string path(Pathname::tidy(Pathname::join(directory, reference)));
		FileInputStream input(path);
		StreamCloser inCloser(input);
		FileIncludeResolver innerIncludeResolver(Pathname::dirname(path, Pathname::LOGICAL));
		IncludeResolver::includeAliasesFrom(input, path, outerGenerator, innerIncludeResolver);
		inCloser.close();
	}

	// ======== BlobLinkerDefinitionGenerator ========

	BlobLinkerDefinitionGenerator::BlobLinkerDefinitionGenerator(InputStream<char>& input,
			const string& inputStreamName, OutputStream<char>& output, IncludeResolver& includeResolver)
			: inputStreamName(inputStreamName), input(input), formattedInput(this->input),
			output(output), formattedOutput(this->output), includeResolver(includeResolver), nextID(0u),
			needsHeader(true) {}

	BlobLinkerDefinitionGenerator::BlobLinkerDefinitionGenerator(InputStream<char>& input,
			const string& inputStreamName, const BlobLinkerDefinitionGenerator& generator,
			IncludeResolver& includeResolver) : inputStreamName(inputStreamName), pathPrefix(generator.pathPrefix),
			fileSuffix(generator.fileSuffix), input(input), formattedInput(this->input), output(generator.output),
			formattedOutput(this->output), includeResolver(includeResolver), nextID(generator.nextID),
			needsHeader(false) {}

	BlobLinkerDefinitionGenerator::BlobLinkerDefinitionGenerator(const BlobLinkerDefinitionGenerator& generator)
			: inputStreamName(generator.inputStreamName), pathPrefix(generator.pathPrefix),
			fileSuffix(generator.fileSuffix), input(generator.input), formattedInput(this->input),
			output(generator.output), formattedOutput(this->output), includeResolver(generator.includeResolver),
			nextID(generator.nextID), needsHeader(generator.needsHeader) {}

	void BlobLinkerDefinitionGenerator::setPathPrefix(const string& prefix) {
		pathPrefix = prefix;
	}

	void BlobLinkerDefinitionGenerator::setFileSuffix(const string& suffix) {
		fileSuffix = suffix;
	}

	void BlobLinkerDefinitionGenerator::defineBlobLinkers() {
		if(needsHeader) {
			formattedOutput.println("#include <redstrain/vfs/BlobVFS.hpp>");
			formattedOutput.endLine();
		}
		string line, prefix(pathPrefix);
		if(!prefix.empty()) {
			if(prefix[prefix.length() - static_cast<string::size_type>(1u)] != '/')
				prefix += '/';
		}
		unsigned lno = 0u;
		while(formattedInput.readLine(line)) {
			++lno;
			string::size_type pos = line.find('#');
			if(pos != string::npos)
				line.erase(pos);
			line = StringUtils::trim(line);
			if(line.empty())
				continue;
			if(line.length() > static_cast<string::size_type>(9u) && StringUtils::startsWith(line, "$include")) {
				switch(line[static_cast<string::size_type>(8u)]) {
					case ' ':
					case '\t':
						{
							string reference(StringUtils::trim(line.substr(static_cast<string::size_type>(9u))));
							if(!reference.empty()) {
								includeResolver.includeAliases(reference, *this);
								line.clear();
								continue;
							}
						}
						break;
					default:
						break;
				}
			}
			pos = line.find('=');
			if(pos == string::npos)
				throw MissingInputSeparatorError("=", inputStreamName, lno);
			formattedOutput.print("static ::redengine::vfs::BlobVFS::BlobLinker alias");
			formattedOutput.print(StringUtils::toString(nextID++));
			formattedOutput.print("(\"");
			formattedOutput.print(CPPUtils::escapeString(line.substr(pos + static_cast<string::size_type>(1u))
					+ fileSuffix, false));
			formattedOutput.print("\", \"");
			formattedOutput.print(CPPUtils::escapeString(prefix
					+ line.substr(static_cast<string::size_type>(0u), pos) + fileSuffix, false));
			formattedOutput.println("\");");
			line.clear();
		}
	}

}}
