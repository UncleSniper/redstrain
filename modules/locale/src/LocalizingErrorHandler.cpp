#include <redstrain/text/Transcode.hpp>
#include <redstrain/error/NoErrorLocalizerRegisteredError.hpp>
#include <redstrain/text/l10n.hpp>

#include "LocalizingErrorHandler.hpp"
#include "StackTraceMessageCatalog.hpp"

using redengine::text::Char16;
using redengine::error::Error;
using redengine::text::String16;
using redengine::text::Transcode;
using redengine::error::NoErrorLocalizerRegisteredError;
using redengine::text::getErrorMessage;

namespace redengine {
namespace locale {

	LocalizingErrorHandler::LocalizingErrorHandler() : messageSource(getDefaultStackTraceMessageCatalog16()),
			desiredLocale(&Locale::getSystemLocale()) {}

	LocalizingErrorHandler::LocalizingErrorHandler(const Locale& desiredLocale)
			: messageSource(getDefaultStackTraceMessageCatalog16()), desiredLocale(&desiredLocale.intern()) {}

	LocalizingErrorHandler::LocalizingErrorHandler(StackTraceMessageSource<Char16>& messageSource)
			: messageSource(messageSource), desiredLocale(&Locale::getSystemLocale()) {}

	LocalizingErrorHandler::LocalizingErrorHandler(StackTraceMessageSource<Char16>& messageSource,
			const Locale& desiredLocale) : messageSource(messageSource), desiredLocale(&desiredLocale.intern()) {}

	LocalizingErrorHandler::LocalizingErrorHandler(const LocalizingErrorHandler& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase16(handler),
			messageSource(handler.messageSource), desiredLocale(handler.desiredLocale) {}

	void LocalizingErrorHandler::setDesiredLocale(const Locale& desiredLocale) {
		this->desiredLocale = &desiredLocale.intern();
	}

	void LocalizingErrorHandler::writeErrorType(const char* typeName) {
		highlightErrorTypeSection();
		writeString(messageSource.localizeMessage(*desiredLocale, STMK_EH_BEFORE_TYPENAME));
		endHighlighting();
		highlightErrorTypeName();
		writeString(Transcode::utf8ToBMP(typeName));
		endHighlighting();
		highlightErrorTypeSection();
		writeString(messageSource.localizeMessage(*desiredLocale, STMK_EH_AFTER_TYPENAME));
		endHighlighting();
	}

	void LocalizingErrorHandler::writeErrorMessage(const Error& error) {
		String16 message;
		try {
			message = getErrorMessage(error, *desiredLocale);
		}
		catch(const NoErrorLocalizerRegisteredError&) {
			failedToLocalizeErrorMessage(error);
			return;
		}
		highlightErrorMessage();
		writeString(message);
		endHighlighting();
	}

	void LocalizingErrorHandler::failedToLocalizeErrorMessage(const Error& error) {
		ErrorHandlerBase16::writeErrorMessage(error);
	}

}}
