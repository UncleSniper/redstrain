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

	void NestedName::addSegment(Segment& segment) {
		segments.push_back(&segment);
	}

	Name::NameType NestedName::getNameType() const {
		return NT_NESTED;
	}

	void NestedName::print(ostream& out, bool& lastWasGreater, const string*) const {
		const string* lastClassName = NULL;
		SegmentIterator sbegin(segments.begin()), send(segments.end());
		Segment::ArgumentIterator abegin, aend;
		for(; sbegin != send; ++sbegin) {
			const Segment& seg = **sbegin;
			seg.getPrefix().print(out, lastWasGreater, lastClassName);
			if(seg.hasArguments()) {
				seg.getArguments(abegin, aend);
				out << '<';
				bool first = true;
				for(; abegin != aend; ++abegin) {
					if(first)
						first = false;
					else
						out << ", ";
					lastWasGreater = false;
					(*abegin)->print(out, lastWasGreater);
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
