#ifndef REDSTRAIN_MOD_BUILD_COMPONENT_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENT_HPP

#include <set>
#include <map>
#include <string>
#include <redstrain/util/UniqueList.hpp>
#include <redstrain/util/ReferenceCounted.hpp>

#include "Flavor.hpp"

namespace redengine {
namespace build {

	class Language;
	class Artifact;
	class FileArtifact;
	class ComponentTypeStringifier;

	class REDSTRAIN_BUILD_API Component : public util::ReferenceCounted {

	  public:
		enum REDSTRAIN_BUILD_API Type {
			LIBRARY,
			EXECUTABLE,
			DATA,
			BLOB
		};

		class REDSTRAIN_BUILD_API FlavoredArtifact {

		  private:
			Artifact* artifact;
			Flavor flavor;
			const Language* language;

		  public:
			FlavoredArtifact(Artifact&, const Flavor&, const Language&);
			FlavoredArtifact(const FlavoredArtifact&);
			~FlavoredArtifact();

			inline Artifact& getArtifact() const {
				return *artifact;
			}

			inline const Flavor& getFlavor() const {
				return flavor;
			}

			inline const Language& getLanguage() const {
				return *language;
			}

			FlavoredArtifact& operator=(const FlavoredArtifact&);

			bool operator==(const FlavoredArtifact&) const;
			bool operator!=(const FlavoredArtifact&) const;
			bool operator<(const FlavoredArtifact&) const;
			bool operator>(const FlavoredArtifact&) const;

		};

	  private:
		typedef util::UniqueList<std::string> Paths;
		typedef std::set<Language*> Languages;
		typedef std::set<Component*> Components;
		typedef std::map<const Language*, std::map<std::string, FileArtifact*> > Headers;
		typedef util::UniqueList<FlavoredArtifact> FlavoredArtifacts;
		typedef std::map<const Artifact*, FileArtifact*> UnexposedHeaders;
		typedef UnexposedHeaders::iterator UnexposedHeaderIterator;
		typedef UnexposedHeaders::const_iterator ConstUnexposedHeaderIterator;
		typedef std::map<const Language*, std::set<std::string> > ExposeDirectories;

	  public:
		typedef Paths::Iterator PathIterator;
		typedef Languages::const_iterator LanguageIterator;
		typedef Components::const_iterator ComponentIterator;
		typedef FlavoredArtifacts::Iterator FlavoredArtifactIterator;
		typedef std::set<std::string>::const_iterator ExposeDirectoryIterator;
		typedef std::map<std::string, FileArtifact*>::const_iterator ExposedHeaderIterator;

	  private:
		const Type type;
		std::string name;
		const std::string baseDirectory;
		Paths sourceDirectories;
		Languages languages;
		Components dependencies;
		Headers privateHeaders, exposedHeaders;
		FlavoredArtifacts finalArtifacts;
		UnexposedHeaders unexposedHeaders, reverseUnexposedHeaders;
		ExposeDirectories exposeDirectories;

	  protected:
		void setName(const std::string&);

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
		bool removeSourceDirectory(const std::string&);
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
		void getExposedHeaders(const Language&, ExposedHeaderIterator&, ExposedHeaderIterator&) const;

		bool addFinalArtifact(Artifact&, const Flavor&, const Language&);
		bool removeFinalArtifact(Artifact&, const Flavor&, const Language&);
		void clearFinalArtifacts();
		void getFinalArtifacts(FlavoredArtifactIterator&, FlavoredArtifactIterator&) const;

		bool addUnexposedHeader(const Artifact&, FileArtifact&);
		bool removeUnexposedHeader(const Artifact&);
		void clearUnexposedHeaders();
		FileArtifact* getUnexposedHeader(const Artifact&) const;

		bool addReverseUnexposedHeader(const Artifact&, FileArtifact&);
		bool removeReverseUnexposedHeader(const Artifact&);
		void clearReverseUnexposedHeaders();
		FileArtifact* getReverseUnexposedHeader(const Artifact&) const;

		bool addHeaderExposeDirectory(const Language&, const std::string&);
		bool removeHeaderExposeDirectory(const Language&, const std::string&);
		void clearHeaderExposeDirectories(const Language&);
		void clearHeaderExposeDirectories();
		void getHeaderExposeDirectories(const Language&, ExposeDirectoryIterator&, ExposeDirectoryIterator&) const;

		static std::string::size_type getMaximalComponentTypeWidth(ComponentTypeStringifier&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENT_HPP */
