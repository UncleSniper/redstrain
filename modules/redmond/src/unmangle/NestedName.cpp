#include "NestedName.hpp"
#include "UnqualifiedName.hpp"
#include "TemplateArgument.hpp"
#include "../unmangle-utils.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	// ======== Segment ========

	NestedName::Segment::Segment(UnqualifiedName* prefix) : prefix(prefix) {}

	NestedName::Segment::Segment(const Segment& segment) {
		UnmangleCollectionPtr<Arguments> delArgs(&arguments);
		ArgumentIterator begin(segment.arguments.begin()), end(segment.arguments.end());
		UnmanglePtr<TemplateArgument> arg(NULL);
		for(; begin != end; ++begin) {
			arg.ptr = (*begin)->cloneTemplateArgument();
			arguments.push_back(arg.ptr);
			arg.ptr = NULL;
		}
		prefix = static_cast<UnqualifiedName*>(segment.prefix->cloneName());
		delArgs.ptr = NULL;
	}

	NestedName::Segment::~Segment() {
		delete prefix;
		ArgumentIterator begin(arguments.begin()), end(arguments.end());
		for(; begin != end; ++begin)
			delete *begin;
	}

	bool NestedName::Segment::hasArguments() const {
		return !arguments.empty();
	}

	unsigned NestedName::Segment::getArgumentCount() const {
		return static_cast<unsigned>(arguments.size());
	}

	void NestedName::Segment::getArguments(ArgumentIterator& begin, ArgumentIterator& end) const {
		begin = arguments.begin();
		end = arguments.end();
	}

	void NestedName::Segment::addArgument(TemplateArgument& argument) {
		arguments.push_back(&argument);
	}

	// ======== NestedName ========

	NestedName::NestedName(int qualifiers) : qualifiers(qualifiers) {}

	NestedName::NestedName(const NestedName& name) : Name(name), qualifiers(name.qualifiers) {
		UnmangleCollectionPtr<Segments> delSegs(&segments);
		SegmentIterator begin(name.segments.begin()), end(name.segments.end());
		UnmanglePtr<Segment> seg(NULL);
		for(; begin != end; ++begin) {
			seg.ptr = new Segment(**begin);
			segments.push_back(seg.ptr);
			seg.ptr = NULL;
		}
		delSegs.ptr = NULL;
	}

	NestedName::~NestedName() {
		SegmentIterator begin(segments.begin()), end(segments.end());
		for(; begin != end; ++begin)
			delete *begin;
	}

	bool NestedName::hasSegments() const {
		return !segments.empty();
	}

	unsigned NestedName::getSegmentCount() const {
		return static_cast<unsigned>(segments.size());
	}

	void NestedName::getSegments(SegmentIterator& begin, SegmentIterator& end) const {
		begin = segments.begin();
		end = segments.end();
	}

	NestedName::Segment* NestedName::getLastSegment() const {
		return segments.empty() ? NULL : segments.back();
	}

	void NestedName::addSegment(Segment& segment) {
		segments.push_back(&segment);
	}

	Name::NameType NestedName::getNameType() const {
		return NT_NESTED;
	}

	int NestedName::getNameCVQualifiers() const {
		return qualifiers;
	}

	void NestedName::getCurrentTemplateArguments(CurrentTemplateArguments& arguments) const {
		arguments.clear();
		if(segments.empty())
			return;
		const Segment& ls = *segments.back();
		if(!ls.hasArguments())
			return;
		arguments.reserve(static_cast<CurrentTemplateArguments::size_type>(ls.getArgumentCount()));
		Segment::ArgumentIterator abegin, aend;
		ls.getArguments(abegin, aend);
		for(; abegin != aend; ++abegin)
			arguments.push_back(*abegin);
	}

	void NestedName::print(ostream& out, bool& lastWasGreater, const CurrentTemplateArguments& arguments,
			const string*) const {
		const string* lastClassName = NULL;
		SegmentIterator sbegin(segments.begin()), send(segments.end());
		Segment::ArgumentIterator abegin, aend;
		bool firstSeg = true;
		for(; sbegin != send; ++sbegin) {
			if(firstSeg)
				firstSeg = false;
			else
				out << "::";
			const Segment& seg = **sbegin;
			seg.getPrefix().print(out, lastWasGreater, arguments, lastClassName);
			if(seg.hasArguments()) {
				seg.getArguments(abegin, aend);
				out << '<';
				bool firstArg = true;
				for(; abegin != aend; ++abegin) {
					if(firstArg)
						firstArg = false;
					else
						out << ", ";
					lastWasGreater = false;
					(*abegin)->print(out, lastWasGreater, arguments);
				}
				if(lastWasGreater)
					out << ' ';
				else
					lastWasGreater = true;
				out << '>';
			}
			lastClassName = seg.getPrefix().getUnqualifiedClassNameData();
		}
	}

	Name* NestedName::cloneName() const {
		return new NestedName(*this);
	}

	bool NestedName::namesTemplate() const {
		if(segments.empty())
			return false;
		return segments.back()->hasArguments();
	}

	bool NestedName::namesReturnless() const {
		if(segments.empty())
			return false;
		return segments.back()->getPrefix().namesReturnless();
	}

}}}
