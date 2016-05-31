#include "FormattingOptions.hpp"

namespace redengine {
namespace calendar {

	FormattingOptions::FormattingOptions() : width(0u), padding(PAD_ZERO) {}

	FormattingOptions::FormattingOptions(const FormattingOptions& options)
			: width(options.width), padding(options.padding) {}

}}
