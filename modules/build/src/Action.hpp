#ifndef REDSTRAIN_MOD_BUILD_ACTION_HPP
#define REDSTRAIN_MOD_BUILD_ACTION_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class BuildContext;

	class REDSTRAIN_BUILD_API Action : public util::ReferenceCounted {

	  public:
		Action();
		Action(const Action&);

		virtual void perform(BuildContext&) = 0;
		virtual void wouldPerform(BuildContext&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ACTION_HPP */
