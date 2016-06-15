#include "OperatorName.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	OperatorName::OperatorName(Operator oper) : oper(oper) {}

	OperatorName::OperatorName(const OperatorName& name) : UnqualifiedName(name), oper(name.oper) {}

	Name::NameType OperatorName::getNameType() const {
		return NT_OPERATOR;
	}

	void OperatorName::print(ostream& out, bool& lastWasGreater, const string*) const {
		out << "operator";
		lastWasGreater = false;
		switch(oper) {
			case OP_NEW:
				out << " new";
				break;
			case OP_NEW_ARRAY:
				out << " new[]";
				break;
			case OP_DELETE:
				out << " delete";
				break;
			case OP_DELETE_ARRAY:
				out << " delete[]";
				break;
			case OP_POSITIVE:
				out << '+';
				break;
			case OP_NEGATIVE:
				out << '-';
				break;
			case OP_ADDRESS_OF:
				out << '&';
				break;
			case OP_DEREFERENCE:
				out << '*';
				break;
			case OP_COMPLEMENT:
				out << '~';
				break;
			case OP_PLUS:
				out << '+';
				break;
			case OP_MINUS:
				out << '-';
				break;
			case OP_MULTIPLY:
				out << '*';
				break;
			case OP_DIVIDE:
				out << '/';
				break;
			case OP_REMAINDER:
				out << '%';
				break;
			case OP_AND:
				out << '&';
				break;
			case OP_OR:
				out << '|';
				break;
			case OP_XOR:
				out << '^';
				break;
			case OP_ASSIGN:
				out << '=';
				break;
			case OP_PLUS_ASSIGN:
				out << "+=";
				break;
			case OP_MINUS_ASSIGN:
				out << "-=";
				break;
			case OP_MULTIPLY_ASSIGN:
				out << "*=";
				break;
			case OP_DIVIDE_ASSIGN:
				out << "/=";
				break;
			case OP_REMAINDER_ASSIGN:
				out << "%=";
				break;
			case OP_AND_ASSIGN:
				out << "&=";
				break;
			case OP_OR_ASSIGN:
				out << "|=";
				break;
			case OP_XOR_ASSIGN:
				out << "^=";
				break;
			case OP_LEFT_SHIFT:
				out << "<<";
				break;
			case OP_RIGHT_SHIFT:
				out << ">>";
				lastWasGreater = true;
				break;
			case OP_LEFT_SHIFT_ASSIGN:
				out << "<<=";
				break;
			case OP_RIGHT_SHIFT_ASSIGN:
				out << ">>=";
				break;
			case OP_EQUAL:
				out << "==";
				break;
			case OP_UNEQUAL:
				out << "!=";
				break;
			case OP_LESS:
				out << '<';
				break;
			case OP_GREATER:
				out << '>';
				lastWasGreater = true;
				break;
			case OP_LESS_EQUAL:
				out << "<=";
				break;
			case OP_GREATER_EQUAL:
				out << ">=";
				break;
			case OP_NOT:
				out << '!';
				break;
			case OP_LOGICAL_AND:
				out << "&&";
				break;
			case OP_LOGICAL_OR:
				out << "||";
				break;
			case OP_INCREMENT:
				out << "++";
				break;
			case OP_DECREMENT:
				out << "--";
				break;
			case OP_COMMA:
				out << ',';
				break;
			case OP_POINTER_TO_MEMBER:
				out << "->*";
				break;
			case OP_POINTER:
				out << "->";
				break;
			case OP_CALL:
				out << "()";
				break;
			case OP_INDEX:
				out << "[]";
				break;
			default:
				out << "<unknown operator>";
				lastWasGreater = true;
				break;
		}
	}

	Name* OperatorName::cloneName() const {
		return new OperatorName(*this);
	}

	bool OperatorName::namesTemplate() const {
		return false;
	}

	bool OperatorName::namesReturnless() const {
		return false;
	}

}}}
