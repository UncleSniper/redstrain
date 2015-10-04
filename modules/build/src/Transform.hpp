#ifndef REDSTRAIN_MOD_BUILD_TRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_TRANSFORM_HPP

#include <string>
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
		std::string componentType, componentName, componentBase;

	  protected:
		void dumpTransformAspects(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	  public:
		Transform();
		Transform(const Transform&);
		virtual ~Transform();

		inline const std::string& getComponentType() const {
			return componentType;
		}

		inline const std::string& getComponentName() const {
			return componentName;
		}

		inline const std::string& getComponentBaseDirectory() const {
			return componentBase;
		}

		void setComponentType(const std::string&);
		void setComponentName(const std::string&);
		void setComponentBaseDirectory(const std::string&);

		bool addPrerequisite(Artifact&);
		bool removePrerequisite(Artifact&);
		void clearPrerequisites();
		void getPrerequisites(PrerequisiteIterator&, PrerequisiteIterator&) const;

		virtual void perform(BuildContext&, Artifact&) = 0;
		virtual void wouldPerform(BuildContext&, Artifact&);

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_TRANSFORM_HPP */
