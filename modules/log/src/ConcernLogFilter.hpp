#ifndef REDSTRAIN_MOD_LOG_CONCERNLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_CONCERNLOGFILTER_HPP

#include "MemberPredicateLogFilter.hpp"

namespace redengine {
namespace log {

	template<typename TimestampT, typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class ConcernLogFilter : public MemberPredicateLogFilter<
		TimestampT,
		SeverityT,
		ComponentT,
		UnitT,
		ConcernT,
		ConcernT
	> {

	  private:
		typedef MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, ConcernT> Base;

	  public:
		typedef typename Base::Message Message;
		typedef typename Base::Predicate Predicate;

	  public:
		ConcernLogFilter(Predicate& predicate, bool managePredicate)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, ConcernT>(predicate,
				managePredicate) {}

		ConcernLogFilter(const ConcernLogFilter& filter)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, ConcernT>(filter) {}

		virtual bool shouldLogMessage(const Message& message) {
			return this->predicate.call(message.concern);
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_CONCERNLOGFILTER_HPP */
