#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_OPERATORNAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_OPERATORNAME_HPP

#include "UnqualifiedName.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API OperatorName : public UnqualifiedName {

	  public:
		enum Operator {
			OP_NEW,
			OP_NEW_ARRAY,
			OP_DELETE,
			OP_DELETE_ARRAY,
			OP_POSITIVE,
			OP_NEGATIVE,
			OP_ADDRESS_OF,
			OP_DEREFERENCE,
			OP_COMPLEMENT,
			OP_PLUS,
			OP_MINUS,
			OP_MULTIPLY,
			OP_DIVIDE,
			OP_REMAINDER,
			OP_AND,
			OP_OR,
			OP_XOR,
			OP_ASSIGN,
			OP_PLUS_ASSIGN,
			OP_MINUS_ASSIGN,
			OP_MULTIPLY_ASSIGN,
			OP_DIVIDE_ASSIGN,
			OP_REMAINDER_ASSIGN,
			OP_AND_ASSIGN,
			OP_OR_ASSIGN,
			OP_XOR_ASSIGN,
			OP_LEFT_SHIFT,
			OP_RIGHT_SHIFT,
			OP_LEFT_SHIFT_ASSIGN,
			OP_RIGHT_SHIFT_ASSIGN,
			OP_EQUAL,
			OP_UNEQUAL,
			OP_LESS,
			OP_GREATER,
			OP_LESS_EQUAL,
			OP_GREATER_EQUAL,
			OP_NOT,
			OP_LOGICAL_AND,
			OP_LOGICAL_OR,
			OP_INCREMENT,
			OP_DECREMENT,
			OP_COMMA,
			OP_POINTER_TO_MEMBER,
			OP_POINTER,
			OP_CALL,
			OP_INDEX
		};

	  private:
		Operator oper;

	  public:
		OperatorName(Operator);
		OperatorName(const OperatorName&);

		virtual NameType getNameType() const;
		virtual void print(std::ostream&, bool&, const std::string*) const;
		virtual Name* cloneName() const;
		virtual bool namesTemplate() const;
		virtual bool namesReturnless() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_OPERATORNAME_HPP */
