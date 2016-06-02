#ifndef REDSTRAIN_MOD_LOG_LOGFILTERLOGGER_HPP
#define REDSTRAIN_MOD_LOG_LOGFILTERLOGGER_HPP

#include "LogFilter.hpp"
#include "FilterLogger.hpp"

namespace redengine {
namespace log {

	template<typename TimestampT, typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class LogFilterLogger : public FilterLogger<TimestampT, SeverityT, ComponentT, UnitT, ConcernT> {

	  public:
		typedef typename FilterLogger<TimestampT, SeverityT, ComponentT, UnitT, ConcernT>::Child Child;
		typedef LogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT> Filter;
		typedef typename Child::Message Message;

	  private:
		Filter& filter;
		bool manageFilter;

	  protected:
		virtual bool shouldPropagateLogMessage(const Message& message) {
			return filter.shouldLogMessage(message);
		}

	  public:
		LogFilterLogger(Child& child, bool manageChild, Filter& filter, bool manageFilter)
				: FilterLogger<TimestampT, SeverityT, ComponentT, UnitT, ConcernT>(child, manageChild),
				filter(filter), manageFilter(manageFilter) {}

		LogFilterLogger(const LogFilterLogger& logger)
				: FilterLogger<TimestampT, SeverityT, ComponentT, UnitT, ConcernT>(logger),
				filter(logger.filter), manageFilter(false) {}

		virtual ~LogFilterLogger() {
			if(manageFilter)
				delete &filter;
		}

		inline Filter& getFilter() {
			return filter;
		}

		inline const Filter& getFilter() const {
			return filter;
		}

		inline bool isManageFilter() const {
			return manageFilter;
		}

		inline void setManageFilter(bool manageFilter) {
			this->manageFilter = manageFilter;
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_LOGFILTERLOGGER_HPP */
