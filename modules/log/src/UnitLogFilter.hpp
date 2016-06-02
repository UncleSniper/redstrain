#ifndef REDSTRAIN_MOD_LOG_UNITLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_UNITLOGFILTER_HPP

#include "MemberPredicateLogFilter.hpp"

namespace redengine {
namespace log {

	template<typename TimestampT, typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class UnitLogFilter
			: public MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, UnitT> {

	  private:
		typedef MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, UnitT> Base;

	  public:
		typedef typename Base::Message Message;
		typedef typename Base::Predicate Predicate;

	  public:
		UnitLogFilter(Predicate& predicate, bool managePredicate)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, UnitT>(predicate,
				managePredicate) {}

		UnitLogFilter(const UnitLogFilter& filter)
				: MemberPredicateLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, UnitT>(filter) {}

		virtual bool shouldLogMessage(const Message& message) {
			return this->predicate.call(message.unit);
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_UNITLOGFILTER_HPP */
