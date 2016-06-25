#include "NestedName.hpp"
#include "SymbolSink.hpp"
#include "UnqualifiedName.hpp"
#include "TemplateArgument.hpp"
#include "../unmangle-utils.hpp"

using std::string;

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

	void NestedName::print(SymbolSink& sink, const CurrentTemplateArguments& arguments, const string*) const {
		const string* lastClassName = NULL;
		SegmentIterator sbegin(segments.begin()), send(segments.end());
		Segment::ArgumentIterator abegin, aend;
		bool firstSeg = true;
		for(; sbegin != send; ++sbegin) {
			if(firstSeg)
				firstSeg = false;
			else
				sink.putSeparator(SymbolSink::SEP_PAAMAYIM_NEKUDOTAYIM);
			const Segment& seg = **sbegin;
			seg.getPrefix().print(sink, arguments, lastClassName);
			if(seg.hasArguments()) {
				sink.putSeparator(SymbolSink::SEP_LEFT_ANGLE);
				seg.getArguments(abegin, aend);
				unsigned space = sink.getRemainingColumnCount();
				bool breakArgs = false, firstArg = true;
				if(space) {
					unsigned iwidth = 0u;
					Segment::ArgumentIterator sabegin(abegin), saend(aend);
					for(; sabegin != saend; ++sabegin) {
						if(firstArg)
							firstArg = false;
						else
							iwidth += sink.getInlineWidthOf(SymbolSink::SEP_COMMA)
									+ sink.getInlineWidthOf(SymbolSink::SEP_AFTER_COMMA);
						iwidth += sink.getInlineWidthOf(**sabegin, arguments);
					}
					iwidth += sink.getInlineWidthOf(SymbolSink::SEP_RIGHT_ANGLE);
					SegmentIterator ssbegin(sbegin);
					const string* slcn = seg.getPrefix().getUnqualifiedClassNameData();
					for(++ssbegin; ssbegin != send; ++ssbegin) {
						iwidth += sink.getInlineWidthOf(SymbolSink::SEP_PAAMAYIM_NEKUDOTAYIM)
								+ sink.getInlineWidthOf((*ssbegin)->getPrefix(), arguments, slcn);
						if((*ssbegin)->hasArguments()) {
							iwidth += sink.getInlineWidthOf(SymbolSink::SEP_LEFT_ANGLE);
							break;
						}
						slcn = (*ssbegin)->getPrefix().getUnqualifiedClassNameData();
					}
					firstArg = true;
					breakArgs = iwidth >= space;
				}
				for(; abegin != aend; ++abegin) {
					if(firstArg) {
						firstArg = false;
						if(breakArgs)
							sink.startNewLine(1);
					}
					else {
						sink.putSeparator(SymbolSink::SEP_COMMA);
						if(breakArgs)
							sink.startNewLine(0);
						else
							sink.putSeparator(SymbolSink::SEP_AFTER_COMMA);
					}
					(*abegin)->print(sink, arguments);
				}
				if(breakArgs && !firstArg)
					sink.startNewLine(-1);
				sink.putSeparator(SymbolSink::SEP_RIGHT_ANGLE);
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
