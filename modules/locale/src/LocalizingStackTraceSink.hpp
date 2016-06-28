#ifndef REDSTRAIN_MOD_LOCALE_LOCALIZINGSTACKTRACESINK_HPP
#define REDSTRAIN_MOD_LOCALE_LOCALIZINGSTACKTRACESINK_HPP

#include <redstrain/text/StackTraceSinkBase16.hpp>

#include "StackTraceMessageSource.hpp"
#include "api.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API LocalizingStackTraceSink : public virtual text::StackTraceSinkBase16 {

	  private:
		StackTraceMessageSource<text::Char16>& messageSource;
		const Locale* desiredLocale;

	  protected:
		virtual void writeNoStackTraceAvailable();
		virtual void writeStackTraceHeader();
		virtual void writeNoFramesOnStack();
		virtual void writeFrameNotReconstructible();
		virtual void writeBeforeModuleName();
		virtual void writeAfterModuleName();
		virtual void writeUnresolvedSymbol();
		virtual void writeBottomOfUnwinding(util::MemorySize);

	  public:
		LocalizingStackTraceSink();
		LocalizingStackTraceSink(const Locale&);
		LocalizingStackTraceSink(StackTraceMessageSource<text::Char16>&);
		LocalizingStackTraceSink(StackTraceMessageSource<text::Char16>&, const Locale&);
		LocalizingStackTraceSink(const LocalizingStackTraceSink&);

		inline StackTraceMessageSource<text::Char16>& getMessageSource() {
			return messageSource;
		}

		inline const StackTraceMessageSource<text::Char16>& getMessageSource() const {
			return messageSource;
		}

		inline const Locale& getDesiredLocale() const {
			return *desiredLocale;
		}

		void setDesiredLocale(const Locale&);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_LOCALIZINGSTACKTRACESINK_HPP */
