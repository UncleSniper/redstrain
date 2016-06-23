#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINK_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINK_HPP

#include <string>
#include <stdint.h>

#include "types.hpp"
#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Name;
	class Type;
	class CPPSymbol;
	class Expression;
	class TableSymbol;
	class BuiltinType;
	class OperatorName;
	class TemplateArgument;
	class GuardVariableSymbol;
	class OverrideThunkSymbol;
	class UnaryOperationExpression;
	class BinaryOperationExpression;
	class CovariantOverrideThunkSymbol;

	class REDSTRAIN_REDMOND_API SymbolSink {

	  public:
		enum Separator {
			SEP_COMMA,   // ','
			SEP_AFTER_COMMA,   // ' '
			SEP_LEFT_ROUND,
			SEP_RIGHT_ROUND,
			SEP_BEFORE_CV,
			SEP_AFTER_CV,
			SEP_PAAMAYIM_NEKUDOTAYIM,
			SEP_LEFT_ANGLE,
			SEP_RIGHT_ANGLE,
			SEP_DESTRUCTOR_TILDE,
			SEP_LEFT_SQUARE,
			SEP_RIGHT_SQUARE,
			SEP_RETURN_TYPE_FUNCTION,   // ' '
			SEP_TYPE_VARIABLE,   // ' '
			SEP_POINTER,
			SEP_REFERENCE,
			SEP_BEFORE_COMPLEX,   // ' '
			SEP_TERNARY_THEN,   // " ? "
			SEP_TERNARY_ELSE   // " : "
		};

		enum Reserved {
			RSV_RESTRICT,
			RSV_VOLATILE,
			RSV_CONST,
			RSV_CONVERSION_OPERATOR,   // "operator "
			RSV_COMPLEX,   // "_Complex"
			RSV_IMAGINARY,   // "_Imaginary"
			RSV_TRUE,
			RSV_FALSE,
			RSV_SIZEOF
		};

		enum ReplacementString {
			RS_ANONYMOUS_LOCAL_NAME,   // "<anonymous>"
			RS_CONSTRUCTOR,   // "<constructor>"
			RS_DESTRUCTOR,   // "<destructor>"
			RS_UNKNOWN_CONSTRUCTOR_LIKE,   // "<unknown constructor-like>"
			RS_UNKNOWN_RETURN_TYPE,   // "<unknown return type>"
			RS_UNKNOWN_TYPE_MODIFIER,   // " <unknown type modifier>"
			RS_UNKNOWN_BINARY_OPERATOR   // "<unknown binary operator>"
		};

	  public:
		SymbolSink();
		SymbolSink(const SymbolSink&);
		virtual ~SymbolSink();

		virtual unsigned getRemainingColumnCount() = 0;
		virtual unsigned getInlineWidthOf(const Type&, const CurrentTemplateArguments&, const Type*) = 0;
		virtual unsigned getInlineWidthOf(const TemplateArgument&, const CurrentTemplateArguments&) = 0;
		virtual unsigned getInlineWidthOf(Separator) = 0;

		virtual void startNewLine(int) = 0;
		virtual void putSeparator(Separator) = 0;
		virtual void putReserved(Reserved) = 0;
		virtual void putReplacementString(ReplacementString) = 0;
		virtual void putSourceName(const std::string&) = 0;
		virtual void putIntLiteral(int64_t) = 0;
		virtual void putIntLiteral(uint64_t) = 0;
		virtual void putFloatLiteral(float) = 0;
		virtual void putFloatLiteral(double) = 0;

		// "<override thunk for " + targetFunction + ">"
		virtual void putOverrideThunk(const OverrideThunkSymbol&) = 0;
		virtual void putCovariantOverrideThunk(const CovariantOverrideThunkSymbol&) = 0;
		// "<guard variable for " + objectName + ">"
		virtual void putGuardVariable(const GuardVariableSymbol&) = 0;
		// switch(tableType) {
		// 	case TST_VIRTUAL_TABLE:
		// 		out << "<virtual table for ";
		// 		break;
		// 	case TST_VTT_STRUCTURE:
		// 		out << "<VTT structure for ";
		// 		break;
		// 	case TST_TYPEINFO_STRUCTURE:
		// 		out << "<typeinfo structure for ";
		// 		break;
		// 	case TST_TYPEINFO_NAME:
		// 		out << "<typeinfo name for ";
		// 		break;
		// 	default:
		// 		out << "<table object of unknown type, for type ";
		// 		break;
		// }
		// CurrentTemplateArguments targuments;
		// targetType->print(out, lastWasGreater, targuments);
		// out << '>';
		virtual void putTable(const TableSymbol&) = 0;
		// '@' + discriminator
		virtual void putLocalNameDiscriminator(unsigned) = 0;
		// out << "operator";
		// lastWasGreater = false;
		// switch(oper) {
		// 	case OP_NEW:
		// 		out << " new";
		// 		break;
		// 	case OP_NEW_ARRAY:
		// 		out << " new[]";
		// 		break;
		// 	case OP_DELETE:
		// 		out << " delete";
		// 		break;
		// 	case OP_DELETE_ARRAY:
		// 		out << " delete[]";
		// 		break;
		// 	case OP_POSITIVE:
		// 		out << '+';
		// 		break;
		// 	case OP_NEGATIVE:
		// 		out << '-';
		// 		break;
		// 	case OP_ADDRESS_OF:
		// 		out << '&';
		// 		break;
		// 	case OP_DEREFERENCE:
		// 		out << '*';
		// 		break;
		// 	case OP_COMPLEMENT:
		// 		out << '~';
		// 		break;
		// 	case OP_PLUS:
		// 		out << '+';
		// 		break;
		// 	case OP_MINUS:
		// 		out << '-';
		// 		break;
		// 	case OP_MULTIPLY:
		// 		out << '*';
		// 		break;
		// 	case OP_DIVIDE:
		// 		out << '/';
		// 		break;
		// 	case OP_REMAINDER:
		// 		out << '%';
		// 		break;
		// 	case OP_AND:
		// 		out << '&';
		// 		break;
		// 	case OP_OR:
		// 		out << '|';
		// 		break;
		// 	case OP_XOR:
		// 		out << '^';
		// 		break;
		// 	case OP_ASSIGN:
		// 		out << '=';
		// 		break;
		// 	case OP_PLUS_ASSIGN:
		// 		out << "+=";
		// 		break;
		// 	case OP_MINUS_ASSIGN:
		// 		out << "-=";
		// 		break;
		// 	case OP_MULTIPLY_ASSIGN:
		// 		out << "*=";
		// 		break;
		// 	case OP_DIVIDE_ASSIGN:
		// 		out << "/=";
		// 		break;
		// 	case OP_REMAINDER_ASSIGN:
		// 		out << "%=";
		// 		break;
		// 	case OP_AND_ASSIGN:
		// 		out << "&=";
		// 		break;
		// 	case OP_OR_ASSIGN:
		// 		out << "|=";
		// 		break;
		// 	case OP_XOR_ASSIGN:
		// 		out << "^=";
		// 		break;
		// 	case OP_LEFT_SHIFT:
		// 		out << "<<";
		// 		break;
		// 	case OP_RIGHT_SHIFT:
		// 		out << ">>";
		// 		lastWasGreater = true;
		// 		break;
		// 	case OP_LEFT_SHIFT_ASSIGN:
		// 		out << "<<=";
		// 		break;
		// 	case OP_RIGHT_SHIFT_ASSIGN:
		// 		out << ">>=";
		// 		break;
		// 	case OP_EQUAL:
		// 		out << "==";
		// 		break;
		// 	case OP_UNEQUAL:
		// 		out << "!=";
		// 		break;
		// 	case OP_LESS:
		// 		out << '<';
		// 		break;
		// 	case OP_GREATER:
		// 		out << '>';
		// 		lastWasGreater = true;
		// 		break;
		// 	case OP_LESS_EQUAL:
		// 		out << "<=";
		// 		break;
		// 	case OP_GREATER_EQUAL:
		// 		out << ">=";
		// 		break;
		// 	case OP_NOT:
		// 		out << '!';
		// 		break;
		// 	case OP_LOGICAL_AND:
		// 		out << "&&";
		// 		break;
		// 	case OP_LOGICAL_OR:
		// 		out << "||";
		// 		break;
		// 	case OP_INCREMENT:
		// 		out << "++";
		// 		break;
		// 	case OP_DECREMENT:
		// 		out << "--";
		// 		break;
		// 	case OP_COMMA:
		// 		out << ',';
		// 		break;
		// 	case OP_POINTER_TO_MEMBER:
		// 		out << "->*";
		// 		break;
		// 	case OP_POINTER:
		// 		out << "->";
		// 		break;
		// 	case OP_CALL:
		// 		out << "()";
		// 		break;
		// 	case OP_INDEX:
		// 		out << "[]";
		// 		break;
		// 	default:
		// 		out << "<unknown operator>";
		// 		lastWasGreater = true;
		// 		break;
		// }
		virtual void putOperatorName(const OperatorName&) = 0;
		// '$' + parameter
		virtual void putUndefinedTemplateParameter(unsigned) = 0;
		virtual void putBuiltinType(const BuiltinType&) = 0;
		// qualifier + ' '
		virtual void putVendorExtendedTypeQualifier(const std::string&) = 0;
		virtual void putVendorExtendedType(const std::string&) = 0;
		// with surrounding blanks
		// clamp(OP_PLUS, "+")
		// clamp(OP_MINUS, "-")
		// clamp(OP_MULTIPLY, "*")
		// clamp(OP_DIVIDE, "/")
		// clamp(OP_REMAINDER, "%")
		// clamp(OP_AND, "&")
		// clamp(OP_OR, "|")
		// clamp(OP_XOR, "^")
		// clamp(OP_ASSIGN, "=")
		// clamp(OP_PLUS_ASSIGN, "+=")
		// clamp(OP_MINUS_ASSIGN, "-=")
		// clamp(OP_MULTIPLY_ASSIGN, "*=")
		// clamp(OP_DIVIDE_ASSIGN, "/=")
		// clamp(OP_REMAINDER_ASSIGN, "%=")
		// clamp(OP_AND_ASSIGN, "&=")
		// clamp(OP_OR_ASSIGN, "|=")
		// clamp(OP_XOR_ASSIGN, "^=")
		// clamp(OP_LEFT_SHIFT_ASSIGN, "<<=")
		// clamp(OP_RIGHT_SHIFT_ASSIGN, ">>=")
		// clamp(OP_LEFT_SHIFT, "<<")
		// clamp(OP_RIGHT_SHIFT, ">>")
		// clamp(OP_EQUAL, "==")
		// clamp(OP_UNEQUAL, "!=")
		// clamp(OP_LESS, "<")
		// clamp(OP_GREATER, ">")
		// clamp(OP_LESS_EQUAL, "<=")
		// clamp(OP_GREATER_EQUAL, ">=")
		// clamp(OP_LOGICAL_AND, "&&")
		// clamp(OP_LOGICAL_OR, "||")
		// clamp(OP_COMMA, ",")
		virtual void putOperatorSymbol(const BinaryOperationExpression&) = 0;
		// switch(oper) {
		// 	case OP_POSITIVE:
		// 		out << '+';
		// 		break;
		// 	case OP_NEGATIVE:
		// 		out << '-';
		// 		break;
		// 	case OP_ADDRESS_OF:
		// 		out << '&';
		// 		break;
		// 	case OP_DEREFERENCE:
		// 		out << '*';
		// 		break;
		// 	case OP_COMPLEMENT:
		// 		out << '~';
		// 		break;
		// 	case OP_NOT:
		// 		out << '!';
		// 		break;
		// 	case OP_INCREMENT:
		// 		out << "++";
		// 		break;
		// 	case OP_DECREMENT:
		// 		out << "--";
		// 		break;
		// 	case OP_SIZEOF_EXPRESSION:
		// 	default:
		// 		out << "<unknown unary operator> ";
		// 		break;
		// }
		virtual void putOperatorSymbol(const UnaryOperationExpression&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINK_HPP */
