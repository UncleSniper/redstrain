#ifndef REDSTRAIN_MOD_LOG_LOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_LOGFILTER_HPP

#include <redstrain/text/types.hpp>

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class LogFilter {

	  public:
		LogFilter() {}
		LogFilter(const LogFilter&) {}
		virtual ~LogFilter() {}

		virtual bool shouldLogMessage(const SeverityT&, const ComponentT&, const UnitT&,
				const ConcernT&, const text::String16&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_LOG_LOGFILTER_HPP */
