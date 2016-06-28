#include <redstrain/text/DefaultIntegerFormatter.hpp>
#include <redstrain/text/DefaultFormattingRendition.hpp>
#include <redstrain/text/DefaultFormattingOptionStringEmitter.hpp>

#include "LocalizingStackTraceSink.hpp"
#include "StackTraceMessageCatalog.hpp"

using redengine::text::Char16;
using redengine::util::MemorySize;
using redengine::text::FormattingOptions;
using redengine::text::DefaultIntegerFormatter;
using redengine::text::DefaultFormattingRendition;
using redengine::text::DefaultFormattingOptionStringEmitter;

namespace redengine {
namespace locale {

	LocalizingStackTraceSink::LocalizingStackTraceSink() : messageSource(getDefaultStackTraceMessageCatalog16()),
			desiredLocale(&Locale::getSystemLocale()) {}

	LocalizingStackTraceSink::LocalizingStackTraceSink(const Locale& desiredLocale)
			: messageSource(getDefaultStackTraceMessageCatalog16()), desiredLocale(&desiredLocale.intern()) {}

	LocalizingStackTraceSink::LocalizingStackTraceSink(StackTraceMessageSource<Char16>& messageSource)
			: messageSource(messageSource), desiredLocale(&Locale::getSystemLocale()) {}

	LocalizingStackTraceSink::LocalizingStackTraceSink(StackTraceMessageSource<Char16>& messageSource,
			const Locale& desiredLocale) : messageSource(messageSource), desiredLocale(&desiredLocale.intern()) {}

	LocalizingStackTraceSink::LocalizingStackTraceSink(const LocalizingStackTraceSink& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase16(sink), messageSource(sink.messageSource), desiredLocale(sink.desiredLocale) {}

	void LocalizingStackTraceSink::setDesiredLocale(const Locale& desiredLocale) {
		this->desiredLocale = &desiredLocale.intern();
	}

	void LocalizingStackTraceSink::writeNoStackTraceAvailable() {
		writeHeader(messageSource.localizeMessage(*desiredLocale, STMK_ST_NO_TRACE_AVAILABLE));
	}

	void LocalizingStackTraceSink::writeStackTraceHeader() {
		writeHeader(messageSource.localizeMessage(*desiredLocale, STMK_ST_STACK_TRACE_HEADER));
	}

	void LocalizingStackTraceSink::writeNoFramesOnStack() {
		writeFrame(messageSource.localizeMessage(*desiredLocale, STMK_ST_NO_FRAMES_ON_STACK));
	}

	void LocalizingStackTraceSink::writeFrameNotReconstructible() {
		writeFrame(messageSource.localizeMessage(*desiredLocale, STMK_ST_FRAME_NOT_RECONSTRUCTIBLE));
	}

	void LocalizingStackTraceSink::writeBeforeModuleName() {
		writeFrameModule(messageSource.localizeMessage(*desiredLocale, STMK_ST_FRAME_BEFORE_MODULE_NAME));
	}

	void LocalizingStackTraceSink::writeAfterModuleName() {
		writeFrameModule(messageSource.localizeMessage(*desiredLocale, STMK_ST_FRAME_AFTER_MODULE_NAME));
	}

	void LocalizingStackTraceSink::writeUnresolvedSymbol() {
		writeFrame(messageSource.localizeMessage(*desiredLocale, STMK_ST_UNRESOLVED_SYMBOL));
	}

	void LocalizingStackTraceSink::writeBottomOfUnwinding(MemorySize frameCount) {
		writeFrame(messageSource.localizeMessage(*desiredLocale, STMK_ST_START_BOTTOM_OF_UNWINDING));
		writeFrame(DefaultIntegerFormatter<Char16>::formatInteger<
			MemorySize,
			DefaultFormattingRendition<Char16>
		>(frameCount, FormattingOptions<Char16>(DefaultFormattingOptionStringEmitter<Char16>::instance)));
		writeFrame(messageSource.localizeMessage(*desiredLocale, STMK_ST_END_BOTTOM_OF_UNWINDING));
	}

}}
