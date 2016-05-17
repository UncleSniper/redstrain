#ifndef REDSTRAIN_MOD_LOG_NULLLOGGER_HPP
#define REDSTRAIN_MOD_LOG_NULLLOGGER_HPP

#include "Logger.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class NullLogger : public Logger<SeverityT, ComponentT, UnitT, ConcernT> {

	  public:
		NullLogger() {}
		NullLogger(const NullLogger& logger) : Logger<SeverityT, ComponentT, UnitT, ConcernT>(logger) {}

		virtual void log(const SeverityT&, const ComponentT&, const UnitT&,
				const ConcernT&, const text::String16&) {}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_NULLLOGGER_HPP */
