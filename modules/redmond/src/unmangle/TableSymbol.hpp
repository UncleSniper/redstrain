#include "SpecialSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Type;

	class REDSTRAIN_REDMOND_API TableSymbol : public SpecialSymbol {

	  public:
		enum TableSymbolType {
			TST_VIRTUAL_TABLE,
			TST_VTT_STRUCTURE,
			TST_TYPEINFO_STRUCTURE,
			TST_TYPEINFO_NAME
		};

	  private:
		TableSymbolType tableType;
		Type* targetType;

	  protected:
		virtual void print(std::ostream&, bool&) const;

	  public:
		TableSymbol(TableSymbolType, Type*);
		TableSymbol(const TableSymbol&);
		virtual ~TableSymbol();

		virtual SpecialSymbolType getSpecialSymbolType() const;
		virtual CPPSymbol* cloneSymbol() const;

	};

}}}
