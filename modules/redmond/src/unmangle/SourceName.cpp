#include "SourceName.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	SourceName::SourceName(const string& segment) : segment(segment) {}

	SourceName::SourceName(const SourceName& name) : UnqualifiedName(name), segment(name.segment) {}

	SourceName::~SourceName() {}

	Name::NameType SourceName::getNameType() const {
		return NT_SOURCE;
	}

	void SourceName::print(ostream& out, bool& lastWasGreater, const string*) const {
		out << segment;
		lastWasGreater = false;
	}

	Name* SourceName::cloneName() const {
		return new SourceName(*this);
	}

	const string* SourceName::getUnqualifiedClassNameData() const {
		return &segment;
	}

}}}
