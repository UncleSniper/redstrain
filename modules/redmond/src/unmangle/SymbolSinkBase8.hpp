#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINKBASE8_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINKBASE8_HPP

#include "SymbolWidthAdapter.hpp"
#include "HighlightingSymbolSink.hpp"
#include "SpecialSymbolMangleStyle.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API SymbolSinkBase8 : public virtual HighlightingSymbolSink, public SymbolWidthAdapter {

	  public:
		static const int FL_UNSIGNED_INT = 01;
		static const int FL_MASK         = 01;

	  private:
		int flags;
		SpecialSymbolMangleStyle specialStyle;

	  private:
		void putTableRS(const TableSymbol&);
		void putTableGCC3(const TableSymbol&);
		unsigned getTableWidthRS(const TableSymbol&);
		unsigned getTableWidthGCC3(const TableSymbol&);

	  protected:
		virtual void writeString(const std::string&) = 0;
		virtual unsigned getSeparatorWidth(Separator);
		virtual unsigned getReservedWidth(Reserved);
		virtual unsigned getReplacementStringWidth(ReplacementString);
		virtual unsigned getOverrideThunkWidth(const OverrideThunkSymbol&);
		virtual unsigned getCovariantOverrideThunkWidth(const CovariantOverrideThunkSymbol&);
		virtual unsigned getGuardVariableWidth(const GuardVariableSymbol&);
		virtual unsigned getTableWidth(const TableSymbol&);
		virtual unsigned getLocalNameDiscriminatorWidth(unsigned);
		virtual unsigned getOperatorNameWidth(const OperatorName&);
		virtual unsigned getBuiltinTypeWidth(const BuiltinType&);
		virtual unsigned getOperatorSymbolWidth(const BinaryOperationExpression&);
		virtual unsigned getOperatorSymbolWidth(const UnaryOperationExpression&);

	  public:
		SymbolSinkBase8(int = 0);
		SymbolSinkBase8(const SymbolSinkBase8&);

		inline int getFlags() const {
			return flags;
		}

		inline void setFlags(int flags) {
			this->flags = flags & SymbolSinkBase8::FL_MASK;
		}

		inline SpecialSymbolMangleStyle getSpecialSymbolMangleStyle() const {
			return specialStyle;
		}

		inline void setSpecialSymbolMangleStyle(SpecialSymbolMangleStyle style) {
			specialStyle = style;
		}

		virtual void putSeparator(Separator);
		virtual void putReserved(Reserved);
		virtual void putReplacementString(ReplacementString);
		virtual void putSourceName(const std::string&);
		virtual void putIntLiteral(int64_t);
		virtual void putIntLiteral(uint64_t);
		virtual void putFloatLiteral(float);
		virtual void putFloatLiteral(double);
		virtual void putOverrideThunk(const OverrideThunkSymbol&);
		virtual void putCovariantOverrideThunk(const CovariantOverrideThunkSymbol&);
		virtual void putGuardVariable(const GuardVariableSymbol&);
		virtual void putTable(const TableSymbol&);
		virtual void putLocalNameDiscriminator(unsigned);
		virtual void putOperatorName(const OperatorName&);
		virtual void putUndefinedTemplateParameter(unsigned);
		virtual void putBuiltinType(const BuiltinType&);
		virtual void putVendorExtendedTypeQualifier(const std::string&);
		virtual void putVendorExtendedType(const std::string&);
		virtual void putOperatorSymbol(const BinaryOperationExpression&);
		virtual void putOperatorSymbol(const UnaryOperationExpression&);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINKBASE8_HPP */
