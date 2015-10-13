#include <string>
#include <redstrain/util/Unref.hpp>
#include <redstrain/util/CPPUtils.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/MissingInputSeparatorError.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "TableCodec16.hpp"
#include "CodecManager.hpp"
#include "BlobTableCodecFactory.hpp"
#include "BlobCodeTable16Registrar.hpp"

using std::map;
using std::list;
using std::string;
using redengine::util::Unref;
using redengine::util::Appender;
using redengine::util::CPPUtils;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::MissingInputSeparatorError;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::operator<<;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;

namespace redengine {
namespace text {

	// ======== NameCache ========

	BlobCodeTable16Registrar::NameCache::NameCache() {}

	BlobCodeTable16Registrar::NameCache::NameCache(const NameCache& cache) : nameMap(cache.nameMap) {}

	void BlobCodeTable16Registrar::NameCache::getBlobs(BlobIterator& begin, BlobIterator& end) const {
		begin = BlobIterator(nameMap.begin());
		end = BlobIterator(nameMap.end());
	}

	static list<string> emptyNameList;

	void BlobCodeTable16Registrar::NameCache::getNames(const string& blob,
			NameIterator& begin, NameIterator& end) const {
		NameMapConstIterator it = nameMap.find(blob);
		if(it == nameMap.end()) {
			begin = emptyNameList.begin();
			end = emptyNameList.end();
		}
		else {
			begin = it->second.begin();
			end = it->second.end();
		}
	}

	void BlobCodeTable16Registrar::NameCache::addBlobName(const string& blob, const string& name) {
		NameMapIterator it = nameMap.find(blob);
		Names* names;
		if(it == nameMap.end())
			names = &(nameMap[blob] = Names());
		else
			names = &it->second;
		names->push_back(name);
	}

	void BlobCodeTable16Registrar::NameCache::clearNames(const string& blob) {
		NameMapIterator it = nameMap.find(blob);
		if(it != nameMap.end())
			it->second.clear();
	}

	void BlobCodeTable16Registrar::NameCache::clearNames() {
		nameMap.clear();
	}

	void BlobCodeTable16Registrar::NameCache::generateBlobRegistrars(GeneratorWriter& writer) const {
		NameMapConstIterator begin(nameMap.begin()), end(nameMap.end());
		for(; begin != end; ++begin)
			writer.generateBlobRegistrar(begin->first, begin->second.begin(), begin->second.end());
	}

	// ======== IncludeResolver ========

	BlobCodeTable16Registrar::GeneratorReader::IncludeResolver::IncludeResolver() {}

	BlobCodeTable16Registrar::GeneratorReader::IncludeResolver::IncludeResolver(const IncludeResolver&) {}

	BlobCodeTable16Registrar::GeneratorReader::IncludeResolver::~IncludeResolver() {}

	void BlobCodeTable16Registrar::GeneratorReader::IncludeResolver::includeBlobsFrom(InputStream<char>& input,
			const string& inputStreamName, const GeneratorReader& outerReader,
			IncludeResolver& innerIncludeResolver) {
		GeneratorReader innerReader(input, inputStreamName, outerReader, innerIncludeResolver);
		innerReader.readBlobNames();
	}

	// ======== FileIncludeResolver ========

	BlobCodeTable16Registrar::GeneratorReader::FileIncludeResolver::FileIncludeResolver(const string& directory)
			: directory(directory) {}

	BlobCodeTable16Registrar::GeneratorReader::FileIncludeResolver::FileIncludeResolver(const
			FileIncludeResolver& resolver) : IncludeResolver(resolver), directory(resolver.directory) {}

	BlobCodeTable16Registrar::GeneratorReader::FileIncludeResolver::~FileIncludeResolver() {}

	void BlobCodeTable16Registrar::GeneratorReader::FileIncludeResolver::includeBlobs(const string& reference,
			const GeneratorReader& outerReader) {
		string path(Pathname::tidy(Pathname::join(directory, reference)));
		FileInputStream input(path);
		StreamCloser inCloser(input);
		FileIncludeResolver innerIncludeResolver(Pathname::dirname(path, Pathname::LOGICAL));
		IncludeResolver::includeBlobsFrom(input, path, outerReader, innerIncludeResolver);
		inCloser.close();
	}

	// ======== GeneratorReader ========

	BlobCodeTable16Registrar::GeneratorReader::GeneratorReader(InputStream<char>& input,
			const string& inputStreamName, NameCache& nameCache, IncludeResolver& includeResolver)
			: inputStreamName(inputStreamName), input(input), formattedInput(this->input),
			nameCache(nameCache), includeResolver(includeResolver) {}

	BlobCodeTable16Registrar::GeneratorReader::GeneratorReader(InputStream<char>& input,
			const string& inputStreamName, const GeneratorReader& reader, IncludeResolver& includeResolver)
			: inputStreamName(inputStreamName), input(input), formattedInput(this->input),
			nameCache(reader.nameCache), includeResolver(includeResolver) {}

	BlobCodeTable16Registrar::GeneratorReader::GeneratorReader(const GeneratorReader& reader)
			: inputStreamName(reader.inputStreamName), input(reader.input), formattedInput(this->input),
			nameCache(reader.nameCache), includeResolver(reader.includeResolver) {}

	void BlobCodeTable16Registrar::GeneratorReader::readBlobNames() {
		string line;
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
								includeResolver.includeBlobs(reference, *this);
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
			nameCache.addBlobName(blob, name);
			line.clear();
		}
	}

	// ======== BlobSymbolMapper ========

	BlobCodeTable16Registrar::GeneratorWriter::BlobSymbolMapper::BlobSymbolMapper() {}

	BlobCodeTable16Registrar::GeneratorWriter::BlobSymbolMapper::BlobSymbolMapper(const BlobSymbolMapper&) {}

	BlobCodeTable16Registrar::GeneratorWriter::BlobSymbolMapper::~BlobSymbolMapper() {}

	// ======== GeneratorWriter ========

	BlobCodeTable16Registrar::GeneratorWriter::GeneratorWriter(OutputStream<char>& output,
			BlobSymbolMapper* blobSymbolMapper)
			: output(output), formattedOutput(this->output), blobSymbolMapper(blobSymbolMapper),
			needsHeader(true), nextID(0u) {}

	BlobCodeTable16Registrar::GeneratorWriter::GeneratorWriter(const GeneratorWriter& writer)
			: output(writer.output), formattedOutput(this->output), blobSymbolMapper(writer.blobSymbolMapper),
			needsHeader(writer.needsHeader), nextID(writer.nextID) {}

	struct BlobCodeTable16RegistrarBlobDeclarer : Appender<string> {

		DefaultConfiguredOutputStream<char>::Stream& output;
		string lastSegment;
		unsigned level;

		BlobCodeTable16RegistrarBlobDeclarer(DefaultConfiguredOutputStream<char>::Stream& output)
				: output(output), level(0u) {}

		virtual void append(const string&);
		virtual void doneAppending();

	};

	void BlobCodeTable16RegistrarBlobDeclarer::append(const string& segment) {
		string seg(StringUtils::trim(segment));
		if(seg.empty())
			return;
		if(!lastSegment.empty()) {
			output << indent << "namespace " << lastSegment << " {" << endln;
			++level;
		}
		lastSegment = seg;
	}

	void BlobCodeTable16RegistrarBlobDeclarer::doneAppending() {
		if(lastSegment.empty())
			return;
		output << shift << indent << "extern const char " << lastSegment << "[];" << endln;
		output << indent << "extern const size_t " << lastSegment << "_size;" << endln << unshift;
		if(level) {
			output << indent;
			for(; level; --level)
				output << '}';
			output << endln;
		}
		output << endln;
	}

	void BlobCodeTable16Registrar::GeneratorWriter::generateBlobRegistrar(const string& blob,
			NameCache::NameIterator namesBegin, NameCache::NameIterator namesEnd) {
		if(needsHeader) {
			formattedOutput << "#include <redstrain/text/BlobCodeTable16Registrar.hpp>" << endln << endln;
			needsHeader = false;
		}
		else
			formattedOutput << endln;
		string symbol(blobSymbolMapper ? blobSymbolMapper->mapBlobSymbol(blob) : blob);
		BlobCodeTable16RegistrarBlobDeclarer declarer(formattedOutput);
		StringUtils::split(symbol, "::", declarer);
		formattedOutput << indent << "static const char *const names" << nextID << "[] = {" << endln << shift;
		formattedOutput << indent << CPPUtils::escapeString(blob, true) << ',' << endln;
		for(; namesBegin != namesEnd; ++namesBegin)
			formattedOutput << indent << CPPUtils::escapeString(*namesBegin, true) << ',' << endln;
		formattedOutput << indent << "NULL" << endln;
		formattedOutput << unshift << indent << "};" << endln << endln;
		formattedOutput
				<< indent << "static ::redengine::text::BlobCodeTable16Registrar register" << nextID
				<< "(::" << symbol << ", ::" << symbol << "_size, names" << nextID << ");" << endln;
		++nextID;
	}

	// ======== BlobCodeTable16Registrar ========

	typedef BlobTableCodecFactory<Char16, Encoder16, TableCodec16> EncoderFactory;
	typedef BlobTableCodecFactory<Char16, Decoder16, TableCodec16> DecoderFactory;
	typedef map<string, EncoderFactory*> EncoderFactories;
	typedef map<string, DecoderFactory*> DecoderFactories;

	static EncoderFactories* encoderFactories = NULL;
	static DecoderFactories* decoderFactories = NULL;

	BlobCodeTable16Registrar::BlobCodeTable16Registrar(const char* data, size_t size, const char *const* names) {
		const char *const* name;
		if(!encoderFactories)
			encoderFactories = new EncoderFactories;
		Unref<EncoderFactory> encoder(new EncoderFactory(data, size));
		for(name = names; *name; ++name) {
			EncoderFactories::iterator it = encoderFactories->find(*name);
			if(it == encoderFactories->end())
				(*encoderFactories)[*name] = *encoder;
			else {
				it->second->unref();
				it->second = *encoder;
			}
			encoder->ref();
		}
		if(!decoderFactories)
			decoderFactories = new DecoderFactories;
		Unref<DecoderFactory> decoder(new DecoderFactory(data, size));
		for(name = names; *name; ++name) {
			DecoderFactories::iterator it = decoderFactories->find(*name);
			if(it == decoderFactories->end())
				(*decoderFactories)[*name] = *decoder;
			else {
				it->second->unref();
				it->second = *decoder;
			}
			decoder->ref();
		}
	}

	void BlobCodeTable16Registrar::registerCodecs(CodecManager& manager) {
		if(encoderFactories) {
			EncoderFactories::const_iterator begin(encoderFactories->begin()), end(encoderFactories->end());
			for(; begin != end; ++begin)
				manager.setEncoder16Factory(begin->first, begin->second);
		}
		if(decoderFactories) {
			DecoderFactories::const_iterator begin(decoderFactories->begin()), end(decoderFactories->end());
			for(; begin != end; ++begin)
				manager.setDecoder16Factory(begin->first, begin->second);
		}
	}

}}
