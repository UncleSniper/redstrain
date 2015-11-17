#include <redstrain/util/CPPUtils.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/MissingInputSeparatorError.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BlobMessageMapping.hpp"
#include "MessageBlobRegistrar.hpp"

using std::string;
using redengine::util::Appender;
using redengine::util::CPPUtils;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::FormattedInputStream;
using redengine::io::MissingInputSeparatorError;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace locale {

	// ======== IncludeResolver ========

	MessageBlobRegistrar::IncludeResolver::IncludeResolver() {}

	MessageBlobRegistrar::IncludeResolver::IncludeResolver(const IncludeResolver&) {}

	MessageBlobRegistrar::IncludeResolver::~IncludeResolver() {}

	// ======== FileIncludeResolver ========

	MessageBlobRegistrar::FileIncludeResolver::FileIncludeResolver(const string& directory)
			: directory(directory) {}

	MessageBlobRegistrar::FileIncludeResolver::FileIncludeResolver(const FileIncludeResolver& resolver)
			: IncludeResolver(resolver), directory(resolver.directory) {}

	MessageBlobRegistrar::FileIncludeResolver::~FileIncludeResolver() {}

	void MessageBlobRegistrar::FileIncludeResolver::includeBlobs(const string& reference, OutputStream<char>& output,
			bool& withInclude, const string& mappingSymbol, const string& blobNSPrefix, unsigned& nextID) {
		string path(Pathname::tidy(Pathname::join(directory, reference)));
		FileInputStream input(path);
		StreamCloser inCloser(input);
		FileIncludeResolver innerIncludeResolver(Pathname::dirname(path, Pathname::LOGICAL));
		MessageBlobRegistrar::generateBlobAliases(input, path, innerIncludeResolver, output, withInclude,
				mappingSymbol, blobNSPrefix, nextID);
		inCloser.close();
	}

	// ======== MessageBlobRegistrar ========

	MessageBlobRegistrar::MessageBlobRegistrar(BlobMessageMapping*& mapping,
			const string& language, const string& country, const char* data, size_t size) {
		if(!mapping)
			mapping = new BlobMessageMapping;
		mapping->addBlob(language, country, data, size);
	}

	struct MessageBlobRegistrarDeclarer : Appender<string> {

		DefaultConfiguredOutputStream<char>::Stream& output;
		string lastSegment;
		unsigned level;

		MessageBlobRegistrarDeclarer(DefaultConfiguredOutputStream<char>::Stream& output)
				: output(output), level(0u) {}

		virtual void append(const string&);
		virtual void doneAppending();

		virtual void emitDeclaration() = 0;

	};

	void MessageBlobRegistrarDeclarer::append(const string& segment) {
		string seg(StringUtils::trim(segment));
		if(seg.empty())
			return;
		if(!lastSegment.empty()) {
			output << indent << "namespace " << lastSegment << " {" << endln;
			++level;
		}
		lastSegment = seg;
	}

	void MessageBlobRegistrarDeclarer::doneAppending() {
		if(lastSegment.empty())
			return;
		if(level)
			output << shift;
		emitDeclaration();
		if(level) {
			output << unshift << indent;
			for(; level; --level)
				output << '}';
			output << endln;
		}
		output << endln;
	}

	struct MessageBlobRegistrarMappingDeclarer : MessageBlobRegistrarDeclarer {

		MessageBlobRegistrarMappingDeclarer(DefaultConfiguredOutputStream<char>::Stream& output)
				: MessageBlobRegistrarDeclarer(output) {}

		virtual void emitDeclaration();

	};

	void MessageBlobRegistrarMappingDeclarer::emitDeclaration() {
		output << indent << "extern ::redengine::locale::BlobMessageMapping* " << lastSegment << ';' << endln;
	}

	struct MessageBlobRegistrarBlobDeclarer : MessageBlobRegistrarDeclarer {

		MessageBlobRegistrarBlobDeclarer(DefaultConfiguredOutputStream<char>::Stream& output)
				: MessageBlobRegistrarDeclarer(output) {}

		virtual void emitDeclaration();

	};

	void MessageBlobRegistrarBlobDeclarer::emitDeclaration() {
		output << indent << "extern const char " << lastSegment << "[];" << endln;
		output << indent << "extern const size_t " << lastSegment << "_size;" << endln;
	}

	void MessageBlobRegistrar::generateBlobRegistrar(OutputStream<char>& stream, const string& mappingSymbol,
			const string& blobSymbol, const string& language, const string& country, bool withInclude,
			unsigned objectID) {
		DefaultConfiguredOutputStream<char>::Stream out(stream);
		if(withInclude)
			out << "#include <redstrain/locale/MessageBlobRegistrar.hpp>" << endln;
		out << endln;
		MessageBlobRegistrarMappingDeclarer mappingDeclarer(out);
		StringUtils::split(mappingSymbol, "::", mappingDeclarer);
		MessageBlobRegistrarBlobDeclarer blobDeclarer(out);
		StringUtils::split(blobSymbol, "::", blobDeclarer);
		out << "static ::redengine::locale::MessageBlobRegistrar registerMessages";
		out << objectID << '(';
		out << mappingSymbol << ", " << CPPUtils::escapeString(language, true);
		out << ", " << CPPUtils::escapeString(country, true) << ", ";
		out << blobSymbol << ", " << blobSymbol << "_size);" << endln;
	}

	void MessageBlobRegistrar::generateBlobAliases(InputStream<char>& input, const string& inputStreamName,
			IncludeResolver& includeResolver, OutputStream<char>& output, bool& withInclude,
			const string& mappingSymbol, const string& blobNSPrefix, unsigned& nextID) {
		FormattedInputStream<char> formattedInput(input);
		string line, language, country;
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
								includeResolver.includeBlobs(reference, output, withInclude,
										mappingSymbol, blobNSPrefix, nextID);
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
			string blob(line.substr(pos + static_cast<string::size_type>(1u)));
			string name(line.substr(static_cast<string::size_type>(0u), pos));
			if(name == "fallback")
				name.clear();
			pos = name.find('_');
			if(pos != string::npos) {
				language = name.substr(static_cast<string::size_type>(0u), pos);
				country = name.substr(pos + static_cast<string::size_type>(1u));
			}
			else {
				language = name;
				country.clear();
			}
			MessageBlobRegistrar::generateBlobRegistrar(output, mappingSymbol, blobNSPrefix + blob,
					language, country, withInclude, nextID);
			withInclude = false;
			++nextID;
			line.clear();
		}
	}

}}
