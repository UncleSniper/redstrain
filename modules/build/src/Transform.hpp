#ifndef REDSTRAIN_MOD_BUILD_TRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_TRANSFORM_HPP

#include <redstrain/util/UniqueList.hpp>
#include <redstrain/util/ReferenceCounted.hpp>

#include "ComponentUIInfo.hpp"

namespace redengine {
namespace build {

	class Artifact;
	class BuildContext;

	class REDSTRAIN_BUILD_API Transform : public util::ReferenceCounted, public ComponentUIInfo {

	  private:
		typedef util::UniqueList<Artifact*> Prerequisites;

	  public:
		typedef Prerequisites::Iterator PrerequisiteIterator;

	  private:
		Prerequisites prerequisites;
		bool uiMinor;

	  protected:
		void dumpTransformAspects(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	  public:
		Transform();
		Transform(const Transform&);
		virtual ~Transform();

		inline bool isUIMinor() const {
			return uiMinor;
		}

		inline void setUIMinor(bool isMinor) {
			uiMinor = isMinor;
		}

		bool addPrerequisite(Artifact&);
		bool removePrerequisite(Artifact&);
		void clearPrerequisites();
		void getPrerequisites(PrerequisiteIterator&, PrerequisiteIterator&) const;

		virtual void perform(BuildContext&, Artifact&) = 0;
		virtual void wouldPerform(BuildContext&, Artifact&);
		virtual bool onlyModifiesTarget() const;

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_TRANSFORM_HPP */
