#ifndef REDSTRAIN_MOD_LOG_JUNCTORLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_JUNCTORLOGFILTER_HPP

#include <set>
#include <redstrain/platform/PolicyLocker.hpp>

#include "LogFilter.hpp"

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
	class JunctorLogFilter : protected LockingPolicyT,
			public LogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT> {

	  public:
		typedef LogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT> Operand;
		typedef std::set<Operand*> Operands;
		typedef typename Operands::const_iterator OperandIterator;

	  protected:
		typedef platform::PolicyLocker<LockingPolicyT> FilterLocker;

	  protected:
		bool manageOperands;
		Operands operands;

	  public:
		JunctorLogFilter(bool manageOperands) : manageOperands(manageOperands) {}

		JunctorLogFilter(bool manageOperands, typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: LockingPolicyT(lockInitializer), manageOperands(manageOperands) {}

		JunctorLogFilter(const JunctorLogFilter& filter) : LockingPolicyT(filter),
				LogFilter<TimestampT, SeverityT, ComponentT, UnitT, ConcernT>(filter), manageOperands(false) {
			FilterLocker lock(filter);
			operands = filter.operands;
			lock.release();
		}

		virtual ~JunctorLogFilter() {
			if(manageOperands) {
				OperandIterator begin(operands.begin()), end(operands.end());
				for(; begin != end; ++begin)
					delete *begin;
			}
		}

		inline bool isManageOperands() const {
			return manageOperands;
		}

		inline void setManageOperands(bool manageOperands) {
			this->manageOperands = manageOperands;
		}

		void getOperands(OperandIterator& begin, OperandIterator& end) const {
			begin = operands.begin();
			end = operands.end();
		}

		void getOperands(Operands& operands) const {
			FilterLocker lock(this);
			operands = this->operands;
			lock.release();
		}

		bool addOperand(Operand& operand) {
			FilterLocker lock(this);
			if(operands.find(&operand) != operands.end()) {
				lock.release();
				return false;
			}
			operands.insert(&operand);
			lock.release();
			return true;
		}

		bool removeOperand(Operand& operand) {
			FilterLocker lock(this);
			typename Operands::iterator it(operands.find(&operand));
			if(it == operands.end()) {
				lock.release();
				return false;
			}
			operands.erase(it);
			lock.release();
			return true;
		}

		void clearOperands() {
			FilterLocker lock(this);
			if(manageOperands) {
				OperandIterator begin(operands.begin()), end(operands.end());
				for(; begin != end; ++begin)
					delete *begin;
			}
			operands.clear();
			lock.release();
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_JUNCTORLOGFILTER_HPP */
