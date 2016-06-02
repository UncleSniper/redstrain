#ifndef REDSTRAIN_MOD_LOG_SEVERITYLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_SEVERITYLOGFILTER_HPP

#include "MemberPredicateLogFilter.hpp"

namespace redengine {
namespace log {

	template<typename TimestampT, typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class SeverityLogFilter
			: public MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, SeverityT> {

	  private:
		typedef MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, SeverityT> Base;

	  public:
		typedef typename Base::Message Message;
		typedef typename Base::Predicate Predicate;

	  public:
		SeverityLogFilter(Predicate& predicate, bool managePredicate)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, SeverityT>(predicate,
				managePredicate) {}

		SeverityLogFilter(const SeverityLogFilter& filter)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, SeverityT>(filter) {}

		virtual bool shouldLogMessage(const Message& message) {
			return this->predicate.call(message.severity);
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_SEVERITYLOGFILTER_HPP */
