#include <vector>

#include "unmangle-utils.hpp"
#include "unmangle/EnumType.hpp"
#include "unmangle/LocalName.hpp"
#include "unmangle/DataSymbol.hpp"
#include "unmangle/NestedName.hpp"
#include "unmangle/TableSymbol.hpp"
#include "unmangle/BuiltinType.hpp"
#include "unmangle/FunctionType.hpp"
#include "unmangle/ModifiedType.hpp"
#include "unmangle/FunctionSymbol.hpp"
#include "unmangle/CVQualifiedType.hpp"
#include "unmangle/StaticArrayType.hpp"
#include "unmangle/ComplexArrayType.hpp"
#include "unmangle/BareFunctionType.hpp"
#include "unmangle/VariableArrayType.hpp"
#include "unmangle/TemplateParamType.hpp"
#include "unmangle/VendorExtendedType.hpp"
#include "unmangle/GuardVariableSymbol.hpp"
#include "unmangle/OverrideThunkSymbol.hpp"
#include "unmangle/PointerToMemberType.hpp"
#include "unmangle/TemplateTemplateParamType.hpp"
#include "unmangle/VendorExtendedQualifiedType.hpp"
#include "unmangle/CovariantOverrideThunkSymbol.hpp"

using std::string;
using std::vector;
using redengine::redmond::unmangle::Name;
using redengine::redmond::unmangle::Type;
using redengine::redmond::unmangle::EnumType;
using redengine::redmond::unmangle::CPPSymbol;
using redengine::redmond::unmangle::LocalName;
using redengine::redmond::unmangle::DataSymbol;
using redengine::redmond::unmangle::NestedName;
using redengine::redmond::unmangle::Expression;
using redengine::redmond::unmangle::TableSymbol;
using redengine::redmond::unmangle::BuiltinType;
using redengine::redmond::unmangle::FunctionType;
using redengine::redmond::unmangle::ModifiedType;
using redengine::redmond::unmangle::SpecialSymbol;
using redengine::redmond::unmangle::UnqualifiedName;
using redengine::redmond::unmangle::CVQualifiedType;
using redengine::redmond::unmangle::StaticArrayType;
using redengine::redmond::unmangle::ComplexArrayType;
using redengine::redmond::unmangle::BareFunctionType;
using redengine::redmond::unmangle::TemplateArgument;
using redengine::redmond::unmangle::VariableArrayType;
using redengine::redmond::unmangle::TemplateParamType;
using redengine::redmond::unmangle::VendorExtendedType;
using redengine::redmond::unmangle::GuardVariableSymbol;
using redengine::redmond::unmangle::OverrideThunkSymbol;
using redengine::redmond::unmangle::PointerToMemberType;
using redengine::redmond::unmangle::TemplateTemplateParamType;
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
		GCC3UMST_TEMPLATE_TEMPLATE_PARAM
	};

	struct GCC3UnmangleSubstitution {

		GCC3UnmangleSubstType type;
		const char* begin;
		const char* end;

		GCC3UnmangleSubstitution(GCC3UnmangleSubstType type, const char* begin, const char* end)
				: type(type), begin(begin), end(end) {}

		GCC3UnmangleSubstitution(const GCC3UnmangleSubstType& substitution)
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

	typedef vector<GCC3UnmangleSubstitution> SBox;

	BareFunctionType* _unmangleGCC3_bareFunctionType(const char*&, const char*, SBox&);

	template<typename IntegerT>
	bool _unmangleGCC3_number(const char*& begin, const char* end, IntegerT& result) {
		if(begin == end)
			return false;
		bool negative;
		if(*begin == 'n') {
			negative = true;
			if(++begin == end)
				return false;
		}
		else
			negative = false;
		if(*begin < '0' || *begin > '9')
			return false;
		result = static_cast<IntegerT>(0);
		do {
			IntegerT digit = static_cast<IntegerT>(*begin - '0');
			if(negative)
				result = result * static_cast<IntegerT>(10) + digit;
			else
				result = result * static_cast<IntegerT>(10) - digit;
		} while(++begin != end && *begin >= '0' && *begin <= '9');
		return true;
	}

	bool _unmangleGCC3_sourceName(const char*& begin, const char* end, string& result) {
		if(begin == end || *begin < '0' || *begin > '9')
			return false;
		unsigned length;
		if(!_unmangleGCC3_number<unsigned>(begin, end, length))
			return false;
		const char* newBegin = begin + length;
		if(newBegin >= end)
			return false;
		result.assign(begin, static_cast<string::size_type>(length));
		begin = newBegin;
		return true;
	}

	bool _unmangleGCC3_substitution(const char*& begin, const char* end, GCC3UnmangleSubstRef& ref, unsigned& id) {
		if(begin == end)
			return false;
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
			id = static_cast<unsigned>(*begin - '0') + 10u;
		else
			return false;
		ref = GCC3UMSR_ID;
		for(;;) {
			if(++begin == end)
				return false;
			if(*begin == '_') {
				++begin;
				++id;
				return true;
			}
			if(*begin >= '0' && *begin <= '9')
				id = id * 36u + static_cast<unsigned>(*begin - '0');
			else if(*begin >= 'A' && *begin <= 'Z')
				id = id * 36u + static_cast<unsigned>(*begin - '0') + 10u;
			else
				return false;
		}
	}

	Expression* _unmangleGCC3_expression(const char*& begin, const char* end, SBox& sbox) {
		//TODO
	}

	TemplateArgument* _unmangleGCC3_templateArg(const char*& begin, const char* end, SBox& sbox) {
		// only called on non-empty input
		//TODO
	}

	NestedName* _unmangleGCC3_nestedName(const char*& begin, const char* end, SBox& sbox) {
		// only called on non-empty input
		//TODO
	}

	LocalName* _unmangleGCC3_localName(const char*& begin, const char* end, SBox& sbox) {
		// only called on non-empty input
		//TODO
	}

	UnqualifiedName* _unmangleGCC3_unqualifiedName(const char*& begin, const char* end, SBox& sbox) {
		if(begin == end)
			return NULL;
		//TODO
	}

	Name* _unmangleGCC3_name(const char*& begin, const char* end, SBox& sbox) {
		if(begin == end)
			return NULL;
		switch(*begin) {
			case 'N':   // nested-name
				return _unmangleGCC3_nestedName(begin, end, sbox);
			case 'S':   // unscoped-name/unscoped-template-name, substitution
				{
					const char* next = begin + 1;
					if(next == end)
						return NULL;
					if(*next == 't') {
						//TODO: call _unmangleGCC3_unqualifiedName() and put result into a NestedName
						//TODO: see if that is followed by <template-args> -- if so, make this
						//      <unscoped-template-name> eligible for substitution and read the args
					}
					else {
						//TODO: this must be an <unscoped-template-name>, since it is being substituted;
						//      also read the following <template-args>
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
				return _unmangleGCC3_unqualifiedName(begin, end, sbox);
			case 'Z':   // local-name
				return _unmangleGCC3_localName(begin, end, sbox);
			default:
				if(*begin >= '0' && *begin <= '9')
					return _unmangleGCC3_unqualifiedName(begin, end, sbox);
				return NULL;
		}
	}

	Type* _unmangleGCC3_type(const char*& begin, const char* end, SBox& sbox) {
		// See _unmangleGCC3_startsType() for details of
		// semantically invalid cases.
		if(begin == end)
			return NULL;
		const char* beforeThisType = begin;
		UnmanglePtr<Type> type(NULL);
		int qualifiers = 0;
		ModifiedType::Modifier modifier;
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
					if(!_unmangleGCC3_sourceName(begin, end, name))
						return NULL;
					type.ptr = new VendorExtendedType(name);
				}
				break;
			case 'F':   // function-type
				{
					if(++begin == end)
						return NULL;
					bool externC;
					if(*begin == 'Y') {
						externC = true;
						++begin;
					}
					else
						externC = false;
					UnmanglePtr<BareFunctionType> bare(_unmangleGCC3_bareFunctionType(begin, end, sbox));
					if(!bare.ptr)
						return NULL;
					if(begin == end || *begin != 'E')
						return NULL;
					++begin;
					type.ptr = new FunctionType(externC, bare.ptr);
					bare.ptr = NULL;
				}
				break;
			case 'A':   // array-type
				{
					UnmanglePtr<Type> elementType(NULL);
					if(++begin == end)
						return NULL;
					if(*begin == '_') {
						++begin;
						elementType.ptr = _unmangleGCC3_type(begin, end, sbox);
						if(!elementType.ptr)
							return NULL;
						type.ptr = new VariableArrayType(elementType.ptr);
						elementType.ptr = NULL;
					}
					else if(*begin >= '0' && *begin <= '9') {
						unsigned dimension;
						if(!_unmangleGCC3_number(begin, end, dimension))
							return NULL;
						if(begin == end || *begin != '_')
							return NULL;
						++begin;
						elementType.ptr = _unmangleGCC3_type(begin, end, sbox);
						if(!elementType.ptr)
							return NULL;
						type.ptr = new StaticArrayType(elementType.ptr, dimension);
						elementType.ptr = NULL;
					}
					else {
						UnmanglePtr<Expression> dimension(_unmangleGCC3_expression(begin, end, sbox));
						if(!dimension.ptr)
							return NULL;
						if(begin == end || *begin != '_')
							return NULL;
						++begin;
						elementType.ptr = _unmangleGCC3_type(begin, end, sbox);
						if(!elementType.ptr)
							return NULL;
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
					if(!classType.ptr)
						return NULL;
					UnmanglePtr<Type> memberType(_unmangleGCC3_type(begin, end, sbox));
					if(!memberType.ptr)
						return NULL;
					type.ptr = new PointerToMemberType(classType.ptr, memberType.ptr);
					classType.ptr = memberType.ptr = NULL;
				}
				break;
			case 'r':   // [operator-name], CV-qualifiers
				qualifiers = CVQualifiedType::CVQ_RESTRICT;
				if(++begin == end)
					return NULL;
				if(*begin == 'K')
					goto atConstQual;
				if(*begin != 'V')
					goto flushCVQual;
			case 'V':   // CV-qualifiers
				qualifiers |= CVQualifiedType::CVQ_VOLATILE;
				if(++begin == end)
					return NULL;
				if(*begin != 'K')
					goto flushCVQual;
			case 'K':   // CV-qualifiers
			  atConstQual:
				qualifiers |= CVQualifiedType::CVQ_CONST;
				++begin;
			  flushCVQual:
				{
					UnmanglePtr<Type> qtype(_unmangleGCC3_type(begin, end, sbox));
					if(!qtype.ptr)
						return NULL;
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
					if(!mtype.ptr)
						return NULL;
					type.ptr = new ModifiedType(modifier, mtype.ptr);
					mtype.ptr = NULL;
				}
				break;
			case 'S':   // unscoped-name/unscoped-template-name, substitution
				{
					++begin;
					const char* beforeSubst = begin;
					GCC3UnmangleSubstRef ref;
					unsigned id;
					if(!_unmangleGCC3_substitution(begin, end, ref, id))
						return NULL;
					switch(ref) {
						case GCC3UMSR_ID:
							{
								if(id >= static_cast<unsigned>(sbox.size()))
									return NULL;
								const GCC3UnmangleSubstitution& substitution = sbox[id];
								const char* sbegin = substitution.begin;
								SBox tmpSBox;
								switch(substitution.type) {
									case GCC3UMST_TYPE:
										return _unmangleGCC3_type(sbegin, substitution.end, tmpSBox);
									case GCC3UMST_UNSCOPED_TEMPLATE_NAME:
										// see other cases of the enclosing switch
										{
											begin = beforeSubst;
											UnmanglePtr<Name> etname(_unmangleGCC3_name(begin, end, sbox));
											if(!etname.ptr)
												return NULL;
											type.ptr = new EnumType(etname.ptr);
											etname.ptr = NULL;
										}
										break;
									case GCC3UMST_TEMPLATE_TEMPLATE_PARAM:
										{
											if(*sbegin != 'T')
												return NULL;
											if(++sbegin == substitution.end)
												return NULL;
											if(*sbegin == '_')
												pindex = 0u;
											else if(*sbegin >= '0' && *sbegin <= '9') {
												if(!_unmangleGCC3_number<unsigned>(sbegin, substitution.end, pindex))
													return NULL;
												++pindex;
												if(sbegin == substitution.end || *sbegin != '_')
													return NULL;
											}
											else
												return NULL;
											if(++sbegin != substitution.end)
												return NULL;
											if(begin == end || *begin != 'I')
												return NULL;
											if(++begin == end)
												return NULL;
											TemplateTemplateParamType* ttpt = new TemplateTemplateParamType(pindex);
											type.ptr = ttpt;
											UnmanglePtr<TemplateArgument> arg(NULL);
											do {
												arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
												if(!arg.ptr || begin == end)
													return NULL;
												ttpt->addArgument(arg.ptr);
												arg.ptr = NULL;
											} while(*begin != 'E');
											++begin;
										}
										break;
									default:
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
								UnmanglePtr<Name> etname(_unmangleGCC3_name(begin, end, sbox));
								if(!etname.ptr)
									return NULL;
								type.ptr = new EnumType(etname.ptr);
								etname.ptr = NULL;
							}
							break;
						default:
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
					if(++begin == end)
						return NULL;
					const char* oldBegin = begin;
					unsigned pindex;
					if(*begin == '_')
						pindex = 0u;
					else if(*begin >= '0' && *begin <= '9') {
						if(!_unmangleGCC3_number<unsigned>(begin, end, pindex))
							return NULL;
						++pindex;
						if(begin == end || *begin != '_')
							return NULL;
					}
					else
						return NULL;
					++begin;
					if(begin != end && *begin == 'I') {
						// <type> ::= <template-template-param> <template-args>
						sbox.push_back(GCC3UnmangleSubstitution(GCC3UMST_TEMPLATE_TEMPLATE_PARAM, oldBegin, begin));
						if(++begin == end)
							return NULL;
						TemplateTemplateParamType* ttpt = new TemplateTemplateParamType(pindex);
						type.ptr = ttpt;
						UnmanglePtr<TemplateArgument> arg(NULL);
						do {
							arg.ptr = _unmangleGCC3_templateArg(begin, end, sbox);
							if(!arg.ptr || begin == end)
								return NULL;
							ttpt->addArgument(arg.ptr);
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
					if(!_unmangleGCC3_sourceName(begin, end, veqname))
						return NULL;
					UnmanglePtr<Type> qtype(_unmangleGCC3_type(begin, end, sbox));
					if(!qtype.ptr)
						return NULL;
					type.ptr = new VendorExtendedQualifiedType(veqname, qtype.ptr);
					qtype.ptr = NULL;
				}
				break;
			default:
				if(c < '0' || c > '9')   // !source-name
					return NULL;
			  useEnumName:
				{
					UnmanglePtr<Name> etname(_unmangleGCC3_name(begin, end, sbox));
					if(!etname.ptr)
						return NULL;
					type.ptr = new EnumType(etname.ptr);
					etname.ptr = NULL;
				}
				break;
		}
		sbox.push_back(GCC3UnmangleSubstitution(GCC3UMST_TYPE, beforeThisType, begin));
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
		if(!type.ptr)
			return NULL;
		UnmanglePtr<BareFunctionType> bare(new BareFunctionType);
		bare.ptr->addType(*type.ptr);
		type.ptr = NULL;
		while(begin != end && _unmangleGCC3_startsType(*begin)) {
			type.ptr = _unmangleGCC3_type(begin, end, sbox);
			if(!type.ptr)
				return NULL;
			bare.ptr->addType(*type.ptr);
			type.ptr = NULL;
		}
		BareFunctionType* bft = bare.ptr;
		bare.ptr = NULL;
		return bft;
	}

	CallOffset* _unmangleGCC3_callOffset(const char*& begin, const char* end, SBox& sbox) {
		if(begin == end)
			return NULL;
		switch(*begin) {
			case 'h':
				{
					int offset;
					++begin;
					if(!_unmangleGCC3_number<int>(begin, end, offset))
						return NULL;
					if(begin == end || *begin != '_')
						return NULL;
					++begin;
					return new CallOffset(offset);
				}
			case 'v':
				{
					int offset, virtualOffset;
					++begin;
					if(!_unmangleGCC3_number<int>(begin, end, offset))
						return NULL;
					if(begin == end || *begin != '_')
						return NULL;
					++begin;
					if(!_unmangleGCC3_number<int>(begin, end, virtualOffset))
						return NULL;
					if(begin == end || *begin != '_')
						return NULL;
					++begin;
					return new CallOffset(offset, virtualOffset);
				}
			default:
				return NULL;
		}
	}

	SpecialSymbol* _unmangleGCC3_specialName(const char*& begin, const char* end, SBox& sbox) {
		if(*begin == 'G') {
			if(++begin == end || *begin != 'V')
				return NULL;
			++begin;
			UnmanglePtr<Name> objectName(_unmangleGCC3_name(begin, end, sbox);
			if(!objectName.ptr)
				return NULL;
			GuardVariableSymbol* gv = new GuardVariableSymbol(objectName.ptr);
			objectName.ptr = NULL;
			return gv;
		}
		if(++begin == end)
			return NULL;
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
					UnmanglePtr<CallOffset> callOffset(_unmangleGCC3_callOffset(begin, end, sbox));
					if(!callOffset.ptr || begin == end)
						return NULL;
					UnmanglePtr<CPPSymbol> targetFunction(_unmangleGCC3_encoding(begin, end, sbox));
					OverrideThunkSymbol* thunk = new OverrideThunkSymbol(*callOffset.ptr, targetFunction.ptr);
					targetFunction.ptr = NULL;
					return thunk;
				}
			case 'c':
				{
					++begin;
					UnmanglePtr<CallOffset> thisAdjustment(_unmangleGCC3_callOffset(begin, end, sbox));
					if(!thisAdjustment.ptr)
						return NULL;
					UnmanglePtr<CallOffset> resultAdjustment(_unmangleGCC3_callOffset(begin, end, sbox));
					if(!resultAdjustment.ptr || begin == end)
						return NULL;
					UnmanglePtr<CPPSymbol> targetFunction(_unmangleGCC3_encoding(begin, end, sbox));
					CovariantOverrideThunkSymbol* thunk = new CovariantOverrideThunkSymbol(*thisAdjustment.ptr,
							*resultAdjustment.ptr, targetFunction.ptr);
					targetFunction.ptr = NULL;
					return thunk;
				}
			default:
				return NULL;
		}
		if(++begin == end)
			return NULL;
		UnmanglePtr<Type> targetType(_unmangleGCC3_type(begin, end, sbox));
		if(!targetType.ptr)
			return NULL;
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
				if(*begin < '0' || *begin > '9')   // !source-name
					return NULL;
				break;
		}
		UnmanglePtr<Name> name(_unmangleGCC3_name(begin, end, sbox));
		if(!name.ptr)
			return NULL;
		if(begin == end) {
			DataSymbol* data = new DataSymbol(name.ptr);
			name.ptr = NULL;
			return data;
		}
		UnmanglePtr<BareFunctionType> type(_unmangleGCC3_bareFunctionType(begin, end, sbox));
		FunctionSymbol* function = new FunctionSymbol(name.ptr, type.ptr);
		name.ptr = NULL;
		type.ptr = NULL;
		return function;
	}

	REDSTRAIN_REDMOND_API CPPSymbol* unmangleCPPSymbol_GCC3(const string& symbol) {
		const char* begin = symbol.data();
		const char* end = begin + symbol.length();
		if(begin == end || *begin != '_')
			return NULL;
		if(++begin == end || *begin != 'Z')
			return NULL;
		if(++begin == end)
			return NULL;
		SBox sbox;
		CPPSymbol* symbol = _unmangleGCC3_encoding(begin, end, sbox);
		if(begin != end) {
			if(symbol)
				delete symbol;
			return NULL;
		}
		return symbol;
	}

}}
