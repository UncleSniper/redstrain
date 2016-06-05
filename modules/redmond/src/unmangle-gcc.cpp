#include "unmangle-utils.hpp"
#include "unmangle/CPPSymbol.hpp"

using std::string;
using redengine::redmond::unmangle::CPPSymbol;

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
	//                ::= Tc <call-offset> <call-offset> <base encoding>
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
	// <type> ::= <CV-qualifiers> <type>
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

	CPPSymbol* _unmangleGCC3_encoding(const char*& begin, const char* end) {
		//TODO
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
		CPPSymbol* symbol = _unmangleGCC3_encoding(begin, end);
		if(begin != end) {
			delete symbol;
			return NULL;
		}
		return symbol;
	}

}}
