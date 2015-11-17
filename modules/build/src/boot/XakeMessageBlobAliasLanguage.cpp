#include <redstrain/util/Unref.hpp>
#include <redstrain/util/CPPUtils.hpp>
#include <redstrain/util/StringUtils.hpp>

#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeMessageBlobAliasLanguage.hpp"
#include "../ComponentRuleBuilder.hpp"

using std::map;
using std::string;
using redengine::util::Unref;
using redengine::util::CPPUtils;
using redengine::util::StringUtils;

namespace redengine {
namespace build {
namespace boot {

	XakeMessageBlobAliasLanguage::XakeMessageBlobAliasLanguage(const XakeProject& project) : project(project) {}

	XakeMessageBlobAliasLanguage::XakeMessageBlobAliasLanguage(const XakeMessageBlobAliasLanguage& language)
			: MessageBlobAliasLanguage(language), project(language.project) {}

	struct XakeMessageBlobAliasTransformPropertyInjector : public Artifact::FollowupTransformPropertyInjector {

		XakeMessageBlobAliasTransformPropertyInjector() {}

		virtual void injectFollowupTransformProperties(Component&, Language&,
				Artifact&, const Flavor&, const Flavor&, Transform&);

	};

	void XakeMessageBlobAliasTransformPropertyInjector::injectFollowupTransformProperties(Component& component,
			Language& language, Artifact& sourceArtifact, const Flavor&, const Flavor&, Transform& transform) {
		FileArtifact* file = dynamic_cast<FileArtifact*>(&sourceArtifact);
		if(file)
			ComponentRuleBuilder::announceSourceReferencesHeader(*file, component, language, transform,
					Language::ReferencedHeader("redstrain/locale/MessageBlobRegistrar.hpp", false));
	}

	static XakeMessageBlobAliasTransformPropertyInjector followupTransformPropertyInjector;

	Transform* XakeMessageBlobAliasLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component, BuildContext& context) {
		Unref<Transform> transform(MessageBlobAliasLanguage::getConversionTransform(sourceArtifact, sourceFlavor,
				targetArtifact, targetFlavor, transformFlavor, component, context));
		targetArtifact.addFollowupTransformPropertyInjector(followupTransformPropertyInjector);
		return transform.set();
	}

	MessageBlobAliasLanguage::AliasConfiguration& XakeMessageBlobAliasLanguage::getAliasConfiguration(
			FileArtifact& sourceArtifact, const Flavor&, FileArtifact&, const Flavor&, const Flavor&,
			Component& component) {
		// determine namespace
		map<string, string> variables;
		variables["project"] = CPPUtils::slugifySymbol(project.getProjectName());
		variables["module"] = CPPUtils::slugifySymbol(component.getName());
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
		// determine mapping symbol
		string mappingSymbol;
		if(xcomponent)
			mappingSymbol = xcomponent->getComponentConfiguration()
					.getProperty(Resources::RES_RSB_MESSAGE_BLOB_MAPPING);
		if(mappingSymbol.empty())
			mappingSymbol = ns + " messageBlobMapping";
		else
			mappingSymbol = XakeUtils::subst(mappingSymbol, variables);
		return *new AliasConfiguration(mappingSymbol, ns);
	}

}}}
