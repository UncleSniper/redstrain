#ifndef REDSTRAIN_MOD_LOCALE_LOCALIZINGOUTPUTSTREAMERRORHANDLER_HPP
#define REDSTRAIN_MOD_LOCALE_LOCALIZINGOUTPUTSTREAMERRORHANDLER_HPP

#include <redstrain/text/OutputStreamErrorHandler16.hpp>

#include "LocalizingErrorHandler.hpp"

namespace redengine {
namespace locale {

	class LocalizingOutputStreamStackTraceSink;

	class REDSTRAIN_LOCALE_API LocalizingOutputStreamErrorHandler
			: public text::OutputStreamErrorHandler16, public LocalizingErrorHandler {

	  protected:
		virtual void writeStackTrace(const error::StackTrace*);
		virtual void configureLocalizingStackTraceSink(LocalizingOutputStreamStackTraceSink&,
				text::OutputStreamStackTraceIndenter16&);

	  public:
		LocalizingOutputStreamErrorHandler(io::OutputStream<text::Char16>&, Indenter&);
		LocalizingOutputStreamErrorHandler(io::OutputStream<text::Char16>&, Indenter&, const Locale&);
		LocalizingOutputStreamErrorHandler(io::OutputStream<text::Char16>&, Indenter&,
				StackTraceMessageSource<text::Char16>&);
		LocalizingOutputStreamErrorHandler(io::OutputStream<text::Char16>&, Indenter&,
				StackTraceMessageSource<text::Char16>&, const Locale&);
		LocalizingOutputStreamErrorHandler(const LocalizingOutputStreamErrorHandler&);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_LOCALIZINGOUTPUTSTREAMERRORHANDLER_HPP */
