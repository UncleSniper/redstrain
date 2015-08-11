#ifndef REDSTRAIN_MOD_BUILD_TRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_TRANSFORM_HPP

#include <redstrain/util/UniqueList.hpp>
#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Artifact;
	class BuildContext;

	class REDSTRAIN_BUILD_API Transform : public util::ReferenceCounted {

	  private:
		typedef util::UniqueList<Artifact*> Prerequisites;

	  public:
		typedef Prerequisites::Iterator PrerequisiteIterator;

	  private:
		Prerequisites prerequisites;

	  public:
		Transform();
		Transform(const Transform&);
		virtual ~Transform();

		bool addPrerequisite(Artifact&);
		bool removePrerequisite(Artifact&);
		void clearPrerequisites();
		void getPrerequisites(PrerequisiteIterator&, PrerequisiteIterator&) const;

		virtual void perform(BuildContext&, Artifact&) = 0;
		virtual void wouldPerform(BuildContext&, Artifact&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_TRANSFORM_HPP */
