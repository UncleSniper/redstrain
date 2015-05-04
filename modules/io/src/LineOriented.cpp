#include "LineOriented.hpp"

namespace redengine {
namespace io {

	LineOriented::LineOriented(LinebreakPolicy linebreaks) : linebreaks(linebreaks) {}

	LineOriented::LineOriented(const LineOriented& stream) : linebreaks(stream.linebreaks) {}

}}
