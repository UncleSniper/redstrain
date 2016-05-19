#ifndef REDSTRAIN_MOD_UTIL_UNARYORDERPREDICATE_HPP
#define REDSTRAIN_MOD_UTIL_UNARYORDERPREDICATE_HPP

#include "BareType.hpp"
#include "Function.hpp"
#include "OrderConstraint.hpp"

namespace redengine {
namespace util {

	template<
		typename BoundT,
		typename OperandT = typename BareType<BoundT>::ConstRef,
		typename ConstructionT = typename BareType<BoundT>::ConstRef
	>
	class UnaryOrderPredicate : public Function<bool, OperandT> {

	  public:
		typedef BoundT Bound;
		typedef OperandT Operand;
		typedef ConstructionT Construction;

	  private:
		BoundT bound;
		OrderConstraint constraint;

	  public:
		UnaryOrderPredicate(ConstructionT bound, OrderConstraint constraint)
				: bound(bound), constraint(constraint) {}

		UnaryOrderPredicate(const UnaryOrderPredicate& predicate)
				: Function<bool, OperandT>(predicate), bound(predicate.bound), constraint(predicate.constraint) {}

		virtual ~UnaryOrderPredicate() {}

		inline ConstructionT getBoundOperand() {
			return bound;
		}

		inline OrderConstraint getConstraint() const {
			return constraint;
		}

		inline void setConstraint(OrderConstraint constraint) {
			this->constraint = constraint;
		}

		virtual bool call(OperandT operand) {
			switch(constraint) {
				case OC_LESS:
					return operand < bound;
				case OC_LESS_EQUAL:
					return operand <= bound;
				case OC_GREATER:
					return operand > bound;
				case OC_GREATER_EQUAL:
					return operand >= bound;
				case OC_EQUAL:
					return operand == bound;
				case OC_UNEQUAL:
					return operand != bound;
				default:
					return false;
			}
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_UNARYORDERPREDICATE_HPP */
