#ifndef REDSTRAIN_MOD_LOG_LOGGER_HPP
#define REDSTRAIN_MOD_LOG_LOGGER_HPP

#include "LogMessage.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class Logger {

	  public:
		typedef SeverityT Severity;
		typedef ComponentT Component;
		typedef UnitT Unit;
		typedef ConcernT Concern;
		typedef LogMessage<SeverityT, ComponentT, UnitT, ConcernT> Message;

	  public:
		Logger() {}
		Logger(const Logger&) {}
		virtual ~Logger() {}

		void log(const SeverityT& severity, const ComponentT& component, const UnitT& unit,
				const ConcernT& concern, const text::String16& message) {
			log(Message(severity, component, unit, concern, message));
		}

		virtual void log(const Message&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_LOG_LOGGER_HPP */
