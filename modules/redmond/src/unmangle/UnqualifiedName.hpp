#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_UNQUALIFIEDNAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_UNQUALIFIEDNAME_HPP

#include "Name.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API UnqualifiedName : public Name {

	  public:
		UnqualifiedName();
		UnqualifiedName(const UnqualifiedName&);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_UNQUALIFIEDNAME_HPP */
