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
			SEP_COMMA,
			SEP_AFTER_COMMA,
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
			SEP_RETURN_TYPE_FUNCTION,
			SEP_TYPE_VARIABLE,
			SEP_POINTER,
			SEP_REFERENCE,
			SEP_BEFORE_COMPLEX,
			SEP_TERNARY_THEN,
			SEP_TERNARY_ELSE
		};

		enum Reserved {
			RSV_RESTRICT,
			RSV_VOLATILE,
			RSV_CONST,
			RSV_CONVERSION_OPERATOR,
			RSV_COMPLEX,
			RSV_IMAGINARY,
			RSV_TRUE,
			RSV_FALSE,
			RSV_SIZEOF
		};

		enum ReplacementString {
			RS_ANONYMOUS_LOCAL_NAME,
			RS_CONSTRUCTOR,
			RS_DESTRUCTOR,
			RS_UNKNOWN_CONSTRUCTOR_LIKE,
			RS_UNKNOWN_RETURN_TYPE,
			RS_UNKNOWN_TYPE_MODIFIER,
			RS_UNKNOWN_BINARY_OPERATOR
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

		virtual void putOverrideThunk(const OverrideThunkSymbol&) = 0;
		virtual void putCovariantOverrideThunk(const CovariantOverrideThunkSymbol&) = 0;
		virtual void putGuardVariable(const GuardVariableSymbol&) = 0;
		virtual void putTable(const TableSymbol&) = 0;
		virtual void putLocalNameDiscriminator(unsigned) = 0;
		virtual void putOperatorName(const OperatorName&) = 0;
		virtual void putUndefinedTemplateParameter(unsigned) = 0;
		virtual void putBuiltinType(const BuiltinType&) = 0;
		virtual void putVendorExtendedTypeQualifier(const std::string&) = 0;
		virtual void putVendorExtendedType(const std::string&) = 0;
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
