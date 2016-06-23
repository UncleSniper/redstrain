#include "SymbolSink.hpp"
#include "VendorExtendedQualifiedType.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	VendorExtendedQualifiedType::VendorExtendedQualifiedType(const string& name, Type* type)
			: name(name), type(type) {}

	VendorExtendedQualifiedType::VendorExtendedQualifiedType(const VendorExtendedQualifiedType& type)
			: Type(type), name(type.name), type(type.type->cloneType()) {}

	VendorExtendedQualifiedType::~VendorExtendedQualifiedType() {
		delete type;
	}

	Type::TypeType VendorExtendedQualifiedType::getTypeType() const {
		return TT_VENDOR_EXTENDED_QUALIFIED;
	}

	bool VendorExtendedQualifiedType::inlinesEnclosingClassName() const {
		return type->inlinesEnclosingClassName();
	}

	void VendorExtendedQualifiedType::print(SymbolSink& sink, const CurrentTemplateArguments& arguments,
			const Type* enclosingClass) const {
		sink.putVendorExtendedTypeQualifier(name);
		type->print(sink, arguments, enclosingClass);
	}

	Type* VendorExtendedQualifiedType::cloneType() const {
		return new VendorExtendedQualifiedType(*this);
	}

}}}
