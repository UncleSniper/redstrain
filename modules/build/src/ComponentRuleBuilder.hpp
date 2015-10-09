#ifndef REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP

#include "Language.hpp"
#include "RuleBuilder.hpp"

namespace redengine {
namespace build {

	class BuildArtifactMapper;
	class ArtifactStageMapper;
	class BuildDirectoryMapper;
	class GoalPropertyInjector;
	class TransformPropertyInjector;

	class REDSTRAIN_BUILD_API ComponentRuleBuilder : public RuleBuilder {

	  private:
		BuildDirectoryMapper& directoryMapper;
		BuildArtifactMapper& artifactMapper;
		TransformPropertyInjector* transformPropertyInjector;
		ArtifactStageMapper* stageMapper;
		GoalPropertyInjector* goalPropertyInjector;

	  private:
		void setupRulesImpl(const Project&, Component&, BuildContext&);

	  public:
		ComponentRuleBuilder(BuildDirectoryMapper&, BuildArtifactMapper&,
				TransformPropertyInjector* = NULL, ArtifactStageMapper* = NULL, GoalPropertyInjector* = NULL);
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

		inline GoalPropertyInjector* getGoalPropertyInjector() const {
			return goalPropertyInjector;
		}

		inline void setGoalPropertyInjector(GoalPropertyInjector* injector) {
			goalPropertyInjector = injector;
		}

		virtual void setupRules(const Project&, Component&, BuildContext&);

		static void announceSourceReferencesHeader(FileArtifact&, const Component&, Language&,
				Transform&, const Language::ReferencedHeader&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP */
