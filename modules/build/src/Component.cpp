#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "Flavor.hpp"
#include "Language.hpp"

using std::list;
using std::string;
using redengine::util::Appender;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;

namespace redengine {
namespace build {

	// ======== BuildDirectoryMapper ========

	Component::BuildDirectoryMapper::BuildDirectoryMapper() {}

	Component::BuildDirectoryMapper::BuildDirectoryMapper(const BuildDirectoryMapper&) {}

	Component::BuildDirectoryMapper::~BuildDirectoryMapper() {}

	// ======== PrimaryArtifactMapper ========

	Component::PrimaryArtifactMapper::PrimaryArtifactMapper() {}

	Component::PrimaryArtifactMapper::PrimaryArtifactMapper(const PrimaryArtifactMapper&) {}

	Component::PrimaryArtifactMapper::~PrimaryArtifactMapper() {}

	// ======== RuleSink ========

	Component::RuleSink::RuleSink() {}

	Component::RuleSink::RuleSink(const RuleSink&) {}

	Component::RuleSink::~RuleSink() {}

	// ======== Component ========

	Component::Component(Type type, const string& name, const string& baseDirectory)
			: type(type), name(name), baseDirectory(baseDirectory) {}

	Component::Component(const Component& component)
			: ReferenceCounted(component), type(component.type), name(component.name),
			baseDirectory(component.baseDirectory) {}

	void Component::addSourceDirectory(const string& directory) {
		if(!directory.empty())
			sourceDirectories.push_back(directory);
	}

	void Component::clearSourceDirectories() {
		sourceDirectories.clear();
	}

	void Component::getSourceDirectories(PathIterator& begin, PathIterator& end) const {
		begin = sourceDirectories.begin();
		end = sourceDirectories.end();
	}

	bool Component::addLanguage(Language* language) {
		if(!language)
			return false;
		return languages.insert(language).second;
	}

	void Component::clearLanguages() {
		languages.clear();
	}

	void Component::getLanguages(LanguageIterator& begin, LanguageIterator& end) const {
		begin = languages.begin();
		end = languages.end();
	}

	struct LanguageInfo {

		Language& language;
		list<string> sources, headers;

		LanguageInfo(Language& language) : language(language) {}

		LanguageInfo(const LanguageInfo& info)
				: language(info.language), sources(info.sources), headers(info.headers) {}

	};

	struct SetupTraverser : public Filesystem::TraversalSink {

		list<LanguageInfo>& languages;

		SetupTraverser(list<LanguageInfo>& languages) : languages(languages) {}

		virtual bool enterDirectory(const string&) {
			return true;
		}

		virtual void leaveDirectory(const string&, bool) {}

		virtual void visitFile(const string& path) {
			list<LanguageInfo>::iterator begin(languages.begin()), end(languages.end());
			for(; begin != end; ++begin) {
				switch(begin->language.classifyFile(path)) {
					case Language::AT_SOURCE:
						begin->sources.push_back(path);
						break;
					case Language::AT_HEADER:
						begin->headers.push_back(path);
						break;
					default:
						break;
				}
			}
		}

	};

	struct FlavorAppender : public Appender<Flavor> {

		const Component& component;
		const string& sourceDirectory;
		Language& language;
		const list<string>& sources;
		Component::BuildDirectoryMapper& directoryMapper;
		Component::PrimaryArtifactMapper& artifactMapper;
		Component::RuleSink& ruleSink;

		FlavorAppender(const Component& component, const string& sourceDirectory, Language& language,
				const list<string>& sources, Component::BuildDirectoryMapper& directoryMapper,
				Component::PrimaryArtifactMapper& artifactMapper, Component::RuleSink& ruleSink)
				: component(component), sourceDirectory(sourceDirectory), language(language), sources(sources),
				directoryMapper(directoryMapper), artifactMapper(artifactMapper), ruleSink(ruleSink) {}

		FlavorAppender(const FlavorAppender& appender) : Appender<Flavor>(appender),
				component(appender.component), sourceDirectory(appender.sourceDirectory),
				language(appender.language), sources(appender.sources), directoryMapper(appender.directoryMapper),
				artifactMapper(appender.artifactMapper), ruleSink(appender.ruleSink) {}

		virtual void append(const Flavor& flavor) {
			string buildDirectory(Pathname::join(component.getBaseDirectory(),
					directoryMapper.getBuildDirectory(language, flavor)));
			string sdprefix(sourceDirectory);
			string::size_type sdlength = sourceDirectory.length();
			if(sdlength && sourceDirectory[sdlength - static_cast<string::size_type>(1u)] != '/') {
				sdprefix += '/';
				++sdlength;
			}
			list<string>::const_iterator sbegin(sources.begin()), send(sources.end());
			for(; sbegin != send; ++sbegin) {
				string srcbase;
				if(StringUtils::startsWith(*sbegin, sdprefix) && sbegin->length() > sdlength)
					srcbase = sbegin->substr(sdlength);
				else
					srcbase = *sbegin;
				string binbase(Pathname::join(Pathname::dirname(srcbase),
						language.getArtifactNameForSource(component.getType(), flavor,
						Pathname::basename(srcbase))));
				string binfile(Pathname::join(buildDirectory, binbase));
				ruleSink.setupSourceTransform(component, language, flavor, *sbegin, binfile);
			}
			string primary(Pathname::join(component.getBaseDirectory(),
					language.getPrimaryArtifactName(component.getType(), flavor,
					artifactMapper.getPrimaryArtifactName(component, flavor))));
			ruleSink.setupPrimaryArtifact(component, language, flavor, primary);
		}

	};

	void Component::setupRules(BuildDirectoryMapper& directoryMapper,
			PrimaryArtifactMapper& artifactMapper, RuleSink& ruleSink) const {
		PathIterator sdbegin(sourceDirectories.begin()), sdend(sourceDirectories.end());
		for(; sdbegin != sdend; ++sdbegin) {
			string srcdir(Pathname::join(baseDirectory, *sdbegin));
			if(!Filesystem::access(srcdir, Filesystem::FILE_EXISTS))
				continue;
			list<LanguageInfo> langinfo;
			LanguageIterator lbegin(languages.begin()), lend(languages.end());
			for(; lbegin != lend; ++lbegin)
				langinfo.push_back(LanguageInfo(**lbegin));
			SetupTraverser handler(langinfo);
			Filesystem::traverse(srcdir, handler);
			list<LanguageInfo>::iterator libegin(langinfo.begin()), liend(langinfo.end());
			for(; libegin != liend; ++libegin) {
				if(!libegin->sources.empty()) {
					FlavorAppender fhandler(*this, *sdbegin, libegin->language, libegin->sources, directoryMapper,
							artifactMapper, ruleSink);
					libegin->language.getSupportedFlavors(type, fhandler);
				}
				if(!libegin->headers.empty()) {
					//TODO
				}
			}
		}
	}

}}
