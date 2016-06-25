#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLWIDTHADAPTER_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLWIDTHADAPTER_HPP

#include "UnboundedSymbolSink.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API SymbolWidthAdapter : public virtual SymbolSink {

	  private:
		class REDSTRAIN_REDMOND_API CountingSymbolSink : public UnboundedSymbolSink {

		  private:
			SymbolWidthAdapter& adapter;
			unsigned columnCount;

		  public:
			CountingSymbolSink(SymbolWidthAdapter&);
			CountingSymbolSink(const CountingSymbolSink&);

			inline unsigned getColumnCount() const {
				return columnCount;
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

	  protected:
		unsigned getInlineWidthOf(const CPPSymbol&);

		virtual unsigned getSeparatorWidth(Separator) = 0;
		virtual unsigned getReservedWidth(Reserved) = 0;
		virtual unsigned getReplacementStringWidth(ReplacementString) = 0;
		virtual unsigned getSourceNameWidth(const std::string&);
		virtual unsigned getIntLiteralWidth(int64_t);
		virtual unsigned getIntLiteralWidth(uint64_t);
		virtual unsigned getFloatLiteralWidth(float);
		virtual unsigned getFloatLiteralWidth(double);
		virtual unsigned getOverrideThunkWidth(const OverrideThunkSymbol&) = 0;
		virtual unsigned getCovariantOverrideThunkWidth(const CovariantOverrideThunkSymbol&) = 0;
		virtual unsigned getGuardVariableWidth(const GuardVariableSymbol&) = 0;
		virtual unsigned getTableWidth(const TableSymbol&) = 0;
		virtual unsigned getLocalNameDiscriminatorWidth(unsigned) = 0;
		virtual unsigned getOperatorNameWidth(const OperatorName&) = 0;
		virtual unsigned getUndefinedTemplateParameterWidth(unsigned);
		virtual unsigned getBuiltinTypeWidth(const BuiltinType&) = 0;
		virtual unsigned getVendorExtendedTypeQualifierWidth(const std::string&);
		virtual unsigned getVendorExtendedTypeWidth(const std::string&);
		virtual unsigned getOperatorSymbolWidth(const BinaryOperationExpression&) = 0;
		virtual unsigned getOperatorSymbolWidth(const UnaryOperationExpression&) = 0;

	  public:
		SymbolWidthAdapter();
		SymbolWidthAdapter(const SymbolWidthAdapter&);

		virtual unsigned getInlineWidthOf(const Type&, const CurrentTemplateArguments&, const Type*);
		virtual unsigned getInlineWidthOf(const Name&, const CurrentTemplateArguments&, const std::string*);
		virtual unsigned getInlineWidthOf(const TemplateArgument&, const CurrentTemplateArguments&);
		virtual unsigned getInlineWidthOf(Separator);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLWIDTHADAPTER_HPP */
