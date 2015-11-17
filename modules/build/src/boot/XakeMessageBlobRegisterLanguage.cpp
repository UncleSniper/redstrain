#include <redstrain/util/Unref.hpp>
#include <redstrain/util/CPPUtils.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "../ComponentRuleBuilder.hpp"
#include "XakeMessageBlobRegisterLanguage.hpp"

using std::map;
using std::string;
using redengine::util::Unref;
using redengine::util::CPPUtils;
using redengine::util::StringUtils;
using redengine::platform::Pathname;

namespace redengine {
namespace build {
namespace boot {

	XakeMessageBlobRegisterLanguage::XakeMessageBlobRegisterLanguage(const XakeProject& project)
			: project(project) {}

	XakeMessageBlobRegisterLanguage::XakeMessageBlobRegisterLanguage(const XakeMessageBlobRegisterLanguage& language)
			: MessageBlobRegisterLanguage(language), project(language.project) {}

	struct XakeMessageBlobRegistrarTransformPropertyInjector : public Artifact::FollowupTransformPropertyInjector {

		XakeMessageBlobRegistrarTransformPropertyInjector() {}

		virtual void injectFollowupTransformProperties(Component&, Language&,
				Artifact&, const Flavor&, const Flavor&, Transform&);

	};

	void XakeMessageBlobRegistrarTransformPropertyInjector::injectFollowupTransformProperties(Component& component,
			Language& language, Artifact& sourceArtifact, const Flavor&, const Flavor&, Transform& transform) {
		FileArtifact* file = dynamic_cast<FileArtifact*>(&sourceArtifact);
		if(file)
			ComponentRuleBuilder::announceSourceReferencesHeader(*file, component, language, transform,
					Language::ReferencedHeader("redstrain/locale/MessageBlobRegistrar.hpp", false));
	}

	static XakeMessageBlobRegistrarTransformPropertyInjector followupTransformPropertyInjector;

	Transform* XakeMessageBlobRegisterLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component, BuildContext& context) {
		Unref<Transform> transform(MessageBlobRegisterLanguage::getConversionTransform(sourceArtifact, sourceFlavor,
				targetArtifact, targetFlavor, transformFlavor, component, context));
		targetArtifact.addFollowupTransformPropertyInjector(followupTransformPropertyInjector);
		return transform.set();
	}

	MessageBlobRegisterLanguage::RegistrarConfiguration& XakeMessageBlobRegisterLanguage::getRegistrarConfiguration(
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
		// determine locale name
		string sourceBasename(Pathname::basename(sourceArtifact.getPath()));
		string::size_type pos = sourceBasename.rfind('.');
		if(pos != string::npos && pos)
			sourceBasename = sourceBasename.substr(static_cast<string::size_type>(0u), pos);
		sourceBasename = CPPUtils::slugifySymbol(sourceBasename);
		// determine locale components
		string language, country;
		pos = sourceBasename.find('_');
		if(pos != string::npos) {
			language = sourceBasename.substr(static_cast<string::size_type>(0u), pos);
			country = sourceBasename.substr(pos + static_cast<string::size_type>(1u));
		}
		else
			language = sourceBasename;
		return *new RegistrarConfiguration(mappingSymbol, ns + sourceBasename, language, country);
	}

}}}
