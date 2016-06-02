#ifndef REDSTRAIN_MOD_LOG_ANYLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_ANYLOGFILTER_HPP

#include "JunctorLogFilter.hpp"

namespace redengine {
namespace log {

	template<
		typename TimestampT,
		typename SeverityT,
		typename ComponentT,
		typename UnitT,
		typename ConcernT,
		typename LockingPolicyT
	>
	class AnyLogFilter : public JunctorLogFilter<
		TimestampT,
		SeverityT,
		ComponentT,
		UnitT,
		ConcernT,
		LockingPolicyT
	> {

	  private:
		typedef JunctorLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT> Base;
		typedef typename Base::FilterLocker FilterLocker;
		typedef typename Base::OperandIterator OperandIterator;

	  public:
		typedef typename Base::Message Message;

	  public:
		AnyLogFilter(bool manageOperands)
				: JunctorLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT>(manageOperands)
				{}

		AnyLogFilter(bool manageOperands, typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: JunctorLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT>(manageOperands,
				lockInitializer) {}

		AnyLogFilter(const AnyLogFilter& filter)
				: JunctorLogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT, LockingPolicyT>(filter) {}

		virtual bool shouldLogMessage(const Message& message) {
			FilterLocker lock(this);
			OperandIterator begin(this->operands.begin()), end(this->operands.end());
			for(; begin != end; ++begin) {
				if((*begin)->shouldLogMessage(message)) {
					lock.release();
					return true;
				}
			}
			lock.release();
			return false;
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_ANYLOGFILTER_HPP */
