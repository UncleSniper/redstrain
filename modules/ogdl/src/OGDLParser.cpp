#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "OGDLSink.hpp"
#include "OGDLParser.hpp"

using redengine::io::Stream;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::util::MemorySize;
using redengine::io::OutputStream;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;

namespace redengine {
namespace ogdl {

	OGDLParser::OGDLParser(OGDLSink& sink, const String16& file) : sink(sink), file(file), line(1u), column(1u),
			breakChar(static_cast<Char16>('\n')), state(ST_TOP) {}

	OGDLParser::OGDLParser(const OGDLParser& parser)
			: Stream(parser), OutputStream<Char16>(parser), sink(parser.sink), file(parser.file),
			line(parser.line), column(parser.column), breakChar(parser.breakChar), state(parser.state),
			nesting(parser.nesting) {}

	OGDLParser::~OGDLParser() {}

	void OGDLParser::writeBlock(const Char16* data, MemorySize size) {
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < size; ++u) {
			Char16 c = data[u];
			if(!c)//TODO
				break;//TODO
		}
	}

}}
