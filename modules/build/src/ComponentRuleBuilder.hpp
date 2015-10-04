#ifndef REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP

#include "RuleBuilder.hpp"

namespace redengine {
namespace build {

	class BuildArtifactMapper;
	class ArtifactStageMapper;
	class BuildDirectoryMapper;
	class TransformPropertyInjector;

	class REDSTRAIN_BUILD_API ComponentRuleBuilder : public RuleBuilder {

	  private:
		BuildDirectoryMapper& directoryMapper;
		BuildArtifactMapper& artifactMapper;
		TransformPropertyInjector* transformPropertyInjector;
		ArtifactStageMapper* stageMapper;

	  private:
		void setupRulesImpl(const Project&, Component&, BuildContext&);

	  public:
		ComponentRuleBuilder(BuildDirectoryMapper&, BuildArtifactMapper&,
				TransformPropertyInjector* = NULL, ArtifactStageMapper* = NULL);
		ComponentRuleBuilder(const ComponentRuleBuilder&);

		inline BuildDirectoryMapper& getBuildDirectoryMapper() {
			return directoryMapper;
		}

		inline const BuildDirectoryMapper& getBuildDirectoryMapper() const {
			return directoryMapper;
		}

		inline BuildArtifactMapper& getBuildArtifactMapper() {
			return artifactMapper;
		}

		inline const BuildArtifactMapper& getBuildArtifactMapper() const {
			return artifactMapper;
		}

		inline TransformPropertyInjector* getTransformPropertyInjector() const {
			return transformPropertyInjector;
		}

		inline void setTransformPropertyInjector(TransformPropertyInjector* injector) {
			transformPropertyInjector = injector;
		}

		inline ArtifactStageMapper* getArtifactStageMapper() const {
			return stageMapper;
		}

		inline void setArtifactStageMapper(ArtifactStageMapper* mapper) {
			stageMapper = mapper;
		}

		virtual void setupRules(const Project&, Component&, BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP */
