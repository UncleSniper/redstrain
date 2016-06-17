#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdint.h>

#include "unmangle-utils.hpp"
#include "unmangle/EnumType.hpp"
#include "unmangle/LocalName.hpp"
#include "unmangle/SourceName.hpp"
#include "unmangle/DataSymbol.hpp"
#include "unmangle/NestedName.hpp"
#include "unmangle/TableSymbol.hpp"
#include "unmangle/BuiltinType.hpp"
#include "unmangle/FunctionType.hpp"
#include "unmangle/ModifiedType.hpp"
#include "unmangle/CtorDtorName.hpp"
#include "unmangle/OperatorName.hpp"
#include "unmangle/CastExpression.hpp"
#include "unmangle/FunctionSymbol.hpp"
#include "unmangle/CVQualifiedType.hpp"
#include "unmangle/StaticArrayType.hpp"
#include "unmangle/ComplexArrayType.hpp"
#include "unmangle/BareFunctionType.hpp"
#include "unmangle/VariableArrayType.hpp"
#include "unmangle/TemplateParamType.hpp"
#include "unmangle/TemplateParamName.hpp"
#include "unmangle/VendorExtendedType.hpp"
#include "unmangle/GuardVariableSymbol.hpp"
#include "unmangle/OverrideThunkSymbol.hpp"
#include "unmangle/PointerToMemberType.hpp"
#include "unmangle/TypeTemplateArgument.hpp"
#include "unmangle/SizeOfTypeExpression.hpp"
#include "unmangle/EnumLiteralExpression.hpp"
#include "unmangle/ConversionOperatorName.hpp"
#include "unmangle/FloatLiteralExpression.hpp"
#include "unmangle/ExternalNameExpression.hpp"
#include "unmangle/DependentNameExpression.hpp"
#include "unmangle/TemplateParamExpression.hpp"
#include "unmangle/UnaryOperationExpression.hpp"
#include "unmangle/IntegerLiteralExpression.hpp"
#include "unmangle/BinaryOperationExpression.hpp"
#include "unmangle/TemplateTemplateParamType.hpp"
#include "unmangle/TernaryOperationExpression.hpp"
#include "unmangle/ExpressionTemplateArgument.hpp"
#include "unmangle/VendorExtendedQualifiedType.hpp"
#include "unmangle/CovariantOverrideThunkSymbol.hpp"

using std::map;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using redengine::redmond::unmangle::Name;
using redengine::redmond::unmangle::Type;
using redengine::redmond::unmangle::EnumType;
using redengine::redmond::unmangle::CPPSymbol;
using redengine::redmond::unmangle::LocalName;
using redengine::redmond::unmangle::CallOffset;
using redengine::redmond::unmangle::DataSymbol;
using redengine::redmond::unmangle::SourceName;
using redengine::redmond::unmangle::NestedName;
using redengine::redmond::unmangle::Expression;
using redengine::redmond::unmangle::TableSymbol;
using redengine::redmond::unmangle::BuiltinType;
using redengine::redmond::unmangle::FunctionType;
using redengine::redmond::unmangle::ModifiedType;
using redengine::redmond::unmangle::CtorDtorName;
using redengine::redmond::unmangle::OperatorName;
using redengine::redmond::unmangle::SpecialSymbol;
using redengine::redmond::unmangle::CastExpression;
using redengine::redmond::unmangle::FunctionSymbol;
using redengine::redmond::unmangle::UnqualifiedName;
using redengine::redmond::unmangle::CVQualifiedType;
using redengine::redmond::unmangle::StaticArrayType;
using redengine::redmond::unmangle::ComplexArrayType;
using redengine::redmond::unmangle::BareFunctionType;
using redengine::redmond::unmangle::TemplateArgument;
using redengine::redmond::unmangle::VariableArrayType;
using redengine::redmond::unmangle::TemplateParamType;
using redengine::redmond::unmangle::TemplateParamName;
using redengine::redmond::unmangle::LiteralExpression;
using redengine::redmond::unmangle::VendorExtendedType;
using redengine::redmond::unmangle::GuardVariableSymbol;
using redengine::redmond::unmangle::OverrideThunkSymbol;
using redengine::redmond::unmangle::PointerToMemberType;
using redengine::redmond::unmangle::TypeTemplateArgument;
using redengine::redmond::unmangle::SizeOfTypeExpression;
using redengine::redmond::unmangle::EnumLiteralExpression;
using redengine::redmond::unmangle::ConversionOperatorName;
using redengine::redmond::unmangle::FloatLiteralExpression;
using redengine::redmond::unmangle::ExternalNameExpression;
using redengine::redmond::unmangle::DependentNameExpression;
using redengine::redmond::unmangle::TemplateParamExpression;
using redengine::redmond::unmangle::IntegerLiteralExpression;
using redengine::redmond::unmangle::UnaryOperationExpression;
using redengine::redmond::unmangle::BinaryOperationExpression;
using redengine::redmond::unmangle::TemplateTemplateParamType;
using redengine::redmond::unmangle::TernaryOperationExpression;
using redengine::redmond::unmangle::ExpressionTemplateArgument;
using redengine::redmond::unmangle::VendorExtendedQualifiedType;
using redengine::redmond::unmangle::CovariantOverrideThunkSymbol;

namespace redengine {
namespace redmond {

	// ======== GCC 3 ========

	// See https://refspecs.linuxfoundation.org/cxxabi-1.75.html

	// <mangled-name> ::= _Z <encoding>
	// <encoding> ::= <function name> <bare-function-type>
	//            ::= <data name>
	//            ::= <special-name>
	// <name> ::= <nested-name>
	//        ::= <unscoped-name>
	//        ::= <unscoped-template-name> <template-args>
	//        ::= <local-name>
	// <unscoped-name> ::= <unqualified-name>
	//                 ::= St <unqualified-name>   # ::std::
	// <unscoped-template-name> ::= <unscoped-name>
	//                          ::= <substitution>
	// <nested-name> ::= N [<CV-qualifiers>] <prefix> <unqualified-name> E
	//               ::= N [<CV-qualifiers>] <template-prefix> <template-args> E
	// <prefix> ::= <prefix> <unqualified-name>
	//          ::= <template-prefix> <template-args>
	//          ::= <template-param>
	//          ::= # empty
	//          ::= <substitution>
	// <template-prefix> ::= <prefix> <template unqualified-name>
	//                   ::= <template-param>
	//                   ::= <substitution>
	// <unqualified-name> ::= <operator-name>
	//                    ::= <ctor-dtor-name>
	//                    ::= <source-name>
	// <source-name> ::= <positive length number> <identifier>
	// <number> ::= [n] <non-negative decimal integer>   # 'n' indicates '-'
	// <identifier> ::= <unqualified source code identifier>
	// <operator-name> ::= nw	# new
	//                 ::= na   # new[]
	//                 ::= dl   # delete
	//                 ::= da   # delete[]
	//                 ::= ps   # + (unary)
	//                 ::= ng   # - (unary)
	//                 ::= ad   # & (unary)
	//                 ::= de   # * (unary)
	//                 ::= co   # ~
	//                 ::= pl   # +
	//                 ::= mi   # -
	//                 ::= ml   # *
	//                 ::= dv   # /
	//                 ::= rm   # %
	//                 ::= an   # &
	//                 ::= or   # |
	//                 ::= eo   # ^
	//                 ::= aS   # =
	//                 ::= pL   # +=
	//                 ::= mI   # -=
	//                 ::= mL   # *=
	//                 ::= dV   # /=
	//                 ::= rM   # %=
	//                 ::= aN   # &=
	//                 ::= oR   # |=
	//                 ::= eO   # ^=
	//                 ::= ls   # <<
	//                 ::= rs   # >>
	//                 ::= lS   # <<=
	//                 ::= rS   # >>=
	//                 ::= eq   # ==
	//                 ::= ne   # !=
	//                 ::= lt   # <
	//                 ::= gt   # >
	//                 ::= le   # <=
	//                 ::= ge   # >=
	//                 ::= nt   # !
	//                 ::= aa   # &&
	//                 ::= oo   # ||
	//                 ::= pp   # ++
	//                 ::= mm   # --
	//                 ::= cm   # ,
	//                 ::= pm   # ->*
	//                 ::= pt   # ->
	//                 ::= cl   # ()
	//                 ::= ix   # []
	//                 ::= qu   # ?
	//                 ::= st   # sizeof (a type)
	//                 ::= sz   # sizeof (an expression)
	//                 ::= cv <type>   # (cast)
	//                 ::= v <digit> <source-name>   # vendor extended operator
	// <special-name> ::= TV <type>   # virtual table
	//                ::= TT <type>   # VTT structure (construction vtable index)
	//                ::= TI <type>   # typeinfo structure
	//                ::= TS <type>   # typeinfo name (null-terminated byte string)
	//                ::= GV <object name>   # Guard variable for one-time initialization
	//                ::= T <call-offset> <base encoding>   # virtual function override thunk
	//                      # base is the nominal target function of thunk
	//                ::= Tc <call-offset> <call-offset> <base encoding>   # covariant virtual function override thunk
	//                       # base is the nominal target function of thunk
	//                       # first call-offset is 'this' adjustment
	//                       # second call-offset is result adjustment
	// <call-offset> ::= h <nv-offset> _
	//               ::= v <v-offset> _
	// <nv-offset> ::= <offset number>   # non-virtual base override
	// <v-offset>  ::= <offset number> _ <virtual offset number>   # virtual base override, with vcall offset
	// <ctor-dtor-name> ::= C1   # complete object constructor
	//                  ::= C2   # base object constructor
	//                  ::= C3   # complete object allocating constructor
	//                  ::= D0   # deleting destructor
	//                  ::= D1   # complete object destructor
	//                  ::= D2   # base object destructor
	// <type> ::= <builtin-type>
	//        ::= <function-type>
	//        ::= <class-enum-type>
	//        ::= <array-type>
	//        ::= <pointer-to-member-type>
	//        ::= <template-param>
	//        ::= <template-template-param> <template-args>
	//        ::= <substitution>
	//        ::= <CV-qualifiers> <type>
	//        ::= P <type>   # pointer-to
	//        ::= R <type>   # reference-to
	//        ::= C <type>   # complex pair (C 2000)
	//        ::= G <type>   # imaginary (C 2000)
	//        ::= U <source-name> <type>   # vendor extended type qualifier
	// <CV-qualifiers> ::= [r] [V] [K]   # restrict (C99), volatile, const
	// <builtin-type> ::= v   # void
	//                ::= w   # wchar_t
	//                ::= b   # bool
	//                ::= c   # char
	//                ::= a   # signed char
	//                ::= h   # unsigned char
	//                ::= s   # short
	//                ::= t   # unsigned short
	//                ::= i   # int
	//                ::= j   # unsigned int
	//                ::= l   # long
	//                ::= m   # unsigned long
	//                ::= x   # long long, __int64
	//                ::= y   # unsigned long long, __int64
	//                ::= n   # __int128
	//                ::= o   # unsigned __int128
	//                ::= f   # float
	//                ::= d   # double
	//                ::= e   # long double, __float80
	//                ::= g   # __float128
	//                ::= z   # ellipsis
	//                ::= u <source-name>   # vendor extended type
	// <function-type> ::= F [Y] <bare-function-type> E   # 'Y' indicates 'extern "C"'
	// <bare-function-type> ::= <signature type>+
	//                          # types are possible return type, then parameter types
	// <class-enum-type> ::= <name>
	// <array-type> ::= A <positive dimension number> _ <element type>
	//              ::= A [<dimension expression>] _ <element type>
	// <pointer-to-member-type> ::= M <class type> <member type>
	// <template-param> ::= T_   # first template parameter
	//                  ::= T <parameter-2 non-negative number> _
	// <template-template-param> ::= <template-param>
	//                           ::= <substitution>
	// <template-args> ::= I <template-arg>+ E
	// <template-arg> ::= <type>             # type or template
	//                ::= X <expression> E   # expression
	//                ::= <expr-primary>     # simple expressions
	// <expression> ::= <unary operator-name> <expression>
	//              ::= <binary operator-name> <expression> <expression>
	//              ::= <trinary operator-name> <expression> <expression> <expression>
	//              ::= st <type>
	//              ::= <template-param>
	//              ::= sr <type> <unqualified-name>   # dependent name
	//              ::= sr <type> <unqualified-name> <template-args>   # dependent template-id
	//              ::= <expr-primary>
	// <expr-primary> ::= L <type> <value number> E
	//                ::= L <type <value float> E
	//                ::= L <mangled-name> E
	// <local-name> ::= Z <function encoding> E <entity name> [<discriminator>]
	//              ::= Z <function encoding> E s [<discriminator>]
	// <discriminator> ::= _ <non-negative number>
	// <substitution> ::= S <seq-id> _
	//                ::= S_
	//                ::= St   # std::
	//                ::= Sa   # std::allocator
	//                ::= Sb   # std::basic_string
	//                ::= Ss   # std::string (std::basic_string<char, std::char_traits<char>, std::allocator<char> >)
	//                ::= Si   # std::istream (std::basic_istream<char, std::char_traits<char> >)
	//                ::= So   # std::ostream (std::basic_ostream<char, std::char_traits<char> >)
	//                ::= Sd   # std::iostream (std::basic_iostream<char, std::char_traits<char> >)

	enum GCC3UnmangleSubstType {
		GCC3UMST_TYPE,
		GCC3UMST_UNSCOPED_TEMPLATE_NAME,
		GCC3UMST_TEMPLATE_TEMPLATE_PARAM,
		GCC3UMST_NESTED_PREFIX,
		GCC3UMST_NESTED_TEMPLATE_PREFIX
	};

	struct GCC3UnmangleSubstitution {

		GCC3UnmangleSubstType type;
		const char* begin;
		const char* end;

		GCC3UnmangleSubstitution(GCC3UnmangleSubstType type, const char* begin, const char* end)
				: type(type), begin(begin), end(end) {}

		GCC3UnmangleSubstitution(const GCC3UnmangleSubstitution& substitution)
				: type(substitution.type), begin(substitution.begin), end(substitution.end) {}

	};

	enum GCC3UnmangleSubstRef {
		GCC3UMSR_ID,
		GCC3UMSR_STD,
		GCC3UMSR_ALLOCATOR,
		GCC3UMSR_BASIC_STRING,
		GCC3UMSR_STRING,
		GCC3UMSR_ISTREAM,
		GCC3UMSR_OSTREAM,
		GCC3UMSR_IOSTREAM
	};

	typedef vector<GCC3UnmangleSubstitution> RawSBox;
	typedef map<string, unsigned> ReverseSBox;

	struct SBox {

		RawSBox raw;
		ReverseSBox reverse;

		SBox() {}
		SBox(const SBox& sbox) : raw(sbox.raw), reverse(sbox.reverse) {}

		void add(const GCC3UnmangleSubstitution& substitution,
				const char* function, const char* file, unsigned line) {
			string key(substitution.begin, static_cast<string::size_type>(substitution.end - substitution.begin));
			ReverseSBox::const_iterator it = reverse.find(key);
			if(it != reverse.end())
				return;
			unsigned index = static_cast<unsigned>(raw.size());
			reverse[key] = index;
			raw.push_back(substitution);
			if(shouldDebugUnmanglers())
				cerr << "***DEBUG: Memorizing '" << key << "' at index " << index << " in " << function << "() ["
						<< file << ':' << line << ']' << endl;
		}

		inline RawSBox::size_type size() const {
			return raw.size();
		}

		inline const GCC3UnmangleSubstitution& operator[](unsigned index) const {
			return raw[static_cast<RawSBox::size_type>(index)];
		}

	};

	void _unmangleGCC3_fail(const char* function, const string& message, const char* begin, const char* end,
			const char* file, unsigned line) {
		if(shouldDebugUnmanglers())
			cerr << "***DEBUG: Unmangling failed in " << function << "(): " << message << "; remaining input: '"
					<< string(begin, static_cast<string::size_type>(end - begin))
					<< "' [" << file << ':' << line << ']' << endl;
	}

	void _unmangleGCC3_abandon(const char* function, const char* file, unsigned line) {
		if(shouldDebugUnmanglers())
			cerr << "***DEBUG: Abandoning call to " << function << "() due to failure further in ["
					<< file << ':' << line << ']' << endl;
	}

	CPPSymbol* _unmangleGCC3_encoding(const char*&, const char*, SBox&);
	Name* _unmangleGCC3_name(const char*&, const char*, SBox&);
	BareFunctionType* _unmangleGCC3_bareFunctionType(const char*&, const char*, SBox&);
	bool _unmangleGCC3_startsType(char);
	Type* _unmangleGCC3_type(const char*&, const char*, SBox&);
	UnqualifiedName* _unmangleGCC3_unqualifiedName(const char*&, const char*, SBox&);
	TemplateArgument* _unmangleGCC3_templateArg(const char*&, const char*, SBox&);

	template<typename IntegerT>
	bool _unmangleGCC3_number(const char*& begin, const char* end, IntegerT& result) {
		if(begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_number", "Expected number", begin, end, __FILE__, __LINE__);
			return false;
		}
		bool negative;
		if(*begin == 'n') {
			negative = true;
			if(++begin == end) {
				_unmangleGCC3_fail("_unmangleGCC3_number", "Expected digits after sign",
						begin, end, __FILE__, __LINE__);
				return false;
			}
		}
		else
			negative = false;
		if(*begin < '0' || *begin > '9') {
			_unmangleGCC3_fail("_unmangleGCC3_number", "Expected digits", begin, end, __FILE__, __LINE__);
			return false;
		}
		result = static_cast<IntegerT>(0);
		do {
			IntegerT digit = static_cast<IntegerT>(*begin - '0');
			if(negative)
				result = result * static_cast<IntegerT>(10) - digit;
			else
				result = result * static_cast<IntegerT>(10) + digit;
		} while(++begin != end && *begin >= '0' && *begin <= '9');
		return true;
	}

	bool _unmangleGCC3_sourceName(const char*& begin, const char* end, string& result) {
		if(begin == end || *begin < '0' || *begin > '9') {
			_unmangleGCC3_fail("_unmangleGCC3_sourceName", "Expected length", begin, end, __FILE__, __LINE__);
			return false;
		}
		unsigned length;
		if(!_unmangleGCC3_number<unsigned>(begin, end, length)) {
			_unmangleGCC3_abandon("_unmangleGCC3_sourceName", __FILE__, __LINE__);
			return false;
		}
		const char* newBegin = begin + length;
		if(newBegin > end) {
			_unmangleGCC3_fail("_unmangleGCC3_sourceName", "Length exceeds name", begin, end, __FILE__, __LINE__);
			return false;
		}
		result.assign(begin, static_cast<string::size_type>(length));
		begin = newBegin;
		return true;
	}

	bool _unmangleGCC3_substitution(const char*& begin, const char* end, GCC3UnmangleSubstRef& ref, unsigned& id) {
		if(begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_substitution", "Expected specifier after 'S'",
					begin, end, __FILE__, __LINE__);
			return false;
		}
		switch(*begin) {
			case '_':
				++begin;
				id = 0u;
				ref = GCC3UMSR_ID;
				return true;
			case 't':
				++begin;
				ref = GCC3UMSR_STD;
				return true;
			case 'a':
				++begin;
				ref = GCC3UMSR_ALLOCATOR;
				return true;
			case 'b':
				++begin;
				ref = GCC3UMSR_BASIC_STRING;
				return true;
			case 's':
				++begin;
				ref = GCC3UMSR_STRING;
				return true;
			case 'i':
				++begin;
				ref = GCC3UMSR_ISTREAM;
				return true;
			case 'o':
				++begin;
				ref = GCC3UMSR_OSTREAM;
				return true;
			case 'd':
				++begin;
				ref = GCC3UMSR_IOSTREAM;
				return true;
			default:
				break;
		}
		if(*begin >= '0' && *begin <= '9')
			id = static_cast<unsigned>(*begin - '0');
		else if(*begin >= 'A' && *begin <= 'Z')
			id = static_cast<unsigned>(*begin - 'A') + 10u;
		else {
			_unmangleGCC3_fail("_unmangleGCC3_substitution", "Expected valid specifier after 'S'",
					begin, end, __FILE__, __LINE__);
			return false;
		}
		ref = GCC3UMSR_ID;
		for(;;) {
			if(++begin == end) {
				_unmangleGCC3_fail("_unmangleGCC3_substitution", "Missing '_' terminator",
						begin, end, __FILE__, __LINE__);
				return false;
			}
			if(*begin == '_') {
				++begin;
				++id;
				return true;
			}
			if(*begin >= '0' && *begin <= '9')
				id = id * 36u + static_cast<unsigned>(*begin - '0');
			else if(*begin >= 'A' && *begin <= 'Z')
				id = id * 36u + static_cast<unsigned>(*begin - 'A') + 10u;
			else {
				_unmangleGCC3_fail("_unmangleGCC3_substitution", "Invalid base36 digit",
						begin, end, __FILE__, __LINE__);
				return false;
			}
		}
	}

	template<typename IntegerT>
	IntegerLiteralExpression<IntegerT>* _unmangleGCC3_intLiteral(BuiltinType::Primitive type,
			const char*& begin, const char* end) {
		IntegerT value;
		if(!_unmangleGCC3_number<IntegerT>(begin, end, value)) {
			_unmangleGCC3_abandon("_unmangleGCC3_intLiteral", __FILE__, __LINE__);
			return NULL;
		}
		if(begin == end || *begin != 'E') {
			_unmangleGCC3_fail("_unmangleGCC3_intLiteral", "Missing 'E' terminator", begin, end, __FILE__, __LINE__);
			return NULL;
		}
		++begin;
		return new IntegerLiteralExpression<IntegerT>(type, value);
	}

	LiteralExpression* _unmangleGCC3_literal(const Type& type, const char*& begin, const char* end) {
		switch(type.getTypeType()) {
			case Type::TT_BUILTIN:
				{
					const BuiltinType& bt = static_cast<const BuiltinType&>(type);
					BuiltinType::Primitive pt = bt.getPrimitive();
					switch(pt) {
						case BuiltinType::P_WCHAR_T:
							return _unmangleGCC3_intLiteral<wchar_t>(pt, begin, end);
						case BuiltinType::P_BOOL:
							return _unmangleGCC3_intLiteral<bool>(pt, begin, end);
						case BuiltinType::P_CHAR:
							return _unmangleGCC3_intLiteral<char>(pt, begin, end);
						case BuiltinType::P_SIGNED_CHAR:
							return _unmangleGCC3_intLiteral<signed char>(pt, begin, end);
						case BuiltinType::P_UNSIGNED_CHAR:
							return _unmangleGCC3_intLiteral<unsigned char>(pt, begin, end);
						case BuiltinType::P_SHORT:
							return _unmangleGCC3_intLiteral<short>(pt, begin, end);
						case BuiltinType::P_UNSIGNED_SHORT:
							return _unmangleGCC3_intLiteral<unsigned short>(pt, begin, end);
						case BuiltinType::P_INT:
							return _unmangleGCC3_intLiteral<int>(pt, begin, end);
						case BuiltinType::P_UNSIGNED_INT:
							return _unmangleGCC3_intLiteral<unsigned>(pt, begin, end);
						case BuiltinType::P_LONG:
							return _unmangleGCC3_intLiteral<long>(pt, begin, end);
						case BuiltinType::P_UNSIGNED_LONG:
							return _unmangleGCC3_intLiteral<unsigned long>(pt, begin, end);
						case BuiltinType::P_LONG_LONG:
							return _unmangleGCC3_intLiteral<long long>(pt, begin, end);
						case BuiltinType::P_UNSIGNED_LONG_LONG:
							return _unmangleGCC3_intLiteral<unsigned long long>(pt, begin, end);
						// Documentations differ in how floating-point literals work and
						// I frankly don't see how they could even turn up in the mangling,
						// so I can't try it out. So yeah, floats are not supported...
						default:
							_unmangleGCC3_fail("_unmangleGCC3_literal", "Unsupported builtin type",
									begin, end, __FILE__, __LINE__);
							return NULL;
					}
				}
			case Type::TT_ENUM:
				{
					const EnumType& et = static_cast<const EnumType&>(type);
					int value;
					if(!_unmangleGCC3_number<int>(begin, end, value)) {
						_unmangleGCC3_abandon("_unmangleGCC3_literal", __FILE__, __LINE__);
						return NULL;
					}
					if(begin == end || *begin != 'E') {
						_unmangleGCC3_fail("_unmangleGCC3_literal", "Missing 'E' terminator",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					++begin;
					UnmanglePtr<Name> typeName(et.getName().cloneName());
					EnumLiteralExpression* ele = new EnumLiteralExpression(typeName.ptr, value);
					typeName.ptr = NULL;
					return ele;
				}
			default:
				_unmangleGCC3_fail("_unmangleGCC3_literal", "Unsupported type", begin, end, __FILE__, __LINE__);
				return NULL;
		}
	}

	Expression* _unmangleGCC3_expression(const char*& begin, const char* end, SBox& sbox) {
		if(begin == end)
			return NULL;
		UnaryOperationExpression::Operator op1;
		BinaryOperationExpression::Operator op2;
		switch(*begin) {
			case 'p':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'p'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 's':
						op1 = UnaryOperationExpression::OP_POSITIVE;
					  makeUnary:
						{
							++begin;
							UnmanglePtr<Expression> operand(_unmangleGCC3_expression(begin, end, sbox));
							if(!operand.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
								return NULL;
							}
							UnaryOperationExpression* unary = new UnaryOperationExpression(op1, operand.ptr);
							operand.ptr = NULL;
							return unary;
						}
					case 'p':
						op1 = UnaryOperationExpression::OP_INCREMENT;
						goto makeUnary;
					case 'l':
						op2 = BinaryOperationExpression::OP_PLUS;
					  makeBinary:
						{
							++begin;
							UnmanglePtr<Expression> leftOperand(_unmangleGCC3_expression(begin, end, sbox));
							if(!leftOperand.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
								return NULL;
							}
							UnmanglePtr<Expression> rightOperand(_unmangleGCC3_expression(begin, end, sbox));
							if(!rightOperand.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
								return NULL;
							}
							BinaryOperationExpression* binary = new BinaryOperationExpression(op2,
									leftOperand.ptr, rightOperand.ptr);
							leftOperand.ptr = rightOperand.ptr = NULL;
							return binary;
						}
					case 'L':
						op2 = BinaryOperationExpression::OP_PLUS;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'p'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'n':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'n'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'g':
						op1 = UnaryOperationExpression::OP_NEGATIVE;
						goto makeUnary;
					case 't':
						op1 = UnaryOperationExpression::OP_NOT;
						goto makeUnary;
					case 'e':
						op2 = BinaryOperationExpression::OP_UNEQUAL;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'n'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'a':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'a'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'd':
						op1 = UnaryOperationExpression::OP_ADDRESS_OF;
						goto makeUnary;
					case 'n':
						op2 = BinaryOperationExpression::OP_AND;
						goto makeBinary;
					case 'S':
						op2 = BinaryOperationExpression::OP_ASSIGN;
						goto makeBinary;
					case 'N':
						op2 = BinaryOperationExpression::OP_AND_ASSIGN;
						goto makeBinary;
					case 'a':
						op2 = BinaryOperationExpression::OP_LOGICAL_AND;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'a'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'd':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'd'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'e':
						op1 = UnaryOperationExpression::OP_DEREFERENCE;
						goto makeUnary;
					case 'v':
						op2 = BinaryOperationExpression::OP_DIVIDE;
						goto makeBinary;
					case 'V':
						op2 = BinaryOperationExpression::OP_DIVIDE_ASSIGN;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'd'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'c':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'c'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'o':
						op1 = UnaryOperationExpression::OP_COMPLEMENT;
						goto makeUnary;
					case 'm':
						op2 = BinaryOperationExpression::OP_COMMA;
						goto makeBinary;
					case 'v':
						{
							++begin;
							UnmanglePtr<Type> type(_unmangleGCC3_type(begin, end, sbox));
							if(!type.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
								return NULL;
							}
							UnmanglePtr<Expression> operand(_unmangleGCC3_expression(begin, end, sbox));
							if(!operand.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
								return NULL;
							}
							CastExpression* cast = new CastExpression(type.ptr, operand.ptr);
							type.ptr = NULL;
							operand.ptr = NULL;
							return cast;
						}
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'c'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 's':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 's'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'z':
						op1 = UnaryOperationExpression::OP_SIZEOF_EXPRESSION;
						goto makeUnary;
					case 't':
						{
							++begin;
							UnmanglePtr<Type> type(_unmangleGCC3_type(begin, end, sbox));
							if(!type.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
								return NULL;
							}
							SizeOfTypeExpression* sot = new SizeOfTypeExpression(type.ptr);
							type.ptr = NULL;
							return sot;
						}
					case 'r':
						{
							++begin;
							UnmanglePtr<Type> type(_unmangleGCC3_type(begin, end, sbox));
							if(!type.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
								return NULL;
							}
							UnmanglePtr<UnqualifiedName> name(_unmangleGCC3_unqualifiedName(begin, end, sbox));
							if(!name.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
								return NULL;
							}
							UnmanglePtr<DependentNameExpression> dname(new DependentNameExpression(type.ptr,
									name.ptr));
							type.ptr = NULL;
							name.ptr = NULL;
							if(begin != end && *begin == 'I') {
								if(++begin == end) {
									_unmangleGCC3_fail("_unmangleGCC3_expression",
											"Missing actual arguments after 'sr...I'",
											begin, end, __FILE__, __LINE__);
									return NULL;
								}
								UnmanglePtr<TemplateArgument> arg(NULL);
								do {
									arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
									if(!arg.ptr) {
										_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
										return NULL;
									}
									if(begin == end) {
										_unmangleGCC3_fail("_unmangleGCC3_expression",
												"Missing 'E' terminator after 'sr...I...'",
												begin, end, __FILE__, __LINE__);
										return NULL;
									}
									dname.ptr->addArgument(*arg.ptr);
									arg.ptr = NULL;
								} while(*begin != 'E');
								++begin;
							}
							DependentNameExpression* dn = dname.ptr;
							dname.ptr = NULL;
							return dn;
						}
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 's'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'm':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'm'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'm':
						op1 = UnaryOperationExpression::OP_DECREMENT;
						goto makeUnary;
					case 'i':
						op2 = BinaryOperationExpression::OP_PLUS;
						goto makeBinary;
					case 'l':
						op2 = BinaryOperationExpression::OP_MULTIPLY;
						goto makeBinary;
					case 'I':
						op2 = BinaryOperationExpression::OP_MINUS_ASSIGN;
						goto makeBinary;
					case 'L':
						op2 = BinaryOperationExpression::OP_MULTIPLY_ASSIGN;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'm'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'r':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'r'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'm':
						op2 = BinaryOperationExpression::OP_REMAINDER;
						goto makeBinary;
					case 'M':
						op2 = BinaryOperationExpression::OP_REMAINDER_ASSIGN;
						goto makeBinary;
					case 's':
						op2 = BinaryOperationExpression::OP_RIGHT_SHIFT;
						goto makeBinary;
					case 'S':
						op2 = BinaryOperationExpression::OP_RIGHT_SHIFT_ASSIGN;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'r'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'o':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'o'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'r':
						op2 = BinaryOperationExpression::OP_OR;
						goto makeBinary;
					case 'R':
						op2 = BinaryOperationExpression::OP_OR_ASSIGN;
						goto makeBinary;
					case 'o':
						op2 = BinaryOperationExpression::OP_LOGICAL_OR;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'o'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'e':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'e'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'o':
						op2 = BinaryOperationExpression::OP_XOR;
						goto makeBinary;
					case 'O':
						op2 = BinaryOperationExpression::OP_XOR_ASSIGN;
						goto makeBinary;
					case 'q':
						op2 = BinaryOperationExpression::OP_EQUAL;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'e'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'l':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'l'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 's':
						op2 = BinaryOperationExpression::OP_LEFT_SHIFT;
						goto makeBinary;
					case 'S':
						op2 = BinaryOperationExpression::OP_LEFT_SHIFT_ASSIGN;
						goto makeBinary;
					case 't':
						op2 = BinaryOperationExpression::OP_LESS;
						goto makeBinary;
					case 'e':
						op2 = BinaryOperationExpression::OP_LESS_EQUAL;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'l'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'g':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'g'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 't':
						op2 = BinaryOperationExpression::OP_GREATER;
						goto makeBinary;
					case 'e':
						op2 = BinaryOperationExpression::OP_GREATER_EQUAL;
						goto makeBinary;
					default:
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Unrecognized second letter after operator name initiator 'g'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'q':
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Missing second letter after operator name initiator 'q'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				if(*begin != 'u') {
					_unmangleGCC3_fail("_unmangleGCC3_expression",
							"Unrecognized second letter after operator name initiator 'q'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				{
					++begin;
					UnmanglePtr<Expression> condition(_unmangleGCC3_expression(begin, end, sbox));
					if(!condition.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
						return NULL;
					}
					UnmanglePtr<Expression> thenBranch(_unmangleGCC3_expression(begin, end, sbox));
					if(!thenBranch.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
						return NULL;
					}
					UnmanglePtr<Expression> elseBranch(_unmangleGCC3_expression(begin, end, sbox));
					if(!elseBranch.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
						return NULL;
					}
					TernaryOperationExpression* ternary = new TernaryOperationExpression(condition.ptr,
							thenBranch.ptr, elseBranch.ptr);
					condition.ptr = thenBranch.ptr = elseBranch.ptr = NULL;
					return ternary;
				}
			case 'T':
				{
					if(++begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_expression",
								"Missing '_' terminator after 'T'", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					unsigned pindex;
					if(*begin == '_')
						pindex = 0u;
					else if(*begin >= '0' && *begin <= '9') {
						if(!_unmangleGCC3_number<unsigned>(begin, end, pindex)) {
							_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
							return NULL;
						}
						++pindex;
						if(begin == end || *begin != '_') {
							_unmangleGCC3_fail("_unmangleGCC3_expression",
									"Missing '_' terminator after 'T...'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
					}
					else {
						_unmangleGCC3_fail("_unmangleGCC3_expression", "Invalid input after 'T'",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					++begin;
					return new TemplateParamExpression(pindex);
				}
			case 'L':
				{
					if(++begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_expression", "Missing primary after 'L'",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					if(*begin == '_') {
						if(++begin == end || *begin != 'Z' || ++begin == end) {
							_unmangleGCC3_fail("_unmangleGCC3_expression",
									"Invalid start of mangled name after 'L'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
						UnmanglePtr<CPPSymbol> symbol(_unmangleGCC3_encoding(begin, end, sbox));
						if(!symbol.ptr) {
							_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
							return NULL;
						}
						if(begin == end || *begin != 'E') {
							_unmangleGCC3_fail("_unmangleGCC3_expression",
									"Missing 'E' terminator after 'L_Z...'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
						++begin;
						ExternalNameExpression* ene = new ExternalNameExpression(symbol.ptr);
						symbol.ptr = NULL;
						return ene;
					}
					UnmanglePtr<Type> type(_unmangleGCC3_type(begin, end, sbox));
					if(!type.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_expression", __FILE__, __LINE__);
						return NULL;
					}
					return _unmangleGCC3_literal(*type.ptr, begin, end);
				}
			default:
				_unmangleGCC3_fail("_unmangleGCC3_expression", "Unrecognized start of expression",
						begin, end, __FILE__, __LINE__);
				return NULL;
		}
	}

	TemplateArgument* _unmangleGCC3_templateArg(const char*& begin, const char* end, SBox& sbox) {
		switch(*begin) {
			case 'X':
				{
					++begin;
					UnmanglePtr<Expression> expr(_unmangleGCC3_expression(begin, end, sbox));
					if(!expr.ptr)
						return NULL;
					if(begin == end || *begin != 'E') {
						_unmangleGCC3_fail("_unmangleGCC3_templateArg",
								"Missing 'E' terminator after 'X...'", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					++begin;
					ExpressionTemplateArgument* arg = new ExpressionTemplateArgument(expr.ptr);
					expr.ptr = NULL;
					return arg;
				}
			case 'L':
				{
					UnmanglePtr<Expression> expr(_unmangleGCC3_expression(begin, end, sbox));
					if(!expr.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_templateArg", __FILE__, __LINE__);
						return NULL;
					}
					ExpressionTemplateArgument* arg = new ExpressionTemplateArgument(expr.ptr);
					expr.ptr = NULL;
					return arg;
				}
			default:
				if(!_unmangleGCC3_startsType(*begin)) {
					_unmangleGCC3_fail("_unmangleGCC3_templateArg", "Unrecognized start of argument",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				{
					UnmanglePtr<Type> type(_unmangleGCC3_type(begin, end, sbox));
					if(!type.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_templateArg", __FILE__, __LINE__);
						return NULL;
					}
					TypeTemplateArgument* arg = new TypeTemplateArgument(type.ptr);
					type.ptr = NULL;
					return arg;
				}
		}
	}

	UnqualifiedName* _unmangleGCC3_unqualifiedName(const char*& begin, const char* end, SBox& sbox) {
		if(begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName", "Expected unqualified name",
					begin, end, __FILE__, __LINE__);
			return NULL;
		}
		switch(*begin) {
			case 'n':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'n'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'w':
						++begin;
						return new OperatorName(OperatorName::OP_NEW);
					case 'a':
						++begin;
						return new OperatorName(OperatorName::OP_NEW_ARRAY);
					case 'g':
						++begin;
						return new OperatorName(OperatorName::OP_NEGATIVE);
					case 'e':
						++begin;
						return new OperatorName(OperatorName::OP_UNEQUAL);
					case 't':
						++begin;
						return new OperatorName(OperatorName::OP_NOT);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'n'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'd':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'd'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'l':
						++begin;
						return new OperatorName(OperatorName::OP_DELETE);
					case 'a':
						++begin;
						return new OperatorName(OperatorName::OP_DELETE_ARRAY);
					case 'e':
						++begin;
						return new OperatorName(OperatorName::OP_DEREFERENCE);
					case 'v':
						++begin;
						return new OperatorName(OperatorName::OP_DIVIDE);
					case 'V':
						++begin;
						return new OperatorName(OperatorName::OP_DIVIDE_ASSIGN);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'd'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'p':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'p'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 's':
						++begin;
						return new OperatorName(OperatorName::OP_POSITIVE);
					case 'l':
						++begin;
						return new OperatorName(OperatorName::OP_PLUS);
					case 'L':
						++begin;
						return new OperatorName(OperatorName::OP_PLUS_ASSIGN);
					case 'p':
						++begin;
						return new OperatorName(OperatorName::OP_INCREMENT);
					case 'm':
						++begin;
						return new OperatorName(OperatorName::OP_POINTER_TO_MEMBER);
					case 't':
						++begin;
						return new OperatorName(OperatorName::OP_POINTER);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'p'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'a':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'a'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'd':
						++begin;
						return new OperatorName(OperatorName::OP_ADDRESS_OF);
					case 'n':
						++begin;
						return new OperatorName(OperatorName::OP_AND);
					case 'S':
						++begin;
						return new OperatorName(OperatorName::OP_ASSIGN);
					case 'N':
						++begin;
						return new OperatorName(OperatorName::OP_AND_ASSIGN);
					case 'a':
						++begin;
						return new OperatorName(OperatorName::OP_LOGICAL_AND);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'a'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'c':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'c'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'o':
						++begin;
						return new OperatorName(OperatorName::OP_COMPLEMENT);
					case 'm':
						++begin;
						return new OperatorName(OperatorName::OP_COMMA);
					case 'l':
						++begin;
						return new OperatorName(OperatorName::OP_CALL);
					case 'v':
						{
							++begin;
							UnmanglePtr<Type> targetType(_unmangleGCC3_type(begin, end, sbox));
							if(!targetType.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_unqualifiedName", __FILE__, __LINE__);
								return NULL;
							}
							ConversionOperatorName* con = new ConversionOperatorName(targetType.ptr);
							targetType.ptr = NULL;
							return con;
						}
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'c'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'm':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'm'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'i':
						++begin;
						return new OperatorName(OperatorName::OP_MINUS);
					case 'l':
						++begin;
						return new OperatorName(OperatorName::OP_MULTIPLY);
					case 'I':
						++begin;
						return new OperatorName(OperatorName::OP_MINUS_ASSIGN);
					case 'L':
						++begin;
						return new OperatorName(OperatorName::OP_MULTIPLY_ASSIGN);
					case 'm':
						++begin;
						return new OperatorName(OperatorName::OP_DECREMENT);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'm'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'r':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'r'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'm':
						++begin;
						return new OperatorName(OperatorName::OP_REMAINDER);
					case 'M':
						++begin;
						return new OperatorName(OperatorName::OP_REMAINDER_ASSIGN);
					case 's':
						++begin;
						return new OperatorName(OperatorName::OP_RIGHT_SHIFT);
					case 'S':
						++begin;
						return new OperatorName(OperatorName::OP_RIGHT_SHIFT_ASSIGN);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'r'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'o':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'o'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'r':
						++begin;
						return new OperatorName(OperatorName::OP_OR);
					case 'R':
						++begin;
						return new OperatorName(OperatorName::OP_OR_ASSIGN);
					case 'o':
						++begin;
						return new OperatorName(OperatorName::OP_LOGICAL_OR);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'o'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'e':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'e'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 'o':
						++begin;
						return new OperatorName(OperatorName::OP_XOR);
					case 'O':
						++begin;
						return new OperatorName(OperatorName::OP_XOR_ASSIGN);
					case 'q':
						++begin;
						return new OperatorName(OperatorName::OP_EQUAL);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'e'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'l':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'l'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 's':
						++begin;
						return new OperatorName(OperatorName::OP_LEFT_SHIFT);
					case 'S':
						++begin;
						return new OperatorName(OperatorName::OP_LEFT_SHIFT_ASSIGN);
					case 't':
						++begin;
						return new OperatorName(OperatorName::OP_LESS);
					case 'e':
						++begin;
						return new OperatorName(OperatorName::OP_LESS_EQUAL);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'l'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'g':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'g'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case 't':
						++begin;
						return new OperatorName(OperatorName::OP_GREATER);
					case 'e':
						++begin;
						return new OperatorName(OperatorName::OP_GREATER_EQUAL);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second letter after operator name initiator 'g'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'i':   // operator-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second letter after operator name initiator 'i'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				if(*begin != 'x') {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Unrecognized second letter after operator name initiator 'i'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				++begin;
				return new OperatorName(OperatorName::OP_INDEX);
			case 'C':   // ctor-dtor-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second character after ctor/dtor name initiator 'C'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case '1':
						++begin;
						return new CtorDtorName(CtorDtorName::FN_COMPLETE_CTOR);
					case '2':
						++begin;
						return new CtorDtorName(CtorDtorName::FN_BASE_CTOR);
					case '3':
						++begin;
						return new CtorDtorName(CtorDtorName::FN_ALLOCATING_CTOR);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second character after ctor/dtor name initiator 'C'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			case 'D':   // ctor-dtor-name
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
							"Missing second character after ctor/dtor name initiator 'D'",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				switch(*begin) {
					case '0':
						++begin;
						return new CtorDtorName(CtorDtorName::FN_DELETING_DTOR);
					case '1':
						++begin;
						return new CtorDtorName(CtorDtorName::FN_COMPLETE_DTOR);
					case '2':
						++begin;
						return new CtorDtorName(CtorDtorName::FN_BASE_DTOR);
					default:
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized second character after ctor/dtor name initiator 'D'",
								begin, end, __FILE__, __LINE__);
						return NULL;
				}
			default:
				{
					if(*begin < '0' || *begin > '9') {
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Unrecognized start of unqualified name", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					unsigned length;
					if(!_unmangleGCC3_number<unsigned>(begin, end, length)) {
						_unmangleGCC3_abandon("_unmangleGCC3_unqualifiedName", __FILE__, __LINE__);
						return NULL;
					}
					const char* newBegin = begin + length;
					if(!length || newBegin > end) {
						_unmangleGCC3_fail("_unmangleGCC3_unqualifiedName",
								"Invalid length of source name", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					string segment(begin, static_cast<string::size_type>(length));
					begin = newBegin;
					return new SourceName(segment);
				}
		}
	}

	NestedName* _unmangleGCC3_nestedNameImpl(const char*& begin, const char* end, SBox& sbox, bool writeSBox,
			int qualifiers, NestedName::Segment*& argumentSink) {
		// token -v   state -> | empty | element-or-template | definite-element
		// --------------------+-------+---------------------+-----------------
		// unqualified-name    |   y   |          y          |        y
		// template-param      |   y   |          n          |        n
		// template-args       |   n   |          y          |        n
		UnmanglePtr<NestedName> nested(new NestedName(qualifiers));
		bool frontIsTemplateParam = false, frontIsSubstitution = false, substitutionExtended = false;
		const char* stdTplName;
		bool stdIsTemplate;
		const char* oldBegin = begin;
		while(begin != end) {
			switch(*begin) {
				case 'n':   // operator-name
				case 'd':   // operator-name
				case 'p':   // operator-name
				case 'a':   // operator-name
				case 'c':   // operator-name
				case 'm':   // operator-name
				case 'r':   // operator-name
				case 'o':   // operator-name
				case 'e':   // operator-name
				case 'l':   // operator-name
				case 'g':   // operator-name
				case 'i':   // operator-name
				case 'q':   // operator-name
				case 's':   // operator-name
				case 'v':   // operator-name
				case 'C':   // ctor-dtor-name
				case 'D':   // ctor-dtor-name
					// unqualified-name
					goto unqualName;
				default:
					if(*begin < '0' || *begin > '9') {
						_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
								"Invalid start of nested name segment", begin, end, __FILE__, __LINE__);
						return NULL;
					}
				  unqualName:
					{
						if(writeSBox && (!frontIsSubstitution || substitutionExtended) && nested.ptr->hasSegments()) {
							if(frontIsTemplateParam)
								sbox.add(GCC3UnmangleSubstitution(GCC3UMST_NESTED_TEMPLATE_PREFIX,
										oldBegin, begin), "_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
							else if(!argumentSink)
								sbox.add(GCC3UnmangleSubstitution(GCC3UMST_NESTED_PREFIX, oldBegin, begin),
										"_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
							else if(nested.ptr->getSegmentCount() == 1u)
								sbox.add(GCC3UnmangleSubstitution(GCC3UMST_UNSCOPED_TEMPLATE_NAME,
										oldBegin, begin), "_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
							else
								sbox.add(GCC3UnmangleSubstitution(GCC3UMST_NESTED_TEMPLATE_PREFIX,
										oldBegin, begin), "_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
						}
						UnmanglePtr<UnqualifiedName> uqname(_unmangleGCC3_unqualifiedName(begin, end, sbox));
						if(!uqname.ptr) {
							_unmangleGCC3_abandon("_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
							return NULL;
						}
						UnmanglePtr<NestedName::Segment> segment(new NestedName::Segment(uqname.ptr));
						uqname.ptr = NULL;
						nested.ptr->addSegment(*segment.ptr);
						argumentSink = segment.ptr;
						segment.ptr = NULL;
					}
					substitutionExtended = true;
					break;
				case 'T':
					// template-param
					if(nested.ptr->hasSegments()) {
						_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
								"A 'T' segment can only occur at the start of a nested name",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					{
						if(++begin == end) {
							_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
									"Missing '_' terminator after 'T'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
						unsigned pindex;
						if(*begin == '_')
							pindex = 0u;
						else if(*begin >= '0' && *begin <= '9') {
							if(!_unmangleGCC3_number<unsigned>(begin, end, pindex)) {
								_unmangleGCC3_abandon("_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
								return NULL;
							}
							++pindex;
							if(begin == end || *begin != '_') {
								_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
										"Missing '_' terminator after 'T...'", begin, end, __FILE__, __LINE__);
								return NULL;
							}
						}
						else {
							_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl", "Invalid input after 'T'",
									begin, end, __FILE__, __LINE__);
							return NULL;
						}
						++begin;
						UnmanglePtr<TemplateParamName> tpname(new TemplateParamName(pindex));
						UnmanglePtr<NestedName::Segment> segment(new NestedName::Segment(tpname.ptr));
						tpname.ptr = NULL;
						nested.ptr->addSegment(*segment.ptr);
						argumentSink = segment.ptr;
						segment.ptr = NULL;
						frontIsTemplateParam = true;
					}
					break;
				case 'I':
					// template-args
					if(!argumentSink) {
						_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
								"An 'I' segment can only occur after a proper name", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					{
						if(writeSBox && !frontIsSubstitution)
							sbox.add(GCC3UnmangleSubstitution(GCC3UMST_NESTED_TEMPLATE_PREFIX,
									oldBegin, begin), "_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
						if(++begin == end) {
							_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
									"Missing actual arguments after 'I'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
						UnmanglePtr<TemplateArgument> arg(NULL);
						do {
							arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
							if(!arg.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
								return NULL;
							}
							if(begin == end) {
								_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
										"Missing 'E' terminator after 'I...'", begin, end, __FILE__, __LINE__);
								return NULL;
							}
							argumentSink->addArgument(*arg.ptr);
							arg.ptr = NULL;
						} while(*begin != 'E');
						++begin;
						argumentSink = NULL;
					}
					substitutionExtended = true;
					break;
				case 'S':
					if(nested.ptr->hasSegments()) {
						_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
								"A 'S' segment can only occur at the start of a nested name",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					{
						GCC3UnmangleSubstRef ref;
						unsigned sid;
						++begin;
						if(!_unmangleGCC3_substitution(begin, end, ref, sid)) {
							_unmangleGCC3_abandon("_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
							return NULL;
						}
						switch(ref) {
							case GCC3UMSR_ID:
								{
									if(sid >= static_cast<unsigned>(sbox.size())) {
										stringstream ss;
										ss << sid;
										_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
												"Substitution index out of bounds: " + ss.str(),
												begin, end, __FILE__, __LINE__);
										return NULL;
									}
									const GCC3UnmangleSubstitution& substitution = sbox[sid];
									switch(substitution.type) {
										case GCC3UMST_UNSCOPED_TEMPLATE_NAME:
										case GCC3UMST_NESTED_PREFIX:
										case GCC3UMST_NESTED_TEMPLATE_PREFIX:
											break;
										default:
											{
												stringstream ss;
												ss << static_cast<int>(substitution.type);
												_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
														"Rejected substitution type " + ss.str(),
														begin, end, __FILE__, __LINE__);
											}
											return NULL;
									}
									delete nested.ptr;
									nested.ptr = NULL;
									const char* sbegin = substitution.begin;
									nested.ptr = _unmangleGCC3_nestedNameImpl(sbegin, substitution.end, sbox,
											false, qualifiers, argumentSink);
									if(!nested.ptr) {
										_unmangleGCC3_abandon("_unmangleGCC3_nestedNameImpl", __FILE__, __LINE__);
										return NULL;
									}
									if(sbegin != substitution.end) {
										_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
												"Recursion did not consume the entire expansion of the "
												"substitution ('" + string(substitution.begin,
												static_cast<string::size_type>(substitution.end
												- substitution.begin)) + "')",
												sbegin, substitution.end, __FILE__, __LINE__);
										return NULL;
									}
								}
								break;
							case GCC3UMSR_STD:
								stdTplName = NULL;
								stdIsTemplate = false;
								goto useStdTplName;
							case GCC3UMSR_ALLOCATOR:
								stdTplName = "allocator";
								stdIsTemplate = true;
								goto useStdTplName;
							case GCC3UMSR_BASIC_STRING:
								stdTplName = "basic_string";
								stdIsTemplate = true;
								goto useStdTplName;
							case GCC3UMSR_STRING:
								stdTplName = "string";
								stdIsTemplate = false;
								goto useStdTplName;
							case GCC3UMSR_ISTREAM:
								stdTplName = "istream";
								stdIsTemplate = false;
								goto useStdTplName;
							case GCC3UMSR_OSTREAM:
								stdTplName = "ostream";
								stdIsTemplate = false;
								goto useStdTplName;
							case GCC3UMSR_IOSTREAM:
								stdTplName = "iostream";
								stdIsTemplate = false;
							  useStdTplName:
								{
									UnmanglePtr<SourceName> sname(new SourceName("std"));
									UnmanglePtr<NestedName::Segment> segment(new NestedName::Segment(sname.ptr));
									sname.ptr = NULL;
									nested.ptr->addSegment(*segment.ptr);
									argumentSink = stdIsTemplate ? segment.ptr : NULL;
									segment.ptr = NULL;
									if(stdTplName) {
										sname.ptr = new SourceName(stdTplName);
										segment.ptr = new NestedName::Segment(sname.ptr);
										sname.ptr = NULL;
										nested.ptr->addSegment(*segment.ptr);
										argumentSink = stdIsTemplate ? segment.ptr : NULL;
										segment.ptr = NULL;
									}
								}
								break;
							default:
								_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl",
										"Unrecognized GCC3UnmangleSubstRef", begin, end, __FILE__, __LINE__);
								return NULL;
						}
					}
					frontIsSubstitution = true;
					break;
				case 'E':
					goto done;
			}
		}
	  done:
		if(!nested.ptr->hasSegments()) {
			_unmangleGCC3_fail("_unmangleGCC3_nestedNameImpl", "Nested name cannot be empty",
					begin, end, __FILE__, __LINE__);
			return NULL;
		}
		NestedName* nn = nested.ptr;
		nested.ptr = NULL;
		return nn;
	}

	NestedName* _unmangleGCC3_nestedName(const char*& begin, const char* end, SBox& sbox) {
		++begin;
		int qualifiers = 0;
		if(begin != end && *begin == 'r') {
			qualifiers |= CVQualifiedType::CVQ_RESTRICT;
			++begin;
		}
		if(begin != end && *begin == 'V') {
			qualifiers |= CVQualifiedType::CVQ_VOLATILE;
			++begin;
		}
		if(begin != end && *begin == 'K') {
			qualifiers |= CVQualifiedType::CVQ_CONST;
			++begin;
		}
		NestedName::Segment* argumentSink = NULL;
		UnmanglePtr<NestedName> nested(_unmangleGCC3_nestedNameImpl(begin, end, sbox,
				true, qualifiers, argumentSink));
		if(!nested.ptr) {
			_unmangleGCC3_abandon("_unmangleGCC3_nestedName", __FILE__, __LINE__);
			return NULL;
		}
		if(begin == end || *begin != 'E') {
			_unmangleGCC3_fail("_unmangleGCC3_nestedName", "Missing 'E' terminator after nested name",
					begin, end, __FILE__, __LINE__);
			return NULL;
		}
		++begin;
		NestedName* nn = nested.ptr;
		nested.ptr = NULL;
		return nn;
	}

	LocalName* _unmangleGCC3_localName(const char*& begin, const char* end, SBox& sbox) {
		if(++begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_localName", "Expected local name", begin, end, __FILE__, __LINE__);
			return NULL;
		}
		UnmanglePtr<CPPSymbol> function(_unmangleGCC3_encoding(begin, end, sbox));
		if(!function.ptr) {
			_unmangleGCC3_abandon("_unmangleGCC3_localName", __FILE__, __LINE__);
			return NULL;
		}
		if(begin == end || *begin != 'E') {
			_unmangleGCC3_fail("_unmangleGCC3_localName",
					"Missing 'E' terminator after function symbol", begin, end, __FILE__, __LINE__);
			return NULL;
		}
		if(++begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_localName",
					"Missing actual local name after function symbol", begin, end, __FILE__, __LINE__);
			return NULL;
		}
		UnmanglePtr<Name> name(NULL);
		if(*begin == 's')
			++begin;
		else {
			name.ptr = _unmangleGCC3_name(begin, end, sbox);
			if(!name.ptr) {
				_unmangleGCC3_abandon("_unmangleGCC3_localName", __FILE__, __LINE__);
				return NULL;
			}
		}
		unsigned discriminator;
		if(begin == end || *begin != '_')
			discriminator = 0u;
		else {
			if(++begin == end || *begin < '0' || *begin > '9') {
				_unmangleGCC3_fail("_unmangleGCC3_localName", "Missing discriminator after '_'",
						begin, end, __FILE__, __LINE__);
				return NULL;
			}
			if(!_unmangleGCC3_number<unsigned>(begin, end, discriminator)) {
				_unmangleGCC3_abandon("_unmangleGCC3_localName", __FILE__, __LINE__);
				return NULL;
			}
			++discriminator;
		}
		LocalName* ln = new LocalName(function.ptr, name.ptr, discriminator);
		function.ptr = NULL;
		name.ptr = NULL;
		return ln;
	}

	Name* _unmangleGCC3_name(const char*& begin, const char* end, SBox& sbox) {
		if(begin == end)
			return NULL;
		switch(*begin) {
			case 'N':   // nested-name
				return _unmangleGCC3_nestedName(begin, end, sbox);
			case 'S':   // unscoped-name/unscoped-template-name, substitution
				{
					const char* atS = begin;
					++begin;
					if(begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_name", "Missing substitution after 'S'",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					if(*begin == 't') {
						++begin;
						UnmanglePtr<UnqualifiedName> uqname(_unmangleGCC3_unqualifiedName(begin, end, sbox));
						if(!uqname.ptr) {
							_unmangleGCC3_abandon("_unmangleGCC3_name", __FILE__, __LINE__);
							return NULL;
						}
						UnmanglePtr<SourceName> sname(new SourceName("std"));
						UnmanglePtr<NestedName> nname(new NestedName(0));
						UnmanglePtr<NestedName::Segment> segment(new NestedName::Segment(sname.ptr));
						sname.ptr = NULL;
						nname.ptr->addSegment(*segment.ptr);
						segment.ptr = NULL;
						NestedName::Segment* tailSegment = new NestedName::Segment(uqname.ptr);
						segment.ptr = tailSegment;
						uqname.ptr = NULL;
						nname.ptr->addSegment(*tailSegment);
						segment.ptr = NULL;
						NestedName* nn = nname.ptr;
						if(begin == end || *begin != 'I') {
							// it's "just" an <unscoped-name>
							nname.ptr = NULL;
							return nn;
						}
						// it's an <unscoped-template-name>, followed by <template-args>
						sbox.add(GCC3UnmangleSubstitution(GCC3UMST_UNSCOPED_TEMPLATE_NAME, atS, begin),
								"_unmangleGCC3_name", __FILE__, __LINE__);
						if(++begin == end) {
							_unmangleGCC3_fail("_unmangleGCC3_name",
									"Missing actual template arguments after 'St...I'",
									begin, end, __FILE__, __LINE__);
							return NULL;
						}
						UnmanglePtr<TemplateArgument> arg(NULL);
						do {
							arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
							if(!arg.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_name", __FILE__, __LINE__);
								return NULL;
							}
							if(begin == end) {
								_unmangleGCC3_fail("_unmangleGCC3_name",
										"Missing 'E' teminator after 'St...I...'", begin, end, __FILE__, __LINE__);
								return NULL;
							}
							tailSegment->addArgument(*arg.ptr);
							arg.ptr = NULL;
						} while(*begin != 'E');
						++begin;
						nname.ptr = NULL;
						return nn;
					}
					else {
						// This must be an <unscoped-template-name>, since it is being substituted.
						// Ooooor so the documentation states, but GCC actually seems to substitute
						// unscoped names as well.
						GCC3UnmangleSubstRef ref;
						unsigned sid;
						if(!_unmangleGCC3_substitution(begin, end, ref, sid)) {
							_unmangleGCC3_abandon("_unmangleGCC3_name", __FILE__, __LINE__);
							return NULL;
						}
						// Since <template-args> follow, this substitution can only be correct
						// if it expands to a template, not a namespace or template instance.
						// Buuuut, due to the weirdness indicated above, <template-args> will
						// *not* necessarily follow, so this is not really true, after all.
						UnmanglePtr<NestedName> nname(new NestedName(0));
						const char* stdTplName;
						NestedName::Segment* tailSegment;
						switch(ref) {
							case GCC3UMSR_ID:
								{
									if(sid >= static_cast<unsigned>(sbox.size())) {
										stringstream ss;
										ss << sid;
										_unmangleGCC3_fail("_unmangleGCC3_name",
												"Substitution index out of bounds: " + ss.str(),
												begin, end, __FILE__, __LINE__);
										return NULL;
									}
									const GCC3UnmangleSubstitution& substitution = sbox[sid];
									// Despite the expansion not necessarily being an
									// <unscoped-template-name>, the only type that makes
									// sense here is still GCC3UMST_UNSCOPED_TEMPLATE_NAME.
									// I think.
									if(substitution.type != GCC3UMST_UNSCOPED_TEMPLATE_NAME) {
										stringstream ss;
										ss << static_cast<int>(substitution.type);
										_unmangleGCC3_fail("_unmangleGCC3_name",
												"Rejected substitution type " + ss.str(),
												begin, end, __FILE__, __LINE__);
										return NULL;
									}
									const char* sbegin = substitution.begin;
									// As per what is being substituted,
									// the expansion must be an <unscoped-name>.
									UnmanglePtr<NestedName::Segment> segment(NULL);
									if(*sbegin == 'S') {
										if(++sbegin == substitution.end) {
											_unmangleGCC3_fail("_unmangleGCC3_name",
													"Missing substitution after 'S' in expansion of substitution",
													sbegin, substitution.end, __FILE__, __LINE__);
											return NULL;
										}
										if(*sbegin != 't') {
											_unmangleGCC3_fail("_unmangleGCC3_name",
													"An 'S' in the expansion of a substitution must be followed "
													"by 't'", sbegin, substitution.end, __FILE__, __LINE__);
											return NULL;
										}
										++sbegin;
										UnmanglePtr<SourceName> sname(new SourceName("std"));
										segment.ptr = new NestedName::Segment(sname.ptr);
										sname.ptr = NULL;
										nname.ptr->addSegment(*segment.ptr);
										segment.ptr = NULL;
									}
									SBox tmpSBox(sbox);
									UnmanglePtr<UnqualifiedName> uqname(_unmangleGCC3_unqualifiedName(sbegin,
											substitution.end, tmpSBox));
									if(!uqname.ptr) {
										_unmangleGCC3_abandon("_unmangleGCC3_name", __FILE__, __LINE__);
										return NULL;
									}
									if(sbegin != substitution.end) {
										_unmangleGCC3_fail("_unmangleGCC3_name",
												"Calling _unmangleGCC3_unqualifiedName() did not consume the "
												"entire expansion of the substitution ('"
												+ string(substitution.begin,
												static_cast<string::size_type>(substitution.end
												- substitution.begin)) + "')",
												sbegin, substitution.end, __FILE__, __LINE__);
										return NULL;
									}
									tailSegment = new NestedName::Segment(uqname.ptr);
									segment.ptr = tailSegment;
									uqname.ptr = NULL;
									nname.ptr->addSegment(*tailSegment);
									segment.ptr = NULL;
								}
								break;
							case GCC3UMSR_ALLOCATOR:
								stdTplName = "allocator";
								goto useStdTplName;
							case GCC3UMSR_BASIC_STRING:
								stdTplName = "basic_string";
							  useStdTplName:
								{
									UnmanglePtr<SourceName> sname(new SourceName("std"));
									UnmanglePtr<NestedName::Segment> segment(new NestedName::Segment(sname.ptr));
									sname.ptr = NULL;
									nname.ptr->addSegment(*segment.ptr);
									segment.ptr = NULL;
									sname.ptr = new SourceName(stdTplName);
									tailSegment = new NestedName::Segment(sname.ptr);
									segment.ptr = tailSegment;
									sname.ptr = NULL;
									nname.ptr->addSegment(*tailSegment);
									segment.ptr = NULL;
								}
								break;
							// Template instance names should be in error,
							// as per the following <template-args>, but
							// since this doesn't seem to be the case in
							// practice, we need to grok them as well.
							case GCC3UMSR_STRING:
								stdTplName = "string";
								goto useStdClassName;
							case GCC3UMSR_ISTREAM:
								stdTplName = "istream";
								goto useStdClassName;
							case GCC3UMSR_OSTREAM:
								stdTplName = "ostream";
								goto useStdClassName;
							case GCC3UMSR_IOSTREAM:
								stdTplName = "iostream";
							  useStdClassName:
								{
									UnmanglePtr<SourceName> sname(new SourceName("std"));
									UnmanglePtr<NestedName::Segment> segment(new NestedName::Segment(sname.ptr));
									sname.ptr = NULL;
									nname.ptr->addSegment(*segment.ptr);
									segment.ptr = NULL;
									sname.ptr = new SourceName(stdTplName);
									segment.ptr = new NestedName::Segment(sname.ptr);
									sname.ptr = NULL;
									nname.ptr->addSegment(*segment.ptr);
									segment.ptr = NULL;
									tailSegment = NULL;
								}
								break;
							case GCC3UMSR_STD:
							default:
								{
									stringstream ss;
									ss << static_cast<int>(ref);
									_unmangleGCC3_fail("_unmangleGCC3_name",
											"Rejected GCC3UnmangleSubstRef: " + ss.str(),
											begin, end, __FILE__, __LINE__);
								}
								return NULL;
						}
						NestedName* nn;
						if(begin == end || *begin != 'I') {
							// No following <template-args> should be an error,
							// but GCC seems to think otherwise, as per the
							// note above.
							/*
							_unmangleGCC3_fail("_unmangleGCC3_name",
									"Missing 'I' after substitution of unscoped template name",
									begin, end, __FILE__, __LINE__);
							return NULL;
							*/
							nn = nname.ptr;
							nname.ptr = NULL;
							return nn;
						}
						if(++begin == end) {
							_unmangleGCC3_fail("_unmangleGCC3_name",
									"Missing actual arguments after 'S...I'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
						UnmanglePtr<TemplateArgument> arg(NULL);
						do {
							arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
							if(!arg.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_name", __FILE__, __LINE__);
								return NULL;
							}
							if(begin == end) {
								_unmangleGCC3_fail("_unmangleGCC3_name",
										"Missing 'E' terminator after 'S...I...'", begin, end, __FILE__, __LINE__);
								return NULL;
							}
							tailSegment->addArgument(*arg.ptr);
							arg.ptr = NULL;
						} while(*begin != 'E');
						++begin;
						nn = nname.ptr;
						nname.ptr = NULL;
						return nn;
					}
				}
			case 'n':   // operator-name
			case 'd':   // operator-name
			case 'p':   // operator-name
			case 'a':   // operator-name
			case 'c':   // operator-name
			case 'm':   // operator-name
			case 'r':   // operator-name
			case 'o':   // operator-name
			case 'e':   // operator-name
			case 'l':   // operator-name
			case 'g':   // operator-name
			case 'i':   // operator-name
			case 'q':   // operator-name
			case 's':   // operator-name
			case 'v':   // operator-name
			case 'C':   // ctor-dtor-name
			case 'D':   // ctor-dtor-name
				goto plainUnqual;
			case 'Z':   // local-name
				return _unmangleGCC3_localName(begin, end, sbox);
			default:
				if(*begin < '0' || *begin > '9') {
					_unmangleGCC3_fail("_unmangleGCC3_name", "Unrecognized start of name",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
			  plainUnqual:
				{
					UnmanglePtr<UnqualifiedName> uqname(_unmangleGCC3_unqualifiedName(begin, end, sbox));
					if(!uqname.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_name", __FILE__, __LINE__);
						return NULL;
					}
					if(begin == end || *begin != 'I') {
						UnqualifiedName* uq = uqname.ptr;
						uqname.ptr = NULL;
						return uq;
					}
					if(++begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_name",
								"Missing actual template arguments after 'I'", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					UnmanglePtr<NestedName> nname(new NestedName(0));
					NestedName::Segment* tailSegment = new NestedName::Segment(uqname.ptr);
					UnmanglePtr<NestedName::Segment> segment(tailSegment);
					uqname.ptr = NULL;
					nname.ptr->addSegment(*tailSegment);
					segment.ptr = NULL;
					UnmanglePtr<TemplateArgument> arg(NULL);
					do {
						arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
						if(!arg.ptr) {
							_unmangleGCC3_abandon("_unmangleGCC3_name", __FILE__, __LINE__);
							return NULL;
						}
						if(begin == end) {
							_unmangleGCC3_fail("_unmangleGCC3_name",
									"Missing 'E' terminator after 'I...'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
						tailSegment->addArgument(*arg.ptr);
						arg.ptr = NULL;
					} while(*begin != 'E');
					++begin;
					NestedName* nn = nname.ptr;
					nname.ptr = NULL;
					return nn;
				}
		}
	}

	Type* _unmangleGCC3_type(const char*& begin, const char* end, SBox& sbox) {
		// See _unmangleGCC3_startsType() for details of
		// semantically invalid cases.
		if(begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_type", "Expected type", begin, end, __FILE__, __LINE__);
			return NULL;
		}
		const char* beforeThisType = begin;
		UnmanglePtr<Type> type(NULL);
		int qualifiers = 0;
		ModifiedType::Modifier modifier;
		bool noMemorize = false;
		switch(*begin) {
			case 'v':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_VOID);
			case 'w':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_WCHAR_T);
			case 'b':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_BOOL);
			case 'c':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_CHAR);
			case 'a':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_SIGNED_CHAR);
			case 'h':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_UNSIGNED_CHAR);
			case 's':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_SHORT);
			case 't':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_UNSIGNED_SHORT);
			case 'i':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_INT);
			case 'j':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_UNSIGNED_INT);
			case 'l':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_LONG);
			case 'm':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_UNSIGNED_LONG);
			case 'x':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_LONG_LONG);
			case 'y':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_UNSIGNED_LONG_LONG);
			case 'n':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_INT128);
			case 'o':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_UNSIGNED_INT128);
			case 'f':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_FLOAT);
			case 'd':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_DOUBLE);
			case 'e':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_LONG_DOUBLE);
			case 'g':   // builtin-type, [operator-name]
				++begin;
				return new BuiltinType(BuiltinType::P_FLOAT128);
			case 'z':   // builtin-type
				++begin;
				return new BuiltinType(BuiltinType::P_ELLIPSIS);
			case 'u':   // builtin-type
				{
					++begin;
					string name;
					if(!_unmangleGCC3_sourceName(begin, end, name)) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					type.ptr = new VendorExtendedType(name);
				}
				break;
			case 'F':   // function-type
				{
					if(++begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_type",
								"Missing actual function type after 'F'", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					bool externC;
					if(*begin == 'Y') {
						externC = true;
						++begin;
					}
					else
						externC = false;
					UnmanglePtr<BareFunctionType> bare(_unmangleGCC3_bareFunctionType(begin, end, sbox));
					if(!bare.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					if(begin == end || *begin != 'E') {
						_unmangleGCC3_fail("_unmangleGCC3_type", "Missing 'E' terminator after 'F...'",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					++begin;
					type.ptr = new FunctionType(externC, bare.ptr);
					bare.ptr = NULL;
				}
				break;
			case 'A':   // array-type
				{
					UnmanglePtr<Type> elementType(NULL);
					if(++begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_type", "Missing actual array type after 'A'",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					if(*begin == '_') {
						++begin;
						elementType.ptr = _unmangleGCC3_type(begin, end, sbox);
						if(!elementType.ptr) {
							_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
							return NULL;
						}
						type.ptr = new VariableArrayType(elementType.ptr);
						elementType.ptr = NULL;
					}
					else if(*begin >= '0' && *begin <= '9') {
						unsigned dimension;
						if(!_unmangleGCC3_number(begin, end, dimension)) {
							_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
							return NULL;
						}
						if(begin == end || *begin != '_') {
							_unmangleGCC3_fail("_unmangleGCC3_type",
									"Missing '_' terminator after static array type dimension",
									begin, end, __FILE__, __LINE__);
							return NULL;
						}
						++begin;
						elementType.ptr = _unmangleGCC3_type(begin, end, sbox);
						if(!elementType.ptr) {
							_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
							return NULL;
						}
						type.ptr = new StaticArrayType(elementType.ptr, dimension);
						elementType.ptr = NULL;
					}
					else {
						UnmanglePtr<Expression> dimension(_unmangleGCC3_expression(begin, end, sbox));
						if(!dimension.ptr) {
							_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
							return NULL;
						}
						if(begin == end || *begin != '_') {
							_unmangleGCC3_fail("_unmangleGCC3_type",
									"Missing '_' terminator after dynamic array type dimension",
									begin, end, __FILE__, __LINE__);
							return NULL;
						}
						++begin;
						elementType.ptr = _unmangleGCC3_type(begin, end, sbox);
						if(!elementType.ptr) {
							_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
							return NULL;
						}
						type.ptr = new ComplexArrayType(elementType.ptr, dimension.ptr);
						elementType.ptr = NULL;
						dimension.ptr = NULL;
					}
				}
				break;
			case 'M':   // pointer-to-member-type
				{
					++begin;
					UnmanglePtr<Type> classType(_unmangleGCC3_type(begin, end, sbox));
					if(!classType.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					UnmanglePtr<Type> memberType(_unmangleGCC3_type(begin, end, sbox));
					if(!memberType.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					type.ptr = new PointerToMemberType(classType.ptr, memberType.ptr);
					classType.ptr = memberType.ptr = NULL;
				}
				break;
			case 'r':   // [operator-name], CV-qualifiers
				qualifiers = CVQualifiedType::CVQ_RESTRICT;
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_type",
							"Missing subject type after 'r' qualifier", begin, end, __FILE__, __LINE__);
					return NULL;
				}
				if(*begin == 'K')
					goto atConstQual;
				if(*begin != 'V')
					goto flushCVQual;
			case 'V':   // CV-qualifiers
				qualifiers |= CVQualifiedType::CVQ_VOLATILE;
				if(++begin == end) {
					_unmangleGCC3_fail("_unmangleGCC3_type",
							"Missing subject type after 'V' qualifier", begin, end, __FILE__, __LINE__);
					return NULL;
				}
				if(*begin != 'K')
					goto flushCVQual;
			case 'K':   // CV-qualifiers
			  atConstQual:
				qualifiers |= CVQualifiedType::CVQ_CONST;
				++begin;
			  flushCVQual:
				{
					UnmanglePtr<Type> qtype(_unmangleGCC3_type(begin, end, sbox));
					if(!qtype.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					type.ptr = new CVQualifiedType(qualifiers, qtype.ptr);
					qtype.ptr = NULL;
				}
				break;
			case 'P':   // pointer-to
				modifier = ModifiedType::MOD_POINTER;
				goto flushModifier;
			case 'R':   // reference-to
				modifier = ModifiedType::MOD_REFERENCE;
				goto flushModifier;
			case 'C':   // [ctor-dtor-name], complex pair (C 2000)
				modifier = ModifiedType::MOD_COMPLEX;
				goto flushModifier;
			case 'G':   // imaginary (C 2000)
				modifier = ModifiedType::MOD_IMAGINARY;
			  flushModifier:
				{
					++begin;
					UnmanglePtr<Type> mtype(_unmangleGCC3_type(begin, end, sbox));
					if(!mtype.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					type.ptr = new ModifiedType(modifier, mtype.ptr);
					mtype.ptr = NULL;
				}
				break;
			case 'S':   // unscoped-name/unscoped-template-name, substitution
				{
					const char* beforeSubst = begin;
					++begin;
					GCC3UnmangleSubstRef ref;
					unsigned id;
					if(!_unmangleGCC3_substitution(begin, end, ref, id)) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					switch(ref) {
						case GCC3UMSR_ID:
							{
								if(id >= static_cast<unsigned>(sbox.size())) {
									stringstream ss;
									ss << id;
									_unmangleGCC3_fail("_unmangleGCC3_type",
											"Substitution index out of bounds: " + ss.str(),
											begin, end, __FILE__, __LINE__);
									return NULL;
								}
								const GCC3UnmangleSubstitution& substitution = sbox[id];
								const char* sbegin = substitution.begin;
								SBox tmpSBox(sbox);
								switch(substitution.type) {
									case GCC3UMST_TYPE:
										{
											Type* stype = _unmangleGCC3_type(sbegin, substitution.end, tmpSBox);
											if(!stype) {
												_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
												return NULL;
											}
											if(sbegin != substitution.end) {
												_unmangleGCC3_fail("_unmangleGCC3_type",
														"Calling _unmangleGCC3_type() did not consume the entire "
														"expansion of the substitution ('"
														+ string(substitution.begin,
														static_cast<string::size_type>(substitution.end
														- substitution.begin)) + "')",
														sbegin, substitution.end, __FILE__, __LINE__);
												delete stype;
												return NULL;
											}
											return stype;
										}
									case GCC3UMST_UNSCOPED_TEMPLATE_NAME:
										// see other cases of the enclosing switch
										{
											begin = beforeSubst;
											UnmanglePtr<Name> etname(_unmangleGCC3_name(begin, end, sbox));
											if(!etname.ptr) {
												_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
												return NULL;
											}
											type.ptr = new EnumType(etname.ptr);
											etname.ptr = NULL;
										}
										noMemorize = true;
										break;
									case GCC3UMST_TEMPLATE_TEMPLATE_PARAM:
										{
											if(*sbegin != 'T') {
												_unmangleGCC3_fail("_unmangleGCC3_type",
														"Expansion of substitution does not start with 'T', "
														"despite having type GCC3UMST_TEMPLATE_TEMPLATE_PARAM",
														sbegin, substitution.end, __FILE__, __LINE__);
												return NULL;
											}
											if(++sbegin == substitution.end) {
												_unmangleGCC3_fail("_unmangleGCC3_type",
														"Expansion of substitution ends after initial 'T'",
														sbegin, substitution.end, __FILE__, __LINE__);
												return NULL;
											}
											unsigned pindex;
											if(*sbegin == '_')
												pindex = 0u;
											else if(*sbegin >= '0' && *sbegin <= '9') {
												if(!_unmangleGCC3_number<unsigned>(sbegin,
														substitution.end, pindex)) {
													_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
													return NULL;
												}
												++pindex;
												if(sbegin == substitution.end || *sbegin != '_') {
													_unmangleGCC3_fail("_unmangleGCC3_type",
															"Missing '_' terminator after 'T...' in expansion "
															"of substitution",
															sbegin, substitution.end, __FILE__, __LINE__);
													return NULL;
												}
											}
											else {
												_unmangleGCC3_fail("_unmangleGCC3_type",
														"Invalid input after 'T' in expansion of substitution",
														sbegin, substitution.end, __FILE__, __LINE__);
												return NULL;
											}
											if(++sbegin != substitution.end) {
												_unmangleGCC3_fail("_unmangleGCC3_type",
														"Reading template argument did not consume the entire "
														"expansion of the substitution ('"
														+ string(substitution.begin,
														static_cast<string::size_type>(substitution.end
														- substitution.begin)) + "')",
														sbegin, substitution.end, __FILE__, __LINE__);
												return NULL;
											}
											if(begin == end || *begin != 'I') {
												_unmangleGCC3_fail("_unmangleGCC3_type",
														"Missing 'I' after expansion of a "
														"GCC3UMST_TEMPLATE_TEMPLATE_PARAM",
														begin, end, __FILE__, __LINE__);
												return NULL;
											}
											if(++begin == end) {
												_unmangleGCC3_fail("_unmangleGCC3_type",
														"Missing actual arguments after 'S...I'",
														begin, end, __FILE__, __LINE__);
												return NULL;
											}
											TemplateTemplateParamType* ttpt = new TemplateTemplateParamType(pindex);
											type.ptr = ttpt;
											UnmanglePtr<TemplateArgument> arg(NULL);
											do {
												arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
												if(!arg.ptr) {
													_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
													return NULL;
												}
												if(begin == end) {
													_unmangleGCC3_fail("_unmangleGCC3_type",
															"Missing 'E' terminator after 'S...I'",
															begin, end, __FILE__, __LINE__);
													return NULL;
												}
												ttpt->addArgument(*arg.ptr);
												arg.ptr = NULL;
											} while(*begin != 'E');
											++begin;
										}
										break;
									case GCC3UMST_NESTED_PREFIX:
										// GCC seems to use this one as well...
										{
											NestedName::Segment* argumentSink = NULL;
											UnmanglePtr<NestedName> nested(_unmangleGCC3_nestedNameImpl(sbegin,
													substitution.end, tmpSBox, false, 0, argumentSink));
											if(!nested.ptr) {
												_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
												return NULL;
											}
											if(sbegin != substitution.end) {
												_unmangleGCC3_fail("_unmangleGCC3_type",
														"Calling _unmangleGCC3_nestedNameImpl() did not consume "
														"the entire expansion of the substitution ('"
														+ string(substitution.begin,
														static_cast<string::size_type>(substitution.end
														- substitution.begin)) + "')",
														sbegin, substitution.end, __FILE__, __LINE__);
												return NULL;
											}
											EnumType* et = new EnumType(nested.ptr);
											nested.ptr = NULL;
											return et;
										}
									default:
										{
											stringstream ss;
											ss << static_cast<int>(substitution.type);
											_unmangleGCC3_fail("_unmangleGCC3_type",
													"Rejected substitution type " + ss.str(),
													begin, end, __FILE__, __LINE__);
										}
										return NULL;
								}
							}
							break;
						case GCC3UMSR_STD:
						case GCC3UMSR_ALLOCATOR:
						case GCC3UMSR_BASIC_STRING:
						case GCC3UMSR_STRING:
						case GCC3UMSR_ISTREAM:
						case GCC3UMSR_OSTREAM:
						case GCC3UMSR_IOSTREAM:
							// In all of these cases, the <class-enum-type> option
							// *must* be what we need, so we'll just read a name
							// and let the <name> rule perform the actual substitution.
							// Also note that we can just let this fall through
							// to the epilogue of this function such that this
							// type will become eligible for substitution in and of
							// itsself without fear of adding the same mangled
							// substring to the s-box twice, since the <name> rule
							// cannot derive <substitution> by itsself.
							{
								begin = beforeSubst;
								UnmanglePtr<Name> etname(_unmangleGCC3_name(begin, end, sbox));
								if(!etname.ptr) {
									_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
									return NULL;
								}
								type.ptr = new EnumType(etname.ptr);
								etname.ptr = NULL;
							}
							noMemorize = true;
							break;
						default:
							_unmangleGCC3_fail("_unmangleGCC3_type", "Unrecognized GCC3UnmangleSubstRef",
									begin, end, __FILE__, __LINE__);
							return NULL;
					}
				}
				break;
			case 'N':   // nested-name
			//case 'q': // [operator-name]
			//case 'D': // [ctor-dtor-name]
			case 'Z':   // local-name
				goto useEnumName;
			case 'T':   // template-param, template-template-param
				{
					if(++begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_type", "Missing '_' terminator after 'T'",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					const char* oldBegin = begin;
					unsigned pindex;
					if(*begin == '_')
						pindex = 0u;
					else if(*begin >= '0' && *begin <= '9') {
						if(!_unmangleGCC3_number<unsigned>(begin, end, pindex)) {
							_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
							return NULL;
						}
						++pindex;
						if(begin == end || *begin != '_') {
							_unmangleGCC3_fail("_unmangleGCC3_type",
									"Missing '_' terminator after 'T...'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
					}
					else {
						_unmangleGCC3_fail("_unmangleGCC3_type", "Invalid input after 'T'",
								begin, end, __FILE__, __LINE__);
						return NULL;
					}
					++begin;
					if(begin != end && *begin == 'I') {
						// <type> ::= <template-template-param> <template-args>
						sbox.add(GCC3UnmangleSubstitution(GCC3UMST_TEMPLATE_TEMPLATE_PARAM, oldBegin, begin),
								"_unmangleGCC3_type", __FILE__, __LINE__);
						if(++begin == end) {
							_unmangleGCC3_fail("_unmangleGCC3_type",
									"Missing actual arguments after 'T...I'", begin, end, __FILE__, __LINE__);
							return NULL;
						}
						TemplateTemplateParamType* ttpt = new TemplateTemplateParamType(pindex);
						type.ptr = ttpt;
						UnmanglePtr<TemplateArgument> arg(NULL);
						do {
							arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
							if(!arg.ptr) {
								_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
								return NULL;
							}
							if(begin == end) {
								_unmangleGCC3_fail("_unmangleGCC3_type",
										"Missing 'E' terminator after 'T...I...'", begin, end, __FILE__, __LINE__);
								return NULL;
							}
							ttpt->addArgument(*arg.ptr);
							arg.ptr = NULL;
						} while(*begin != 'E');
						++begin;
					}
					else
						// <type> ::= <template-param>
						type.ptr = new TemplateParamType(pindex);
				}
				break;
			case 'U':   // vendor extended type qualifier
				{
					++begin;
					string veqname;
					if(!_unmangleGCC3_sourceName(begin, end, veqname)) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					UnmanglePtr<Type> qtype(_unmangleGCC3_type(begin, end, sbox));
					if(!qtype.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					type.ptr = new VendorExtendedQualifiedType(veqname, qtype.ptr);
					qtype.ptr = NULL;
				}
				break;
			default:
				if(*begin < '0' || *begin > '9') {   // !source-name
					_unmangleGCC3_fail("_unmangleGCC3_type", "Unrecognized start of type",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
			  useEnumName:
				{
					UnmanglePtr<Name> etname(_unmangleGCC3_name(begin, end, sbox));
					if(!etname.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_type", __FILE__, __LINE__);
						return NULL;
					}
					type.ptr = new EnumType(etname.ptr);
					etname.ptr = NULL;
				}
				break;
		}
		if(!noMemorize)
			sbox.add(GCC3UnmangleSubstitution(GCC3UMST_TYPE, beforeThisType, begin),
					"_unmangleGCC3_type", __FILE__, __LINE__);
		Type* resultType = type.ptr;
		type.ptr = NULL;
		return resultType;
	}

	bool _unmangleGCC3_startsType(char c) {
		// The grammar allows a <type> to be a general <name>
		// since the former could be a <class-enum-type>. This
		// technically allows for some of rules to be drawn in
		// (operator-name, ctor-dtor-name) that, semantically,
		// cannot actually *be* a type. This function consequently
		// excludes these cases -- hence the rule names in
		// square brackets and the commented-out cases.
		switch(c) {
			case 'v':   // builtin-type, [operator-name]
			case 'w':   // builtin-type
			case 'b':   // builtin-type
			case 'c':   // builtin-type, [operator-name]
			case 'a':   // builtin-type, [operator-name]
			case 'h':   // builtin-type
			case 's':   // builtin-type, [operator-name]
			case 't':   // builtin-type
			case 'i':   // builtin-type, [operator-name]
			case 'j':   // builtin-type
			case 'l':   // builtin-type, [operator-name]
			case 'm':   // builtin-type, [operator-name]
			case 'x':   // builtin-type
			case 'y':   // builtin-type
			case 'n':   // builtin-type, [operator-name]
			case 'o':   // builtin-type, [operator-name]
			case 'f':   // builtin-type
			case 'd':   // builtin-type, [operator-name]
			case 'e':   // builtin-type, [operator-name]
			case 'g':   // builtin-type, [operator-name]
			case 'z':   // builtin-type
			case 'u':   // builtin-type
			case 'F':   // function-type
			case 'N':   // nested-name
			case 'S':   // unscoped-name/unscoped-template-name, substitution
			case 'r':   // [operator-name], CV-qualifiers
			//case 'q': // [operator-name]
			case 'C':   // [ctor-dtor-name], complex pair (C 2000)
			//case 'D': // [ctor-dtor-name]
			case 'Z':   // local-name
			case 'A':   // array-type
			case 'M':   // pointer-to-member-type
			case 'T':   // template-param, template-template-param
			case 'V':   // CV-qualifiers
			case 'K':   // CV-qualifiers
			case 'P':   // pointer-to
			case 'R':   // reference-to
			case 'G':   // imaginary (C 2000)
			case 'U':   // vendor extended type qualifier
				return true;
			default:
				return c >= '0' && c <= '9';   // source-name
		}
	}

	BareFunctionType* _unmangleGCC3_bareFunctionType(const char*& begin, const char* end, SBox& sbox) {
		UnmanglePtr<Type> type(_unmangleGCC3_type(begin, end, sbox));
		if(!type.ptr) {
			_unmangleGCC3_abandon("_unmangleGCC3_bareFunctionType", __FILE__, __LINE__);
			return NULL;
		}
		UnmanglePtr<BareFunctionType> bare(new BareFunctionType);
		bare.ptr->addType(*type.ptr);
		type.ptr = NULL;
		while(begin != end && _unmangleGCC3_startsType(*begin)) {
			type.ptr = _unmangleGCC3_type(begin, end, sbox);
			if(!type.ptr) {
				_unmangleGCC3_abandon("_unmangleGCC3_bareFunctionType", __FILE__, __LINE__);
				return NULL;
			}
			bare.ptr->addType(*type.ptr);
			type.ptr = NULL;
		}
		BareFunctionType* bft = bare.ptr;
		bare.ptr = NULL;
		return bft;
	}

	CallOffset* _unmangleGCC3_callOffset(const char*& begin, const char* end) {
		if(begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_callOffset", "Expected call offset", begin, end, __FILE__, __LINE__);
			return NULL;
		}
		switch(*begin) {
			case 'h':
				{
					int offset;
					++begin;
					if(!_unmangleGCC3_number<int>(begin, end, offset)) {
						_unmangleGCC3_abandon("_unmangleGCC3_callOffset", __FILE__, __LINE__);
						return NULL;
					}
					if(begin == end || *begin != '_') {
						_unmangleGCC3_fail("_unmangleGCC3_callOffset",
								"Missing '_' terminator after offset", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					++begin;
					return new CallOffset(offset);
				}
			case 'v':
				{
					int offset, virtualOffset;
					++begin;
					if(!_unmangleGCC3_number<int>(begin, end, offset)) {
						_unmangleGCC3_abandon("_unmangleGCC3_callOffset", __FILE__, __LINE__);
						return NULL;
					}
					if(begin == end || *begin != '_') {
						_unmangleGCC3_fail("_unmangleGCC3_callOffset",
								"Missing '_' separator after offset", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					++begin;
					if(!_unmangleGCC3_number<int>(begin, end, virtualOffset)) {
						_unmangleGCC3_abandon("_unmangleGCC3_callOffset", __FILE__, __LINE__);
						return NULL;
					}
					if(begin == end || *begin != '_') {
						_unmangleGCC3_fail("_unmangleGCC3_callOffset",
								"Missing '_' terminator after virtual offset", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					++begin;
					return new CallOffset(offset, virtualOffset);
				}
			default:
				_unmangleGCC3_fail("_unmangleGCC3_callOffset", "Unrecognized start of call offset",
						begin, end, __FILE__, __LINE__);
				return NULL;
		}
	}

	SpecialSymbol* _unmangleGCC3_specialName(const char*& begin, const char* end, SBox& sbox) {
		if(*begin == 'G') {
			if(++begin == end || *begin != 'V') {
				_unmangleGCC3_fail("_unmangleGCC3_specialName", "Missing 'V' after 'G'",
						begin, end, __FILE__, __LINE__);
				return NULL;
			}
			++begin;
			UnmanglePtr<Name> objectName(_unmangleGCC3_name(begin, end, sbox));
			if(!objectName.ptr) {
				_unmangleGCC3_abandon("_unmangleGCC3_specialName", __FILE__, __LINE__);
				return NULL;
			}
			GuardVariableSymbol* gv = new GuardVariableSymbol(objectName.ptr);
			objectName.ptr = NULL;
			return gv;
		}
		if(++begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_specialName", "Missing actual special name after 'T'",
					begin, end, __FILE__, __LINE__);
			return NULL;
		}
		TableSymbol::TableSymbolType tstype;
		switch(*begin) {
			case 'V':
				tstype = TableSymbol::TST_VIRTUAL_TABLE;
				break;
			case 'T':
				tstype = TableSymbol::TST_VTT_STRUCTURE;
				break;
			case 'I':
				tstype = TableSymbol::TST_TYPEINFO_STRUCTURE;
				break;
			case 'S':
				tstype = TableSymbol::TST_TYPEINFO_NAME;
				break;
			case 'h':
			case 'v':
				{
					UnmanglePtr<CallOffset> callOffset(_unmangleGCC3_callOffset(begin, end));
					if(!callOffset.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_specialName", __FILE__, __LINE__);
						return NULL;
					}
					if(begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_specialName",
								"Missing target function after call offset", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					UnmanglePtr<CPPSymbol> targetFunction(_unmangleGCC3_encoding(begin, end, sbox));
					OverrideThunkSymbol* thunk = new OverrideThunkSymbol(*callOffset.ptr, targetFunction.ptr);
					targetFunction.ptr = NULL;
					return thunk;
				}
			case 'c':
				{
					++begin;
					UnmanglePtr<CallOffset> thisAdjustment(_unmangleGCC3_callOffset(begin, end));
					if(!thisAdjustment.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_specialName", __FILE__, __LINE__);
						return NULL;
					}
					UnmanglePtr<CallOffset> resultAdjustment(_unmangleGCC3_callOffset(begin, end));
					if(!resultAdjustment.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_specialName", __FILE__, __LINE__);
						return NULL;
					}
					if(begin == end) {
						_unmangleGCC3_fail("_unmangleGCC3_specialName",
								"Missing target function after adjustments", begin, end, __FILE__, __LINE__);
						return NULL;
					}
					UnmanglePtr<CPPSymbol> targetFunction(_unmangleGCC3_encoding(begin, end, sbox));
					if(!targetFunction.ptr) {
						_unmangleGCC3_abandon("_unmangleGCC3_specialName", __FILE__, __LINE__);
						return NULL;
					}
					CovariantOverrideThunkSymbol* thunk = new CovariantOverrideThunkSymbol(*thisAdjustment.ptr,
							*resultAdjustment.ptr, targetFunction.ptr);
					targetFunction.ptr = NULL;
					return thunk;
				}
			default:
				_unmangleGCC3_fail("_unmangleGCC3_specialName", "Unrecognized start of special name",
						begin, end, __FILE__, __LINE__);
				return NULL;
		}
		if(++begin == end) {
			_unmangleGCC3_fail("_unmangleGCC3_specialName", "Missing target type after 'T[VTIS]'",
					begin, end, __FILE__, __LINE__);
			return NULL;
		}
		UnmanglePtr<Type> targetType(_unmangleGCC3_type(begin, end, sbox));
		if(!targetType.ptr) {
			_unmangleGCC3_abandon("_unmangleGCC3_specialName", __FILE__, __LINE__);
			return NULL;
		}
		TableSymbol* table = new TableSymbol(tstype, targetType.ptr);
		targetType.ptr = NULL;
		return table;
	}

	CPPSymbol* _unmangleGCC3_encoding(const char*& begin, const char* end, SBox& sbox) {
		// FIRST(encoding) = FIRST(name) + FIRST(special-name)
		// FIRST(name) = FIRST(nested-name) + FIRST(unscoped-name) + FIRST(unscoped-template-name) + FIRST(local-name)
		// FIRST(special-name) = 'TV' + 'TT' + 'TI' + 'TS' + 'GV' + 'Th' + 'Tv' + 'Tc'
		// FIRST(nested-name) = 'N'
		// FIRST(unscoped-name) = FIRST(unqualified-name) + 'St'
		// FIRST(unqualified-name) = FIRST(operator-name) + FIRST(ctor-dtor-name) + FIRST(source-name)
		// FIRST(source-name) = <digit>
		// FIRST(unscoped-template-name) = FIRST(unscoped-name) + FIRST(substitution)
		// FIRST(local-name) = 'Z'
		// FIRST(substitution) = 'S'
		switch(*begin) {
			case 'N':   // nested-name
			case 'S':   // unscoped-name/unscoped-template-name, substitution
			case 'n':   // operator-name
			case 'd':   // operator-name
			case 'p':   // operator-name
			case 'a':   // operator-name
			case 'c':   // operator-name
			case 'm':   // operator-name
			case 'r':   // operator-name
			case 'o':   // operator-name
			case 'e':   // operator-name
			case 'l':   // operator-name
			case 'g':   // operator-name
			case 'i':   // operator-name
			case 'q':   // operator-name
			case 's':   // operator-name
			case 'v':   // operator-name
			case 'C':   // ctor-dtor-name
			case 'D':   // ctor-dtor-name
			case 'Z':   // local-name
				break;
			case 'T':   // special-name
			case 'G':   // special-name
				return _unmangleGCC3_specialName(begin, end, sbox);
			default:
				if(*begin < '0' || *begin > '9') {   // !source-name
					_unmangleGCC3_fail("_unmangleGCC3_encoding", "Unrecognized start of encoding",
							begin, end, __FILE__, __LINE__);
					return NULL;
				}
				break;
		}
		UnmanglePtr<Name> name(_unmangleGCC3_name(begin, end, sbox));
		if(!name.ptr) {
			_unmangleGCC3_abandon("_unmangleGCC3_encoding", __FILE__, __LINE__);
			return NULL;
		}
		if(begin == end) {
			DataSymbol* data = new DataSymbol(name.ptr);
			name.ptr = NULL;
			return data;
		}
		UnmanglePtr<BareFunctionType> type(_unmangleGCC3_bareFunctionType(begin, end, sbox));
		if(!type.ptr) {
			_unmangleGCC3_abandon("_unmangleGCC3_encoding", __FILE__, __LINE__);
			return NULL;
		}
		FunctionSymbol* function = new FunctionSymbol(name.ptr, type.ptr);
		name.ptr = NULL;
		type.ptr = NULL;
		return function;
	}

	REDSTRAIN_REDMOND_API CPPSymbol* unmangleCPPSymbol_GCC3(const string& symbol) {
		if(shouldDebugUnmanglers())
			cerr << "***DEBUG: Will now unmangle: " << symbol << endl;
		const char* begin = symbol.data();
		const char* end = begin + symbol.length();
		if(begin == end || *begin != '_') {
			_unmangleGCC3_fail("unmangleCPPSymbol_GCC3", "Mangled name does not start with '_'",
					begin, end, __FILE__, __LINE__);
			return NULL;
		}
		if(++begin == end || *begin != 'Z') {
			_unmangleGCC3_fail("unmangleCPPSymbol_GCC3", "Mangled name does not start with '_Z'",
					begin, end, __FILE__, __LINE__);
			return NULL;
		}
		if(++begin == end) {
			_unmangleGCC3_fail("unmangleCPPSymbol_GCC3", "Missing encoding after '_Z'",
					begin, end, __FILE__, __LINE__);
			return NULL;
		}
		SBox sbox;
		CPPSymbol* sym = _unmangleGCC3_encoding(begin, end, sbox);
		if(begin != end) {
			if(sym) {
				delete sym;
				_unmangleGCC3_fail("unmangleCPPSymbol_GCC3",
						"Calling _unmangleGCC3_encoding() did not consume the entire input",
						begin, end, __FILE__, __LINE__);
			}
			return NULL;
		}
		return sym;
	}

}}
