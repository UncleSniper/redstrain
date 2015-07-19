#ifndef REDSTRAIN_MOD_BUILD_COMPONENT_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENT_HPP

#include <set>
#include <list>
#include <string>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Flavor;
	class Language;
	class BuildContext;

	class REDSTRAIN_BUILD_API Component : public util::ReferenceCounted {

	  public:
		enum REDSTRAIN_BUILD_API Type {
			LIBRARY,
			EXECUTABLE,
			DATA,
			BLOB
		};

		class REDSTRAIN_BUILD_API BuildDirectoryMapper {

		  public:
			BuildDirectoryMapper();
			BuildDirectoryMapper(const BuildDirectoryMapper&);
			virtual ~BuildDirectoryMapper();

			virtual std::string getBuildDirectory(const Language&, const Flavor&) = 0;
			virtual std::string getHeaderExposeDirectory(const Language&) = 0;

		};

	  private:
		typedef std::list<std::string> Paths;
		typedef std::set<Language*> Languages;

	  public:
		typedef Paths::const_iterator PathIterator;
		typedef Languages::const_iterator LanguageIterator;

	  private:
		const Type type;
		const std::string name, baseDirectory;
		Paths sourceDirectories;
		Languages languages;

	  public:
		Component(Type, const std::string&, const std::string&);
		Component(const Component&);

		inline Type getType() const {
			return type;
		}

		inline const std::string& getName() const {
			return name;
		}

		inline const std::string& getBaseDirectory() const {
			return baseDirectory;
		}

		void addSourceDirectory(const std::string&);
		void clearSourceDirectories();
		void getSourceDirectories(PathIterator&, PathIterator&) const;
		bool addLanguage(Language*);
		void clearLanguages();
		void getLanguages(LanguageIterator&, LanguageIterator&) const;
		void setupRules(BuildDirectoryMapper&, BuildContext&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENT_HPP */
