#include <redstrain/util/CPPUtils.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/io/streamtypes.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BlobMessageMapping.hpp"
#include "MessageBlobRegistrar.hpp"

using std::string;
using redengine::util::Appender;
using redengine::util::CPPUtils;
using redengine::io::OutputStream;
using redengine::util::StringUtils;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace locale {

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
			const string& blobSymbol, const string& language, const string& country) {
		DefaultConfiguredOutputStream<char>::Stream out(stream);
		out << "#include <redstrain/locale/MessageBlobRegistrar.hpp>" << endln << endln;
		MessageBlobRegistrarMappingDeclarer mappingDeclarer(out);
		StringUtils::split(mappingSymbol, "::", mappingDeclarer);
		MessageBlobRegistrarBlobDeclarer blobDeclarer(out);
		StringUtils::split(blobSymbol, "::", blobDeclarer);
		out << "static ::redengine::locale::MessageBlobRegistrar registerMessages(";
		out << mappingSymbol << ", " << CPPUtils::escapeString(language, true);
		out << ", " << CPPUtils::escapeString(country, true) << ", ";
		out << blobSymbol << ", " << blobSymbol << "_size);" << endln;
	}

}}
