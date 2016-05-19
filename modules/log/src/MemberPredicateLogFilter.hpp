#ifndef REDSTRAIN_MOD_LOG_MEMBERPREDICATELOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_MEMBERPREDICATELOGFILTER_HPP

#include <redstrain/util/Function.hpp>

#include "LogFilter.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT, typename MemberT>
	class MemberPredicateLogFilter : public LogFilter<SeverityT, ComponentT, UnitT, ConcernT> {

	  private:
		typedef LogFilter<SeverityT, ComponentT, UnitT, ConcernT> Base;

	  public:
		typedef util::Function<bool, const MemberT&> Predicate;

	  protected:
		Predicate& predicate;
		bool managePredicate;

	  public:
		MemberPredicateLogFilter(Predicate& predicate, bool managePredicate)
				: predicate(predicate), managePredicate(managePredicate) {}

		MemberPredicateLogFilter(const MemberPredicateLogFilter& filter)
				: LogFilter<SeverityT, ComponentT, UnitT, ConcernT>(filter),
				predicate(filter.predicate), managePredicate(false) {}

		virtual ~MemberPredicateLogFilter() {
			if(managePredicate)
				delete &predicate;
		}

		inline Predicate& getPredicate() {
			return predicate;
		}

		inline const Predicate& getPredicate() const {
			return predicate;
		}

		inline bool isManagePredicate() const {
			return managePredicate;
		}

		inline void setManagePredicate(bool managePredicate) {
			this->managePredicate = managePredicate;
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_MEMBERPREDICATELOGFILTER_HPP */
