#include <sstream>

#include "Type.hpp"
#include "Name.hpp"
#include "CPPSymbol.hpp"
#include "TemplateArgument.hpp"
#include "SymbolWidthAdapter.hpp"

using std::string;
using std::stringstream;

namespace redengine {
namespace redmond {
namespace unmangle {

	// ======== CountingSymbolSink ========

	SymbolWidthAdapter::CountingSymbolSink::CountingSymbolSink(SymbolWidthAdapter& adapter)
			: adapter(adapter), columnCount(0u) {}

	SymbolWidthAdapter::CountingSymbolSink::CountingSymbolSink(const CountingSymbolSink& sink)
			: SymbolSink(sink), UnboundedSymbolSink(sink), adapter(sink.adapter), columnCount(sink.columnCount) {}

	void SymbolWidthAdapter::CountingSymbolSink::putSeparator(Separator separator) {
		columnCount += adapter.getSeparatorWidth(separator);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putReserved(Reserved reserved) {
		columnCount += adapter.getReservedWidth(reserved);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putReplacementString(ReplacementString replacement) {
		columnCount += adapter.getReplacementStringWidth(replacement);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putSourceName(const string& segment) {
		columnCount += adapter.getSourceNameWidth(segment);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putIntLiteral(int64_t value) {
		columnCount += adapter.getIntLiteralWidth(value);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putIntLiteral(uint64_t value) {
		columnCount += adapter.getIntLiteralWidth(value);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putFloatLiteral(float value) {
		columnCount += adapter.getFloatLiteralWidth(value);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putFloatLiteral(double value) {
		columnCount += adapter.getFloatLiteralWidth(value);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putOverrideThunk(const OverrideThunkSymbol& symbol) {
		columnCount += adapter.getOverrideThunkWidth(symbol);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putCovariantOverrideThunk(const CovariantOverrideThunkSymbol&
			symbol) {
		columnCount += adapter.getCovariantOverrideThunkWidth(symbol);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putGuardVariable(const GuardVariableSymbol& symbol) {
		columnCount += adapter.getGuardVariableWidth(symbol);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putTable(const TableSymbol& symbol) {
		columnCount += adapter.getTableWidth(symbol);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putLocalNameDiscriminator(unsigned discriminator) {
		columnCount += adapter.getLocalNameDiscriminatorWidth(discriminator);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putOperatorName(const OperatorName& name) {
		columnCount += adapter.getOperatorNameWidth(name);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putUndefinedTemplateParameter(unsigned parameter) {
		columnCount += adapter.getUndefinedTemplateParameterWidth(parameter);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putBuiltinType(const BuiltinType& type) {
		columnCount += adapter.getBuiltinTypeWidth(type);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putVendorExtendedTypeQualifier(const string& qualifier) {
		columnCount += adapter.getVendorExtendedTypeQualifierWidth(qualifier);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putVendorExtendedType(const string& name) {
		columnCount += adapter.getVendorExtendedTypeWidth(name);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putOperatorSymbol(const BinaryOperationExpression& oper) {
		columnCount += adapter.getOperatorSymbolWidth(oper);
	}

	void SymbolWidthAdapter::CountingSymbolSink::putOperatorSymbol(const UnaryOperationExpression& oper) {
		columnCount += adapter.getOperatorSymbolWidth(oper);
	}

	// ======== SymbolWidthAdapter ========

	SymbolWidthAdapter::SymbolWidthAdapter() {}

	SymbolWidthAdapter::SymbolWidthAdapter(const SymbolWidthAdapter& sink) : SymbolSink(sink) {}

	unsigned SymbolWidthAdapter::getInlineWidthOf(const CPPSymbol& symbol) {
		CountingSymbolSink sink(*this);
		symbol.print(sink);
		return sink.getColumnCount();
	}

	unsigned SymbolWidthAdapter::getInlineWidthOf(const Name& name, const CurrentTemplateArguments& arguments,
			const string* className) {
		CountingSymbolSink sink(*this);
		name.print(sink, arguments, className);
		return sink.getColumnCount();
	}

	unsigned SymbolWidthAdapter::getSourceNameWidth(const string& segment) {
		return static_cast<unsigned>(segment.length());
	}

	unsigned SymbolWidthAdapter::getIntLiteralWidth(int64_t value) {
		stringstream ss;
		ss << value;
		return static_cast<unsigned>(ss.tellp());
	}

	unsigned SymbolWidthAdapter::getIntLiteralWidth(uint64_t value) {
		stringstream ss;
		ss << value;
		return static_cast<unsigned>(ss.tellp());
	}

	unsigned SymbolWidthAdapter::getFloatLiteralWidth(float value) {
		stringstream ss;
		ss << value;
		return static_cast<unsigned>(ss.tellp());
	}

	unsigned SymbolWidthAdapter::getFloatLiteralWidth(double value) {
		stringstream ss;
		ss << value;
		return static_cast<unsigned>(ss.tellp());
	}

	unsigned SymbolWidthAdapter::getUndefinedTemplateParameterWidth(unsigned parameter) {
		stringstream ss;
		ss << parameter;
		return 1u + static_cast<unsigned>(ss.tellp());
	}

	unsigned SymbolWidthAdapter::getVendorExtendedTypeQualifierWidth(const string& qualifier) {
		return static_cast<unsigned>(qualifier.length()) + 1u;
	}

	unsigned SymbolWidthAdapter::getVendorExtendedTypeWidth(const string& name) {
		return static_cast<unsigned>(name.length());
	}

	unsigned SymbolWidthAdapter::getInlineWidthOf(const Type& type, const CurrentTemplateArguments& arguments,
			const Type* enclosingClass) {
		CountingSymbolSink sink(*this);
		type.print(sink, arguments, enclosingClass);
		return sink.getColumnCount();
	}

	unsigned SymbolWidthAdapter::getInlineWidthOf(const TemplateArgument& argument,
			const CurrentTemplateArguments& arguments) {
		CountingSymbolSink sink(*this);
		argument.print(sink, arguments);
		return sink.getColumnCount();
	}

	unsigned SymbolWidthAdapter::getInlineWidthOf(Separator separator) {
		return getSeparatorWidth(separator);
	}

}}}
