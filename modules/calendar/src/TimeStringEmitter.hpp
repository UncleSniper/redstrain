#ifndef REDSTRAIN_MOD_CALENDAR_TIMESTRINGEMITTER_HPP
#define REDSTRAIN_MOD_CALENDAR_TIMESTRINGEMITTER_HPP

#include <string>

#include "TimeStringOption.hpp"

namespace redengine {
namespace calendar {

	template<typename CharT>
	class TimeStringEmitter {

	  public:
		typedef CharT Char;
		typedef std::basic_string<CharT> String;

	  public:
		TimeStringEmitter() {}
		TimeStringEmitter(const TimeStringEmitter&) {}
		virtual ~TimeStringEmitter() {}

		virtual String emitString(TimeStringOption) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_TIMESTRINGEMITTER_HPP */
