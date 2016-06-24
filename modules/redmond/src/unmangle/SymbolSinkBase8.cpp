#include <sstream>

#include "TableSymbol.hpp"
#include "BuiltinType.hpp"
#include "OperatorName.hpp"
#include "SymbolSinkBase8.hpp"
#include "OverrideThunkSymbol.hpp"
#include "GuardVariableSymbol.hpp"
#include "UnaryOperationExpression.hpp"
#include "BinaryOperationExpression.hpp"
#include "CovariantOverrideThunkSymbol.hpp"

using std::string;
using std::stringstream;

namespace redengine {
namespace redmond {
namespace unmangle {

	SymbolSinkBase8::SymbolSinkBase8(int flags) : flags(flags) {}

	SymbolSinkBase8::SymbolSinkBase8(const SymbolSinkBase8& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), flags(sink.flags) {}

	void SymbolSinkBase8::putSeparator(Separator separator) {
		switch(separator) {
#define clamp(cons, str) \
	case cons: \
		writeString(str); \
		break;
			clamp(SEP_COMMA, ",")
			clamp(SEP_AFTER_COMMA, " ")
			clamp(SEP_LEFT_ROUND, "(")
			clamp(SEP_RIGHT_ROUND, ")")
			clamp(SEP_BEFORE_CV, " ")
			clamp(SEP_AFTER_CV, " ")
			clamp(SEP_PAAMAYIM_NEKUDOTAYIM, "::")
			clamp(SEP_LEFT_ANGLE, "<")
			clamp(SEP_RIGHT_ANGLE, ">")
			clamp(SEP_DESTRUCTOR_TILDE, "~")
			clamp(SEP_LEFT_SQUARE, "[")
			clamp(SEP_RIGHT_SQUARE, "]")
			clamp(SEP_RETURN_TYPE_FUNCTION, " ")
			clamp(SEP_TYPE_VARIABLE, " ")
			clamp(SEP_POINTER, "*")
			clamp(SEP_REFERENCE, "&")
			clamp(SEP_BEFORE_COMPLEX, " ")
			clamp(SEP_TERNARY_THEN, " ? ")
			clamp(SEP_TERNARY_ELSE, " : ")
#undef clamp
			default:
				writeString("<unknown-separator>");
				break;
		}
	}

	void SymbolSinkBase8::putReserved(Reserved reserved) {
		switch(reserved) {
#define clamp(cons, str, hl) \
	case cons: \
		hl(); \
		writeString(str); \
		endHighlighting(); \
		break;
			clamp(RSV_RESTRICT, "restrict", highlightCVQualifier)
			clamp(RSV_VOLATILE, "volatile", highlightCVQualifier)
			clamp(RSV_CONST, "const", highlightCVQualifier)
			case RSV_CONVERSION_OPERATOR:
				highlightKeyword();
				writeString("operator");
				endHighlighting();
				writeString(" ");
				break;
			clamp(RSV_COMPLEX, "_Complex", highlightTypeModifier)
			clamp(RSV_IMAGINARY, "_Imaginary", highlightTypeModifier)
			clamp(RSV_TRUE, "true", highlightSymbolicConstant)
			clamp(RSV_FALSE, "false", highlightSymbolicConstant)
			clamp(RSV_SIZEOF, "sizeof", highlightKeyword)
#undef clamp
			default:
				writeString("<unknown reserved word>");
				break;
		}
	}

	void SymbolSinkBase8::putReplacementString(ReplacementString replacement) {
		switch(replacement) {
#define clamp(cons, str) \
	case cons: \
		writeString(str); \
		break;
			clamp(RS_ANONYMOUS_LOCAL_NAME, "<anonymous>")
			clamp(RS_CONSTRUCTOR, "<constructor>")
			clamp(RS_DESTRUCTOR, "<destructor>")
			clamp(RS_UNKNOWN_CONSTRUCTOR_LIKE, "<unknown constructor-like>")
			clamp(RS_UNKNOWN_RETURN_TYPE, "<unknown return type>")
			clamp(RS_UNKNOWN_TYPE_MODIFIER, "<unknown type modifier>")
			clamp(RS_UNKNOWN_BINARY_OPERATOR, "<unknown binary operator>")
#undef clamp
			default:
				writeString("<unknown replacement string>");
				break;
		}
	}

	void SymbolSinkBase8::putSourceName(const string& segment) {
		highlightSourceName(segment);
		writeString(segment);
		endHighlighting();
	}

	void SymbolSinkBase8::putIntLiteral(int64_t value) {
		stringstream ss;
		ss << value;
		highlightNumericLiteral();
		writeString(ss.str());
		endHighlighting();
	}

	void SymbolSinkBase8::putIntLiteral(uint64_t value) {
		stringstream ss;
		ss << value;
		highlightNumericLiteral();
		writeString(ss.str());
		endHighlighting();
	}

	void SymbolSinkBase8::putFloatLiteral(float value) {
		stringstream ss;
		ss << value;
		highlightNumericLiteral();
		writeString(ss.str());
		endHighlighting();
	}

	void SymbolSinkBase8::putFloatLiteral(double value) {
		stringstream ss;
		ss << value;
		highlightNumericLiteral();
		writeString(ss.str());
		endHighlighting();
	}

	void SymbolSinkBase8::putOverrideThunk(const OverrideThunkSymbol& symbol) {
		writeString("<override thunk for ");
		symbol.getTargetFunction().print(*this);
		writeString(">");
	}

	void SymbolSinkBase8::putCovariantOverrideThunk(const CovariantOverrideThunkSymbol& symbol) {
		writeString("<override thunk for ");
		symbol.getTargetFunction().print(*this);
		writeString(">");
	}

	void SymbolSinkBase8::putGuardVariable(const GuardVariableSymbol& symbol) {
		writeString("<guard variable for ");
		CurrentTemplateArguments arguments;
		symbol.getObjectName().print(*this, arguments, NULL);
		writeString(">");
	}

	void SymbolSinkBase8::putTable(const TableSymbol& symbol) {
		CurrentTemplateArguments arguments;
		switch(symbol.getTableType()) {
#define clamp(cons, str) \
	case TableSymbol::cons: \
		writeString("<" str " for "); \
		symbol.getTargetType().print(*this, arguments, NULL); \
		writeString(">"); \
		break;
			clamp(TST_VIRTUAL_TABLE, "virtual table")
			clamp(TST_VTT_STRUCTURE, "VTT structure")
			clamp(TST_TYPEINFO_STRUCTURE, "typeinfo structure")
			clamp(TST_TYPEINFO_NAME, "typeinfo name")
#undef clamp
			default:
				writeString("<table object of unknown type, for type ");
				symbol.getTargetType().print(*this, arguments, NULL);
				writeString(">");
				break;
		}
	}

	void SymbolSinkBase8::putLocalNameDiscriminator(unsigned discriminator) {
		writeString("@");
		stringstream ss;
		ss << discriminator;
		highlightDiscriminator();
		writeString(ss.str());
		endHighlighting();
	}

	void SymbolSinkBase8::putOperatorName(const OperatorName& name) {
		highlightKeyword();
		writeString("operator");
		endHighlighting();
		switch(name.getOperator()) {
			case OperatorName::OP_NEW:
				writeString(" ");
				highlightKeyword();
				writeString("new");
				endHighlighting();
				break;
			case OperatorName::OP_NEW_ARRAY:
				writeString(" ");
				highlightKeyword();
				writeString("new");
				endHighlighting();
				writeString("[]");
				break;
			case OperatorName::OP_DELETE:
				writeString(" ");
				highlightKeyword();
				writeString("delete");
				endHighlighting();
				break;
			case OperatorName::OP_DELETE_ARRAY:
				writeString(" ");
				highlightKeyword();
				writeString("delete");
				endHighlighting();
				writeString("[]");
				break;
#define clamp(cons, str) \
	case OperatorName::cons: \
		writeString(str); \
		break;
			clamp(OP_POSITIVE, "+")
			clamp(OP_NEGATIVE, "-")
			clamp(OP_ADDRESS_OF, "&")
			clamp(OP_DEREFERENCE, "*")
			clamp(OP_COMPLEMENT, "~")
			clamp(OP_PLUS, "+")
			clamp(OP_MINUS, "-")
			clamp(OP_MULTIPLY, "*")
			clamp(OP_DIVIDE, "/")
			clamp(OP_REMAINDER, "%")
			clamp(OP_AND, "&")
			clamp(OP_OR, "|")
			clamp(OP_XOR, "^")
			clamp(OP_ASSIGN, "=")
			clamp(OP_PLUS_ASSIGN, "+=")
			clamp(OP_MINUS_ASSIGN, "-=")
			clamp(OP_MULTIPLY_ASSIGN, "*=")
			clamp(OP_DIVIDE_ASSIGN, "/=")
			clamp(OP_REMAINDER_ASSIGN, "%=")
			clamp(OP_AND_ASSIGN, "&=")
			clamp(OP_OR_ASSIGN, "|=")
			clamp(OP_XOR_ASSIGN, "^=")
			clamp(OP_LEFT_SHIFT, "<<")
			clamp(OP_RIGHT_SHIFT, ">>")
			clamp(OP_LEFT_SHIFT_ASSIGN, "<<=")
			clamp(OP_RIGHT_SHIFT_ASSIGN, ">>=")
			clamp(OP_EQUAL, "==")
			clamp(OP_UNEQUAL, "!=")
			clamp(OP_LESS, "<")
			clamp(OP_GREATER, ">")
			clamp(OP_LESS_EQUAL, "<=")
			clamp(OP_GREATER_EQUAL, ">=")
			clamp(OP_NOT, "!")
			clamp(OP_LOGICAL_AND, "&&")
			clamp(OP_LOGICAL_OR, "||")
			clamp(OP_INCREMENT, "++")
			clamp(OP_DECREMENT, "--")
			clamp(OP_COMMA, ",")
			clamp(OP_POINTER_TO_MEMBER, "->*")
			clamp(OP_POINTER, "->")
			clamp(OP_CALL, "()")
			clamp(OP_INDEX, "[]")
#undef clamp
			default:
				writeString("<unknown operator>");
				break;
		}
	}

	void SymbolSinkBase8::putUndefinedTemplateParameter(unsigned parameter) {
		stringstream ss;
		ss << '$' << parameter;
		highlightUndefinedTemplateParameter();
		writeString(ss.str());
		endHighlighting();
	}

	void SymbolSinkBase8::putBuiltinType(const BuiltinType& type) {
		switch(type.getPrimitive()) {
#define clamp(cons, str) \
	case BuiltinType::cons: \
		highlightBuiltinType(); \
		writeString(str); \
		endHighlighting(); \
		break;
			clamp(P_VOID, "void")
			clamp(P_WCHAR_T, "wchar_t")
			clamp(P_BOOL, "bool")
			clamp(P_CHAR, "char")
			clamp(P_SIGNED_CHAR, "signed char")
			clamp(P_UNSIGNED_CHAR, "unsigned char")
			clamp(P_SHORT, "short")
			clamp(P_UNSIGNED_SHORT, "unsigned short")
			clamp(P_INT, "int")
			clamp(P_UNSIGNED_INT, flags & SymbolSinkBase8::FL_UNSIGNED_INT ? "unsigned int" : "unsigned")
			clamp(P_LONG, "long")
			clamp(P_UNSIGNED_LONG, "unsigned long")
			clamp(P_LONG_LONG, "long long")
			clamp(P_UNSIGNED_LONG_LONG, "unsigned long long")
			clamp(P_INT128, "__int128")
			clamp(P_UNSIGNED_INT128, "unsigned __int128")
			clamp(P_FLOAT, "float")
			clamp(P_DOUBLE, "double")
			clamp(P_FLOAT128, "__float128")
#undef clamp
			case BuiltinType::P_ELLIPSIS:
				writeString("...");
				break;
			default:
				writeString("<unknown builtin type>");
				break;
		}
	}

	void SymbolSinkBase8::putVendorExtendedTypeQualifier(const string& qualifier) {
		highlightCVQualifier();
		writeString(qualifier);
		endHighlighting();
		writeString(" ");
	}

	void SymbolSinkBase8::putVendorExtendedType(const string& name) {
		highlightBuiltinType();
		writeString(name);
		endHighlighting();
	}

	void SymbolSinkBase8::putOperatorSymbol(const BinaryOperationExpression& expression) {
		switch(expression.getOperator()) {
#define clamp(cons, str) \
	case BinaryOperationExpression::cons: \
		writeString(str " "); \
		break;
			clamp(OP_PLUS, " +")
			clamp(OP_MINUS, " -")
			clamp(OP_MULTIPLY, " *")
			clamp(OP_DIVIDE, " /")
			clamp(OP_REMAINDER, " %")
			clamp(OP_AND, " &")
			clamp(OP_OR, " |")
			clamp(OP_XOR, " ^")
			clamp(OP_ASSIGN, " =")
			clamp(OP_PLUS_ASSIGN, " +=")
			clamp(OP_MINUS_ASSIGN, " -=")
			clamp(OP_MULTIPLY_ASSIGN, " *=")
			clamp(OP_DIVIDE_ASSIGN, " /=")
			clamp(OP_REMAINDER_ASSIGN, " %=")
			clamp(OP_AND_ASSIGN, " &=")
			clamp(OP_OR_ASSIGN, " |=")
			clamp(OP_XOR_ASSIGN, " ^=")
			clamp(OP_LEFT_SHIFT_ASSIGN, " <<=")
			clamp(OP_RIGHT_SHIFT_ASSIGN, " >>=")
			clamp(OP_LEFT_SHIFT, " <<")
			clamp(OP_RIGHT_SHIFT, " >>")
			clamp(OP_EQUAL, " ==")
			clamp(OP_UNEQUAL, " !=")
			clamp(OP_LESS, " <")
			clamp(OP_GREATER, " >")
			clamp(OP_LESS_EQUAL, " <=")
			clamp(OP_GREATER_EQUAL, " >=")
			clamp(OP_LOGICAL_AND, " &&")
			clamp(OP_LOGICAL_OR, " ||")
			clamp(OP_COMMA, ",")
#undef clamp
			default:
				writeString(" <unknown binary operator> ");
				break;
		}
	}

	void SymbolSinkBase8::putOperatorSymbol(const UnaryOperationExpression& expression) {
		switch(expression.getOperator()) {
#define clamp(cons, str) \
	case UnaryOperationExpression::cons: \
		writeString(str); \
		break;
			clamp(OP_POSITIVE, "+")
			clamp(OP_NEGATIVE, "-")
			clamp(OP_ADDRESS_OF, "&")
			clamp(OP_DEREFERENCE, "*")
			clamp(OP_COMPLEMENT, "~")
			clamp(OP_NOT, "!")
			clamp(OP_INCREMENT, "++")
			clamp(OP_DECREMENT, "--")
#undef clamp
			case UnaryOperationExpression::OP_SIZEOF_EXPRESSION:
			default:
				writeString("<unknown unary operator>");
				break;
		}
	}

}}}
