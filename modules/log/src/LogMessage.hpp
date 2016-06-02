#ifndef REDSTRAIN_MOD_LOG_LOGMESSAGE_HPP
#define REDSTRAIN_MOD_LOG_LOGMESSAGE_HPP

#include <redstrain/text/types.hpp>

namespace redengine {
namespace log {

	template<typename TimestampT, typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class LogMessage {

	  public:
		typedef TimestampT Timestamp;
		typedef SeverityT Severity;
		typedef ComponentT Component;
		typedef UnitT Unit;
		typedef ConcernT Concern;

	  public:
		const TimestampT& timestamp;
		const SeverityT& severity;
		const ComponentT& component;
		const UnitT& unit;
		const ConcernT& concern;
		const text::String16& message;

	  public:
		LogMessage(const TimestampT& timestamp, const SeverityT& severity, const ComponentT& component,
				const UnitT& unit, const ConcernT& concern, const text::String16& message)
				: timestamp(timestamp), severity(severity), component(component), unit(unit), concern(concern),
				message(message) {}

		LogMessage(const LogMessage& message) : timestamp(message.timestamp), severity(message.severity),
				component(message.component), unit(message.unit), concern(message.concern),
				message(message.message) {}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_LOGMESSAGE_HPP */
