#ifndef REDSTRAIN_MOD_BUILD_GOAL_HPP
#define REDSTRAIN_MOD_BUILD_GOAL_HPP

#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class BuildContext;

	class REDSTRAIN_BUILD_API Goal : public util::ReferenceCounted {

	  public:
		Goal();
		Goal(const Goal&);
		virtual ~Goal();

		virtual void attain(BuildContext&) = 0;
		virtual void wouldAttain(BuildContext&) = 0;

		virtual void dumpGoal(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GOAL_HPP */
