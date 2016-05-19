#ifndef REDSTRAIN_MOD_UTIL_BINARYORDERPREDICATE_HPP
#define REDSTRAIN_MOD_UTIL_BINARYORDERPREDICATE_HPP

#include "Function.hpp"
#include "OrderConstraint.hpp"

namespace redengine {
namespace util {

	template<typename LeftOperandT, typename RightOperandT = LeftOperandT>
	class BinaryOrderPredicate : public Function<bool, LeftOperandT, RightOperandT> {

	  public:
		typedef LeftOperandT LeftOperand;
		typedef RightOperandT RightOperand;

	  private:
		OrderConstraint constraint;

	  public:
		BinaryOrderPredicate(OrderConstraint constraint) : constraint(constraint) {}

		BinaryOrderPredicate(const BinaryOrderPredicate& predicate)
				: Function<bool, LeftOperandT, RightOperandT>(predicate), constraint(predicate.constraint) {}

		inline OrderConstraint getConstraint() const {
			return constraint;
		}

		inline void setConstraint(OrderConstraint constraint) {
			this->constraint = constraint;
		}

		virtual bool call(LeftOperandT left, RightOperandT right) {
			switch(constraint) {
				case OC_LESS:
					return left < right;
				case OC_LESS_EQUAL:
					return left <= right;
				case OC_GREATER:
					return left > right;
				case OC_GREATER_EQUAL:
					return left >= right;
				case OC_EQUAL:
					return left == right;
				case OC_UNEQUAL:
					return left != right;
				default:
					return false;
			}
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_BINARYORDERPREDICATE_HPP */
