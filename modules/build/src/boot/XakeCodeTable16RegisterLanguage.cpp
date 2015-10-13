#include <algorithm>
#include <redstrain/util/Unref.hpp>
#include <redstrain/util/StringUtils.hpp>

#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "../ComponentRuleBuilder.hpp"
#include "XakeCodeTable16RegisterLanguage.hpp"

using std::map;
using std::string;
using std::transform;
using redengine::util::Unref;
using redengine::util::StringUtils;
using redengine::text::BlobCodeTable16Registrar;

namespace redengine {
namespace build {
namespace boot {

	// ======== XakeRegistrarConfiguration ========

	XakeCodeTable16RegisterLanguage::XakeRegistrarConfiguration::XakeRegistrarConfiguration(const string& nsPrefix)
			: nsPrefix(nsPrefix) {}

	XakeCodeTable16RegisterLanguage::XakeRegistrarConfiguration::XakeRegistrarConfiguration(const
			XakeRegistrarConfiguration& configuration)
			: RegistrarConfiguration(configuration), BlobSymbolMapper(configuration),
			nsPrefix(configuration.nsPrefix) {}

	XakeCodeTable16RegisterLanguage::XakeRegistrarConfiguration::~XakeRegistrarConfiguration() {}

	void XakeCodeTable16RegisterLanguage::XakeRegistrarConfiguration::applyConfiguration(
			BlobCodeTable16Registrar::GeneratorReader&) {}

	void XakeCodeTable16RegisterLanguage::XakeRegistrarConfiguration::applyConfiguration(
			BlobCodeTable16Registrar::GeneratorWriter& writer) {
		writer.setBlobSymbolMapper(this);
	}

	static char slugify(char c) {
		if(
			(c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')
			|| (c >= '0' && c <= '9')
		)
			return c;
		else
			return '_';
	}

	string XakeCodeTable16RegisterLanguage::XakeRegistrarConfiguration::mapBlobSymbol(const string& symbol) {
		string s(symbol);
		transform(s.begin(), s.end(), s.begin(), slugify);
		return nsPrefix + s;
	}

	// ======== XakeCodeTable16RegisterLanguage ========

	XakeCodeTable16RegisterLanguage::XakeCodeTable16RegisterLanguage(const XakeProject& project)
			: project(project) {}

	XakeCodeTable16RegisterLanguage::XakeCodeTable16RegisterLanguage(const XakeCodeTable16RegisterLanguage& language)
			: CodeTable16RegisterLanguage(language), project(language.project) {}

	struct XakeCodeTable16RegistrarTransformPropertyInjector : public Artifact::FollowupTransformPropertyInjector {

		XakeCodeTable16RegistrarTransformPropertyInjector() {}

		virtual void injectFollowupTransformProperties(Component&, Language&,
				Artifact&, const Flavor&, const Flavor&, Transform&);

	};

	void XakeCodeTable16RegistrarTransformPropertyInjector::injectFollowupTransformProperties(Component& component,
			Language& language, Artifact& sourceArtifact, const Flavor&, const Flavor&, Transform& transform) {
		FileArtifact* file = dynamic_cast<FileArtifact*>(&sourceArtifact);
		if(file)
			ComponentRuleBuilder::announceSourceReferencesHeader(*file, component, language, transform,
					Language::ReferencedHeader("redstrain/text/BlobCodeTable16Registrar.hpp", false));
	}

	static XakeCodeTable16RegistrarTransformPropertyInjector followupTransformPropertyInjector;

	Transform* XakeCodeTable16RegisterLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component) {
		Unref<Transform> transform(CodeTable16RegisterLanguage::getConversionTransform(sourceArtifact, sourceFlavor,
				targetArtifact, targetFlavor, transformFlavor, component));
		targetArtifact.addFollowupTransformPropertyInjector(followupTransformPropertyInjector);
		return transform.set();
	}

	CodeTable16RegisterLanguage::RegistrarConfiguration* XakeCodeTable16RegisterLanguage::getRegistrarConfiguration(
			FileArtifact& sourceArtifact, const Flavor&, FileArtifact&, const Flavor&, const Flavor&,
			Component& component) {
		map<string, string> variables, mvariables;
		variables["project"] = project.getProjectName();
		variables["module"] = component.getName();
		string ns;
		const XakeComponent* xcomponent = dynamic_cast<const XakeComponent*>(&component);
		if(xcomponent)
			ns = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_GENERATED_NAMESPACE);
		if(ns.empty())
			ns = project.getProjectConfiguration().getProperty(Resources::RES_RSB_GENERATED_NAMESPACE);
		if(!ns.empty()) {
			ns = XakeUtils::subst(ns, variables);
			if(!StringUtils::endsWith(ns, "::"))
				ns += "::";
		}
		return new XakeRegistrarConfiguration(ns);
	}

}}}
