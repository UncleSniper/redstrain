#include "SymbolSink.hpp"
#include "VendorExtendedType.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	VendorExtendedType::VendorExtendedType(const string& name) : name(name) {}

	VendorExtendedType::VendorExtendedType(const VendorExtendedType& type) : Type(type), name(type.name) {}

	VendorExtendedType::~VendorExtendedType() {}

	Type::TypeType VendorExtendedType::getTypeType() const {
		return TT_VENDOR_EXTENDED;
	}

	void VendorExtendedType::print(SymbolSink& sink, const CurrentTemplateArguments&, const Type*) const {
		sink.putVendorExtendedType(name);
	}

	Type* VendorExtendedType::cloneType() const {
		return new VendorExtendedType(*this);
	}

}}}
