#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_SOURCENAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_SOURCENAME_HPP

#include "UnqualifiedName.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API SourceName : public UnqualifiedName {

	  private:
		std::string segment;

	  public:
		SourceName(const std::string&);
		SourceName(const SourceName&);
		virtual ~SourceName();

		virtual NameType getNameType() const;
		virtual void print(std::ostream&, bool&, const std::string*) const;
		virtual Name* cloneName() const;
		virtual const string* getUnqualifiedClassNameData() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_SOURCENAME_HPP */
