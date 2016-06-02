#ifndef REDSTRAIN_MOD_LOG_TIMESTAMPLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_TIMESTAMPLOGFILTER_HPP

#include "MemberPredicateLogFilter.hpp"

namespace redengine {
namespace log {

	template<typename TimestampT, typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class TimestampLogFilter
			: public MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, TimestampT> {

	  private:
		typedef MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, TimestampT> Base;

	  public:
		typedef typename Base::Message Message;
		typedef typename Base::Predicate Predicate;

	  public:
		TimestampLogFilter(Predicate& predicate, bool managePredicate)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, TimestampT>(predicate,
				managePredicate) {}

		TimestampLogFilter(const TimestampLogFilter& filter)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, TimestampT>(filter) {}

		virtual bool shouldLogMessage(const Message& message) {
			return this->predicate.call(message.timestamp);
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_TIMESTAMPLOGFILTER_HPP */
