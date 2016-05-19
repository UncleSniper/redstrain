#ifndef REDSTRAIN_MOD_LOG_NULLLOGGER_HPP
#define REDSTRAIN_MOD_LOG_NULLLOGGER_HPP

#include "Logger.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class NullLogger : public Logger<SeverityT, ComponentT, UnitT, ConcernT> {

	  public:
		typedef LogMessage<SeverityT, ComponentT, UnitT, ConcernT> Message;

	  public:
		NullLogger() {}
		NullLogger(const NullLogger& logger) : Logger<SeverityT, ComponentT, UnitT, ConcernT>(logger) {}

		virtual void log(const Message&) {}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_NULLLOGGER_HPP */
