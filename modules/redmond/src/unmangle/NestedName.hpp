#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_NESTEDNAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_NESTEDNAME_HPP

#include <list>

#include "Name.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class UnqualifiedName;
	class TemplateArgument;

	class REDSTRAIN_REDMOND_API NestedName : public Name {

	  public:
		class REDSTRAIN_REDMOND_API Segment {

		  private:
			typedef std::list<TemplateArgument*> Arguments;

		  public:
			typedef Arguments::const_iterator ArgumentIterator;

		  private:
			UnqualifiedName* prefix;
			Arguments arguments;

		  public:
			Segment(UnqualifiedName*);
			Segment(const Segment&);
			~Segment();

			inline UnqualifiedName& getPrefix() const {
				return *prefix;
			}

			bool hasArguments() const;
			unsigned getArgumentCount() const;
			void getArguments(ArgumentIterator&, ArgumentIterator&) const;
			void addArgument(TemplateArgument&);

		};

	  private:
		typedef std::list<Segment*> Segments;

	  public:
		typedef Segments::const_iterator SegmentIterator;

	  private:
		int qualifiers;
		Segments segments;

	  public:
		NestedName(int);
		NestedName(const NestedName&);
		virtual ~NestedName();

		inline int getCVQualifiers() const {
			return qualifiers;
		}

		bool hasSegments() const;
		unsigned getSegmentCount() const;
		void getSegments(SegmentIterator&, SegmentIterator&) const;
		Segment* getLastSegment() const;
		void addSegment(Segment&);

		virtual NameType getNameType() const;
		virtual void print(std::ostream&, bool&, const std::string*) const;
		virtual Name* cloneName() const;
		virtual bool namesTemplate() const;
		virtual bool namesReturnless() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_NESTEDNAME_HPP */
