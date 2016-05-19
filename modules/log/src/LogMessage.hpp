#ifndef REDSTRAIN_MOD_LOG_LOGMESSAGE_HPP
#define REDSTRAIN_MOD_LOG_LOGMESSAGE_HPP

#include <redstrain/text/types.hpp>

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class LogMessage {

	  public:
		typedef SeverityT Severity;
		typedef ComponentT Component;
		typedef UnitT Unit;
		typedef ConcernT Concern;

	  public:
		const SeverityT& severity;
		const ComponentT& component;
		const UnitT& unit;
		const ConcernT& concern;
		const text::String16& message;

	  public:
		LogMessage(const SeverityT& severity, const ComponentT& component, const UnitT& unit,
				const ConcernT& concern, const text::String16& message)
				: severity(severity), component(component), unit(unit), concern(concern), message(message) {}

		LogMessage(const LogMessage& message) : severity(message.severity), component(message.component),
				unit(message.unit), concern(message.concern), message(message.message) {}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_LOGMESSAGE_HPP */
