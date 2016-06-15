#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALEXPRESSION_HPP

#include "IntegerLiteralBase.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	template<typename IntegerT>
	class IntegerLiteralExpression : public IntegerLiteralBase {

	  private:
		static const bool SIGNED = static_cast<IntegerT>(-1) < static_cast<IntegerT>(0);

	  private:
		template<int, bool>
		struct StreamType {
			typedef int Printable;
		};

		template<int Dummy>
		struct StreamType<Dummy, false> {
			typedef unsigned Printable;
		};

	  private:
		BuiltinType::Primitive type;
		IntegerT value;

	  public:
		IntegerLiteralExpression(BuiltinType::Primitive type, IntegerT value) : type(type), value(value) {}

		IntegerLiteralExpression(const IntegerLiteralExpression& expression)
				: IntegerLiteralBase(expression), type(expression.type), value(expression.value) {}

		virtual BuiltinType::Primitive getIntegerType() const {
			return type;
		}

		virtual void print(std::ostream& out, int) const {
			typedef typename StreamType<0, SIGNED>::Printable Printable;
			switch(type) {
				case BuiltinType::P_CHAR:
				case BuiltinType::P_SIGNED_CHAR:
				case BuiltinType::P_UNSIGNED_CHAR:
				case BuiltinType::P_WCHAR_T:
					out << static_cast<Printable>(value);
					break;
				case BuiltinType::P_BOOL:
					out << (value ? "true" : "false");
					break;
				default:
					out << value;
					break;
			}
		}

		virtual Expression* cloneExpression() const {
			return new IntegerLiteralExpression(*this);
		}

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALEXPRESSION_HPP */
