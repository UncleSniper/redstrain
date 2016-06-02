#ifndef REDSTRAIN_MOD_LOG_COMPONENTLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_COMPONENTLOGFILTER_HPP

#include "MemberPredicateLogFilter.hpp"

namespace redengine {
namespace log {

	template<typename TimestampT, typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class ComponentLogFilter : public MemberPredicateLogFilter<
		TimestampT,
		SeverityT,
		ComponentT,
		UnitT,
		ConcernT,
		ComponentT
	> {

	  private:
		typedef MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, ComponentT> Base;

	  public:
		typedef typename Base::Message Message;
		typedef typename Base::Predicate Predicate;

	  public:
		ComponentLogFilter(Predicate& predicate, bool managePredicate)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, ComponentT>(predicate,
				managePredicate) {}

		ComponentLogFilter(const ComponentLogFilter& filter)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, ComponentT>(filter) {}

		virtual bool shouldLogMessage(const Message& message) {
			return this->predicate.call(message.component);
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_COMPONENTLOGFILTER_HPP */
