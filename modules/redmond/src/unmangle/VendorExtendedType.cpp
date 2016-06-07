#include "VendorExtendedType.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	VendorExtendedType::VendorExtendedType(const string& name) : name(name) {}

	VendorExtendedType::VendorExtendedType(const VendorExtendedType& type) : Type(type), name(type.name) {}

	VendorExtendedType::~VendorExtendedType() {}

	Type::TypeType VendorExtendedType::getTypeType() const {
		return TT_VENDOR_EXTENDED;
	}

	void VendorExtendedType::print(ostream& out, bool& lastWasGreater) const {
		out << name;
		lastWasGreater = !name.empty() && name[name.length() - static_cast<string::size_type>(1u)] == '>';
	}

	Type* VendorExtendedType::cloneType() const {
		return new VendorExtendedType(*this);
	}

}}}
