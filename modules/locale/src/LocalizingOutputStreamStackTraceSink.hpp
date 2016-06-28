#ifndef REDSTRAIN_MOD_LOCALE_LOCALIZINGOUTPUTSTREAMSTACKTRACESINK_HPP
#define REDSTRAIN_MOD_LOCALE_LOCALIZINGOUTPUTSTREAMSTACKTRACESINK_HPP

#include <redstrain/text/OutputStreamStackTraceSink16.hpp>

#include "LocalizingStackTraceSink.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API LocalizingOutputStreamStackTraceSink
			: public text::OutputStreamStackTraceSink16, public LocalizingStackTraceSink {

	  public:
		LocalizingOutputStreamStackTraceSink(io::OutputStream<text::Char16>&, Indenter&);
		LocalizingOutputStreamStackTraceSink(io::OutputStream<text::Char16>&, Indenter&, const Locale&);
		LocalizingOutputStreamStackTraceSink(io::OutputStream<text::Char16>&, Indenter&,
				StackTraceMessageSource<text::Char16>&);
		LocalizingOutputStreamStackTraceSink(io::OutputStream<text::Char16>&, Indenter&,
				StackTraceMessageSource<text::Char16>&, const Locale&);
		LocalizingOutputStreamStackTraceSink(const LocalizingOutputStreamStackTraceSink&);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_LOCALIZINGOUTPUTSTREAMSTACKTRACESINK_HPP */
