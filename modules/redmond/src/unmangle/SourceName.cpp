#include "SourceName.hpp"
#include "SymbolSink.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	SourceName::SourceName(const string& segment) : segment(segment) {}

	SourceName::SourceName(const SourceName& name) : UnqualifiedName(name), segment(name.segment) {}

	SourceName::~SourceName() {}

	Name::NameType SourceName::getNameType() const {
		return NT_SOURCE;
	}

	void SourceName::print(SymbolSink& sink, const CurrentTemplateArguments&, const string*) const {
		sink.putSourceName(segment);
	}

	Name* SourceName::cloneName() const {
		return new SourceName(*this);
	}

	const string* SourceName::getUnqualifiedClassNameData() const {
		return &segment;
	}

	bool SourceName::namesTemplate() const {
		return false;
	}

	bool SourceName::namesReturnless() const {
		return false;
	}

}}}
