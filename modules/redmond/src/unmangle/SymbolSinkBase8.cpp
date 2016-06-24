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

	SymbolSinkBase8::SymbolSinkBase8(int flags) : flags(flags), specialStyle(SSMS_REDSTRAIN) {}

	SymbolSinkBase8::SymbolSinkBase8(const SymbolSinkBase8& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), SymbolWidthAdapter(sink), flags(sink.flags),
			specialStyle(sink.specialStyle) {}

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

	unsigned SymbolSinkBase8::getSeparatorWidth(Separator separator) {
#define clamp(cons, width) case cons: return width;
		switch(separator) {
			clamp(SEP_COMMA, 1u)
			clamp(SEP_AFTER_COMMA, 1u)
			clamp(SEP_LEFT_ROUND, 1u)
			clamp(SEP_RIGHT_ROUND, 1u)
			clamp(SEP_BEFORE_CV, 1u)
			clamp(SEP_AFTER_CV, 1u)
			clamp(SEP_PAAMAYIM_NEKUDOTAYIM, 2u)
			clamp(SEP_LEFT_ANGLE, 1u)
			clamp(SEP_RIGHT_ANGLE, 1u)
			clamp(SEP_DESTRUCTOR_TILDE, 1u)
			clamp(SEP_LEFT_SQUARE, 1u)
			clamp(SEP_RIGHT_SQUARE, 1u)
			clamp(SEP_RETURN_TYPE_FUNCTION, 1u)
			clamp(SEP_TYPE_VARIABLE, 1u)
			clamp(SEP_POINTER, 1u)
			clamp(SEP_REFERENCE, 1u)
			clamp(SEP_BEFORE_COMPLEX, 1u)
			clamp(SEP_TERNARY_THEN, 3u)
			clamp(SEP_TERNARY_ELSE, 3u)
#undef clamp
			default:
				return 19u;
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

	unsigned SymbolSinkBase8::getReservedWidth(Reserved reserved) {
		switch(reserved) {
#define clamp(cons, width) case cons: return width;
			clamp(RSV_RESTRICT, 8u)
			clamp(RSV_VOLATILE, 8u)
			clamp(RSV_CONST, 5u)
			clamp(RSV_CONVERSION_OPERATOR, 9u)
			clamp(RSV_COMPLEX, 8u)
			clamp(RSV_IMAGINARY, 10u)
			clamp(RSV_TRUE, 4u)
			clamp(RSV_FALSE, 5u)
			clamp(RSV_SIZEOF, 6u)
#undef clamp
			default:
				return 23u;
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

	unsigned SymbolSinkBase8::getReplacementStringWidth(ReplacementString replacement) {
		switch(replacement) {
#define clamp(cons, width) case cons: return width;
			clamp(RS_ANONYMOUS_LOCAL_NAME, 11u)
			clamp(RS_CONSTRUCTOR, 13u)
			clamp(RS_DESTRUCTOR, 12u)
			clamp(RS_UNKNOWN_CONSTRUCTOR_LIKE, 26u)
			clamp(RS_UNKNOWN_RETURN_TYPE, 21u)
			clamp(RS_UNKNOWN_TYPE_MODIFIER, 23u)
			clamp(RS_UNKNOWN_BINARY_OPERATOR, 25u)
#undef clamp
			default:
				return 28u;
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
		switch(specialStyle) {
			case SSMS_REDSTRAIN:
			default:
				writeString("<override thunk for ");
				symbol.getTargetFunction().print(*this);
				writeString(">");
				break;
			case SSMS_GCC3:
				writeString("virtual thunk to ");
				symbol.getTargetFunction().print(*this);
				break;
		}
	}

	unsigned SymbolSinkBase8::getOverrideThunkWidth(const OverrideThunkSymbol& symbol) {
		unsigned width;
		switch(specialStyle) {
			case SSMS_REDSTRAIN:
			default:
				width = 21u;
				break;
			case SSMS_GCC3:
				width = 17u;
				break;
		}
		return width + getInlineWidthOf(symbol.getTargetFunction());
	}

	void SymbolSinkBase8::putCovariantOverrideThunk(const CovariantOverrideThunkSymbol& symbol) {
		switch(specialStyle) {
			case SSMS_REDSTRAIN:
			default:
				writeString("<override thunk for ");
				symbol.getTargetFunction().print(*this);
				writeString(">");
				break;
			case SSMS_GCC3:
				writeString("virtual thunk to ");
				symbol.getTargetFunction().print(*this);
				break;
		}
	}

	unsigned SymbolSinkBase8::getCovariantOverrideThunkWidth(const CovariantOverrideThunkSymbol& symbol) {
		unsigned width;
		switch(specialStyle) {
			case SSMS_REDSTRAIN:
			default:
				width = 21u;
				break;
			case SSMS_GCC3:
				width = 17u;
				break;
		}
		return width + getInlineWidthOf(symbol.getTargetFunction());
	}

	void SymbolSinkBase8::putGuardVariable(const GuardVariableSymbol& symbol) {
		writeString("<guard variable for ");
		CurrentTemplateArguments arguments;
		symbol.getObjectName().print(*this, arguments, NULL);
		writeString(">");
	}

	unsigned SymbolSinkBase8::getGuardVariableWidth(const GuardVariableSymbol& symbol) {
		CurrentTemplateArguments arguments;
		return 21u + getInlineWidthOf(symbol.getObjectName(), arguments, NULL);
	}

	void SymbolSinkBase8::putTableRS(const TableSymbol& symbol) {
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

	void SymbolSinkBase8::putTableGCC3(const TableSymbol& symbol) {
		CurrentTemplateArguments arguments;
		switch(symbol.getTableType()) {
#define clamp(cons, str) \
	case TableSymbol::cons: \
		writeString(str " for "); \
		symbol.getTargetType().print(*this, arguments, NULL); \
		break;
			clamp(TST_VIRTUAL_TABLE, "vtable")
			clamp(TST_VTT_STRUCTURE, "VTT")
			clamp(TST_TYPEINFO_STRUCTURE, "typeinfo")
			clamp(TST_TYPEINFO_NAME, "typeinfo name")
#undef clamp
			default:
				writeString("<table object of unknown type, for type ");
				symbol.getTargetType().print(*this, arguments, NULL);
				writeString(">");
				break;
		}
	}

	void SymbolSinkBase8::putTable(const TableSymbol& symbol) {
		switch(specialStyle) {
			case SSMS_REDSTRAIN:
			default:
				putTableRS(symbol);
				break;
			case SSMS_GCC3:
				putTableGCC3(symbol);
				break;
		}
	}

	unsigned SymbolSinkBase8::getTableWidthRS(const TableSymbol& symbol) {
		unsigned width;
		switch(symbol.getTableType()) {
#define clamp(cons, w) \
	case TableSymbol::cons: \
		width = w; \
		break;
			clamp(TST_VIRTUAL_TABLE, 20u)
			clamp(TST_VTT_STRUCTURE, 20u)
			clamp(TST_TYPEINFO_STRUCTURE, 25u)
			clamp(TST_TYPEINFO_NAME, 13u)
#undef clamp
			default:
				width = 41u;
				break;
		}
		CurrentTemplateArguments arguments;
		return width + getInlineWidthOf(symbol.getTargetType(), arguments, NULL);
	}

	unsigned SymbolSinkBase8::getTableWidthGCC3(const TableSymbol& symbol) {
		unsigned width;
		switch(symbol.getTableType()) {
#define clamp(cons, w) \
	case TableSymbol::cons: \
		width = w; \
		break;
			clamp(TST_VIRTUAL_TABLE, 11u)
			clamp(TST_VTT_STRUCTURE, 8u)
			clamp(TST_TYPEINFO_STRUCTURE, 13u)
			clamp(TST_TYPEINFO_NAME, 11u)
#undef clamp
			default:
				width = 41u;
				break;
		}
		CurrentTemplateArguments arguments;
		return width + getInlineWidthOf(symbol.getTargetType(), arguments, NULL);
	}

	unsigned SymbolSinkBase8::getTableWidth(const TableSymbol& symbol) {
		switch(specialStyle) {
			case SSMS_REDSTRAIN:
			default:
				return getTableWidthRS(symbol);
			case SSMS_GCC3:
				return getTableWidthGCC3(symbol);
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

	unsigned SymbolSinkBase8::getLocalNameDiscriminatorWidth(unsigned discriminator) {
		stringstream ss;
		ss << discriminator;
		return 1u + static_cast<unsigned>(ss.tellp());
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

	unsigned SymbolSinkBase8::getOperatorNameWidth(const OperatorName& name) {
		switch(name.getOperator()) {
#define clamp(cons, width) case OperatorName::cons: return width;
			clamp(OP_NEW, 12u)
			clamp(OP_NEW_ARRAY, 14u)
			clamp(OP_DELETE, 15u)
			clamp(OP_DELETE_ARRAY, 17u)
			clamp(OP_POSITIVE, 9u)
			clamp(OP_NEGATIVE, 9u)
			clamp(OP_ADDRESS_OF, 9u)
			clamp(OP_DEREFERENCE, 9u)
			clamp(OP_COMPLEMENT, 9u)
			clamp(OP_PLUS, 9u)
			clamp(OP_MINUS, 9u)
			clamp(OP_MULTIPLY, 9u)
			clamp(OP_DIVIDE, 9u)
			clamp(OP_REMAINDER, 9u)
			clamp(OP_AND, 9u)
			clamp(OP_OR, 9u)
			clamp(OP_XOR, 9u)
			clamp(OP_ASSIGN, 9u)
			clamp(OP_PLUS_ASSIGN, 10u)
			clamp(OP_MINUS_ASSIGN, 10u)
			clamp(OP_MULTIPLY_ASSIGN, 10u)
			clamp(OP_DIVIDE_ASSIGN, 10u)
			clamp(OP_REMAINDER_ASSIGN, 10u)
			clamp(OP_AND_ASSIGN, 10u)
			clamp(OP_OR_ASSIGN, 10u)
			clamp(OP_XOR_ASSIGN, 10u)
			clamp(OP_LEFT_SHIFT, 10u)
			clamp(OP_RIGHT_SHIFT, 10u)
			clamp(OP_LEFT_SHIFT_ASSIGN, 12u)
			clamp(OP_RIGHT_SHIFT_ASSIGN, 12u)
			clamp(OP_EQUAL, 10u)
			clamp(OP_UNEQUAL, 10u)
			clamp(OP_LESS, 9u)
			clamp(OP_GREATER, 9u)
			clamp(OP_LESS_EQUAL, 10u)
			clamp(OP_GREATER_EQUAL, 10u)
			clamp(OP_NOT, 9u)
			clamp(OP_LOGICAL_AND, 10u)
			clamp(OP_LOGICAL_OR, 10u)
			clamp(OP_INCREMENT, 10u)
			clamp(OP_DECREMENT, 10u)
			clamp(OP_COMMA, 9u)
			clamp(OP_POINTER_TO_MEMBER, 11u)
			clamp(OP_POINTER, 10u)
			clamp(OP_CALL, 10u)
			clamp(OP_INDEX, 10u)
#undef clamp
			default:
				return 26u;
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

	unsigned SymbolSinkBase8::getBuiltinTypeWidth(const BuiltinType& type) {
		switch(type.getPrimitive()) {
#define clamp(cons, width) case BuiltinType::cons: return width;
			clamp(P_VOID, 4u)
			clamp(P_WCHAR_T, 7u)
			clamp(P_BOOL, 4u)
			clamp(P_CHAR, 4u)
			clamp(P_SIGNED_CHAR, 11u)
			clamp(P_UNSIGNED_CHAR, 13u)
			clamp(P_SHORT, 5u)
			clamp(P_UNSIGNED_SHORT, 14u)
			clamp(P_INT, 3u)
			clamp(P_UNSIGNED_INT, flags & SymbolSinkBase8::FL_UNSIGNED_INT ? 12u : 8u)
			clamp(P_LONG, 4u)
			clamp(P_UNSIGNED_LONG, 13u)
			clamp(P_LONG_LONG, 9u)
			clamp(P_UNSIGNED_LONG_LONG, 18u)
			clamp(P_INT128, 8u)
			clamp(P_UNSIGNED_INT128, 17u)
			clamp(P_FLOAT, 5u)
			clamp(P_DOUBLE, 6u)
			clamp(P_FLOAT128, 10u)
			clamp(P_ELLIPSIS, 3u)
#undef clamp
			default:
				return 22u;
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

	unsigned SymbolSinkBase8::getOperatorSymbolWidth(const BinaryOperationExpression& expression) {
		switch(expression.getOperator()) {
#define clamp(cons, w) case BinaryOperationExpression::cons: return w;
			clamp(OP_PLUS, 3u)
			clamp(OP_MINUS, 3u)
			clamp(OP_MULTIPLY, 3u)
			clamp(OP_DIVIDE, 3u)
			clamp(OP_REMAINDER, 3u)
			clamp(OP_AND, 3u)
			clamp(OP_OR, 3u)
			clamp(OP_XOR, 3u)
			clamp(OP_ASSIGN, 3u)
			clamp(OP_PLUS_ASSIGN, 4u)
			clamp(OP_MINUS_ASSIGN, 4u)
			clamp(OP_MULTIPLY_ASSIGN, 4u)
			clamp(OP_DIVIDE_ASSIGN, 4u)
			clamp(OP_REMAINDER_ASSIGN, 4u)
			clamp(OP_AND_ASSIGN, 4u)
			clamp(OP_OR_ASSIGN, 4u)
			clamp(OP_XOR_ASSIGN, 4u)
			clamp(OP_LEFT_SHIFT_ASSIGN, 5u)
			clamp(OP_RIGHT_SHIFT_ASSIGN, 5u)
			clamp(OP_LEFT_SHIFT, 4u)
			clamp(OP_RIGHT_SHIFT, 4u)
			clamp(OP_EQUAL, 4u)
			clamp(OP_UNEQUAL, 4u)
			clamp(OP_LESS, 3u)
			clamp(OP_GREATER, 3u)
			clamp(OP_LESS_EQUAL, 4u)
			clamp(OP_GREATER_EQUAL, 4u)
			clamp(OP_LOGICAL_AND, 4u)
			clamp(OP_LOGICAL_OR, 4u)
			clamp(OP_COMMA, 2u)
#undef clamp
			default:
				return 27u;
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

	unsigned SymbolSinkBase8::getOperatorSymbolWidth(const UnaryOperationExpression& expression) {
		switch(expression.getOperator()) {
#define clamp(cons, w) case UnaryOperationExpression::cons: return w;
			clamp(OP_POSITIVE, 1u)
			clamp(OP_NEGATIVE, 1u)
			clamp(OP_ADDRESS_OF, 1u)
			clamp(OP_DEREFERENCE, 1u)
			clamp(OP_COMPLEMENT, 1u)
			clamp(OP_NOT, 1u)
			clamp(OP_INCREMENT, 2u)
			clamp(OP_DECREMENT, 2u)
#undef clamp
			case UnaryOperationExpression::OP_SIZEOF_EXPRESSION:
			default:
				return 24u;
		}
	}

}}}
