#ifndef REDSTRAIN_MOD_LOG_LOGGER_HPP
#define REDSTRAIN_MOD_LOG_LOGGER_HPP

#include <redstrain/text/types.hpp>

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class Logger {

	  public:
		typedef SeverityT Severity;
		typedef ComponentT Component;
		typedef UnitT Unit;
		typedef ConcernT Concern;

	  public:
		Logger() {}
		Logger(const Logger&) {}
		virtual ~Logger() {}

		virtual void log(const SeverityT&, const ComponentT&, const UnitT&,
				const ConcernT&, const text::String16&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_LOG_LOGGER_HPP */
