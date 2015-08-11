#ifndef REDSTRAIN_MOD_BUILD_COMPONENT_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENT_HPP

#include <set>
#include <map>
#include <string>
#include <redstrain/util/UniqueList.hpp>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Language;
	class Artifact;
	class FileArtifact;

	class REDSTRAIN_BUILD_API Component : public util::ReferenceCounted {

	  public:
		enum REDSTRAIN_BUILD_API Type {
			LIBRARY,
			EXECUTABLE,
			DATA,
			BLOB
		};

	  private:
		typedef util::UniqueList<std::string> Paths;
		typedef std::set<Language*> Languages;
		typedef std::set<Component*> Components;
		typedef std::map<const Language*, std::map<std::string, FileArtifact*> > Headers;
		typedef util::UniqueList<Artifact*> Artifacts;
		typedef std::map<const Artifact*, FileArtifact*> UnexposedHeaders;
		typedef UnexposedHeaders::iterator UnexposedHeaderIterator;
		typedef UnexposedHeaders::const_iterator ConstUnexposedHeaderIterator;

	  public:
		typedef Paths::Iterator PathIterator;
		typedef Languages::const_iterator LanguageIterator;
		typedef Components::const_iterator ComponentIterator;
		typedef Artifacts::Iterator ArtifactIterator;

	  private:
		const Type type;
		const std::string name, baseDirectory;
		Paths sourceDirectories;
		Languages languages;
		Components dependencies;
		Headers privateHeaders, exposedHeaders;
		Artifacts finalArtifacts;
		UnexposedHeaders unexposedHeaders;

	  public:
		Component(Type, const std::string&, const std::string&);
		Component(const Component&);
		virtual ~Component();

		inline Type getType() const {
			return type;
		}

		inline const std::string& getName() const {
			return name;
		}

		inline const std::string& getBaseDirectory() const {
			return baseDirectory;
		}

		std::string getGoalName() const;

		bool addSourceDirectory(const std::string&);
		bool removeSourceDirecotry(const std::string&);
		void clearSourceDirectories();
		void getSourceDirectories(PathIterator&, PathIterator&) const;

		bool addLanguage(Language&);
		bool removeLanguage(Language&);
		void clearLanguages();
		void getLanguages(LanguageIterator&, LanguageIterator&) const;

		bool addDependency(Component&);
		bool removeDependency(Component&);
		void clearDependencies();
		void getDependencies(ComponentIterator&, ComponentIterator&) const;
		void getTransitiveDependencies(std::list<Component*>&) const;

		bool addPrivateHeader(const Language&, const std::string&, FileArtifact&);
		bool removePrivateHeader(const Language&, const std::string&);
		void clearPrivateHeaders(const Language&);
		void clearPrivateHeaders();
		FileArtifact* getPrivateHeader(const Language&, const std::string&) const;

		bool addExposedHeader(const Language&, const std::string&, FileArtifact&);
		bool removeExposedHeader(const Language&, const std::string&);
		void clearExposedHeaders(const Language&);
		void clearExposedHeaders();
		FileArtifact* getExposedHeader(const Language&, const std::string&) const;

		bool addFinalArtifact(Artifact&);
		bool removeFinalArtifact(Artifact&);
		void clearFinalArtifacts();
		void getFinalArtifacts(ArtifactIterator&, ArtifactIterator&) const;

		bool addUnexposedHeader(const Artifact&, FileArtifact&);
		bool removeUnexposedHeader(const Artifact&);
		void clearUnexposedHeaders();
		FileArtifact* getUnexposedHeader(const Artifact&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENT_HPP */
