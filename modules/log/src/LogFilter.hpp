#ifndef REDSTRAIN_MOD_LOG_LOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_LOGFILTER_HPP

#include "LogMessage.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class LogFilter {

	  public:
		typedef LogMessage<SeverityT, ComponentT, UnitT, ConcernT> Message;

	  public:
		LogFilter() {}
		LogFilter(const LogFilter&) {}
		virtual ~LogFilter() {}

		virtual bool shouldLogMessage(const Message&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_LOG_LOGFILTER_HPP */
