#ifndef REDSTRAIN_MOD_LOG_LOGGER_HPP
#define REDSTRAIN_MOD_LOG_LOGGER_HPP

#include "LogMessage.hpp"

namespace redengine {
namespace log {

	template<typename TimestampT, typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class Logger {

	  public:
		typedef TimestampT Timestamp;
		typedef SeverityT Severity;
		typedef ComponentT Component;
		typedef UnitT Unit;
		typedef ConcernT Concern;
		typedef LogMessage<TimestampT, SeverityT, ComponentT, UnitT, ConcernT> Message;

	  public:
		Logger() {}
		Logger(const Logger&) {}
		virtual ~Logger() {}

		void logln(const SeverityT& severity, const ComponentT& component, const UnitT& unit,
				const ConcernT& concern, const text::String16& message) {
			TimestampT now;
			log(Message(now, severity, component, unit, concern, message));
		}

		void logln(const TimestampT& timestamp, const SeverityT& severity, const ComponentT& component,
				const UnitT& unit, const ConcernT& concern, const text::String16& message) {
			log(Message(timestamp, severity, component, unit, concern, message));
		}

		virtual void log(const Message&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_LOG_LOGGER_HPP */
