#ifndef REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP
#define REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP

#include <map>
#include <set>
#include <list>
#include <string>
#include <redstrain/util/MapKeyIterator.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Goal;
	class BuildUI;
	class Component;
	class FileArtifact;

	class REDSTRAIN_BUILD_API BuildContext {

	  public:
		class REDSTRAIN_BUILD_API CachedHeaderReference {

		  private:
			std::string path;
			bool local;

		  public:
			CachedHeaderReference(const std::string&, bool);
			CachedHeaderReference(const CachedHeaderReference&);

			inline const std::string& getPath() const {
				return path;
			}

			void setPath(const std::string&);

			inline bool isLocal() const {
				return local;
			}

			inline void setLocal(bool local) {
				this->local = local;
			}

			CachedHeaderReference& operator=(const CachedHeaderReference&);

		};

		class REDSTRAIN_BUILD_API CachedArtifactIncludes {

		  private:
			typedef std::list<CachedHeaderReference> References;

		  public:
			typedef References::const_iterator ReferenceIterator;

		  private:
			time_t timestamp;
			References references;

		  public:
			CachedArtifactIncludes();
			CachedArtifactIncludes(const CachedArtifactIncludes&);

			inline time_t getTimestamp() const {
				return timestamp;
			}

			inline void setTimestamp(time_t timestamp) {
				this->timestamp = timestamp;
			}

			void getReferences(ReferenceIterator&, ReferenceIterator&) const;
			void addReference(const CachedHeaderReference&);

		};

	  private:
		typedef std::map<std::string, Goal*> Goals;
		typedef Goals::iterator GoalIterator;
		typedef Goals::const_iterator ConstGoalIterator;
		typedef std::map<std::string, FileArtifact*> FileArtifacts;
		typedef std::set<const Component*> Components;

	  public:
		typedef util::MapKeyIterator<std::string, Goal*> GoalNameIterator;
		typedef FileArtifacts::const_iterator FileArtifactIterator;

	  private:
		BuildUI& ui;
		time_t virtualTime;
		Goals goals;
		FileArtifacts fileArtifacts;
		Components buildingComponents, builtComponents;
		Goal* defaultGoal;
		std::string includeCachePath;

	  private:
		BuildContext(const BuildContext&);

	  public:
		BuildContext(BuildUI&);
		~BuildContext();

		inline BuildUI& getUI() {
			return ui;
		}

		inline const BuildUI& getUI() const {
			return ui;
		}

		inline time_t getVirtualTime() const {
			return virtualTime;
		}

		time_t tickVirtualTime();

		const std::string& getIncludeCachePath() const {
			return includeCachePath;
		}

		void setIncludeCachePath(const std::string&);

		Goal* getGoal(const std::string&) const;
		bool addGoal(const std::string&, Goal&);
		bool removeGoal(const std::string&);
		void clearGoals();
		void getGoals(GoalNameIterator&, GoalNameIterator&) const;

		FileArtifact& internFileArtifact(const std::string&, const std::string&);

		void beginBuildingComponent(Component&);
		void endBuildingComponent(const Component&);
		bool hasComponentBeenBuilt(const Component&) const;

		inline Goal* getDefaultGoal() const {
			return defaultGoal;
		}

		void setDefaultGoal(Goal*);

		void dumpContext(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP */
