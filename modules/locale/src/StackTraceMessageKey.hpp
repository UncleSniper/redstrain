#ifndef REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGEKEY_HPP
#define REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGEKEY_HPP

namespace redengine {
namespace locale {

	enum StackTraceMessageKey {
		STMK_ST_NO_TRACE_AVAILABLE,
		STMK_ST_STACK_TRACE_HEADER,
		STMK_ST_NO_FRAMES_ON_STACK,
		STMK_ST_FRAME_NOT_RECONSTRUCTIBLE,
		STMK_ST_FRAME_BEFORE_MODULE_NAME,
		STMK_ST_FRAME_AFTER_MODULE_NAME,
		STMK_ST_UNRESOLVED_SYMBOL,
		STMK_ST_START_BOTTOM_OF_UNWINDING,
		STMK_ST_END_BOTTOM_OF_UNWINDING,
		STMK__LAST
	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGEKEY_HPP */
