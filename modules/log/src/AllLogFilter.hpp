#ifndef REDSTRAIN_MOD_LOG_ALLLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_ALLLOGFILTER_HPP

#include "JunctorLogFilter.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT, typename LockingPolicyT>
	class AllLogFilter : public JunctorLogFilter<SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT> {

	  private:
		typedef JunctorLogFilter<SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT> Base;
		typedef typename Base::FilterLocker FilterLocker;
		typedef typename Base::OperandIterator OperandIterator;

	  public:
		AllLogFilter(bool manageOperands)
				: JunctorLogFilter<SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT>(manageOperands) {}

		AllLogFilter(bool manageOperands, typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: JunctorLogFilter<SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT>(manageOperands,
				lockInitializer) {}

		AllLogFilter(const AllLogFilter& filter)
				: JunctorLogFilter<SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT>(filter) {}

		virtual bool shouldLogMessage(const SeverityT& severity, const ComponentT& component, const UnitT& unit,
				const ConcernT& concern, const text::String16& message) {
			FilterLocker lock(this);
			OperandIterator begin(this->operands.begin()), end(this->operands.end());
			for(; begin != end; ++begin) {
				if(!(*begin)->shouldLogMessage(severity, component, unit, concern, message)) {
					lock.release();
					return false;
				}
			}
			lock.release();
			return true;
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_ALLLOGFILTER_HPP */
