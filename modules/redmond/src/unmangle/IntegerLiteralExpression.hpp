#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALEXPRESSION_HPP

#include "SymbolSink.hpp"
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
			typedef int64_t Printable;
		};

		template<int Dummy>
		struct StreamType<Dummy, false> {
			typedef uint64_t Printable;
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

		virtual void print(SymbolSink& sink, int, const CurrentTemplateArguments&) const {
			typedef typename StreamType<0, SIGNED>::Printable Printable;
			if(type == BuiltinType::P_BOOL)
				sink.putReserved(value ? SymbolSink::RSV_TRUE : SymbolSink::RSV_FALSE);
			else
				sink.putIntLiteral(static_cast<Printable>(value));
		}

		virtual Expression* cloneExpression() const {
			return new IntegerLiteralExpression(*this);
		}

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALEXPRESSION_HPP */
