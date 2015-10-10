#include <redstrain/util/Unref.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeBlobAliasLanguage.hpp"
#include "../ComponentRuleBuilder.hpp"

using std::map;
using std::string;
using redengine::util::Unref;
using redengine::platform::Pathname;

namespace redengine {
namespace build {
namespace boot {

	XakeBlobAliasLanguage::XakeBlobAliasLanguage(const XakeProject& project) : project(project) {}

	XakeBlobAliasLanguage::XakeBlobAliasLanguage(const XakeBlobAliasLanguage& language)
			: BlobAliasLanguage(language), project(language.project) {}

	struct XakeBlobAliasTransformPropertyInjector : public Artifact::FollowupTransformPropertyInjector {

		XakeBlobAliasTransformPropertyInjector() {}

		virtual void injectFollowupTransformProperties(Component&, Language&,
				Artifact&, const Flavor&, const Flavor&, Transform&);

	};

	void XakeBlobAliasTransformPropertyInjector::injectFollowupTransformProperties(Component& component,
			Language& language, Artifact& sourceArtifact, const Flavor&, const Flavor&, Transform& transform) {
		FileArtifact* file = dynamic_cast<FileArtifact*>(&sourceArtifact);
		if(file)
			ComponentRuleBuilder::announceSourceReferencesHeader(*file, component, language, transform,
					Language::ReferencedHeader("redstrain/vfs/BlobVFS.hpp", false));
	}

	static XakeBlobAliasTransformPropertyInjector followupTransformPropertyInjector;

	Transform* XakeBlobAliasLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component) {
		Unref<Transform> transform(BlobAliasLanguage::getConversionTransform(sourceArtifact, sourceFlavor,
				targetArtifact, targetFlavor, transformFlavor, component));
		targetArtifact.addFollowupTransformPropertyInjector(followupTransformPropertyInjector);
		return transform.set();
	}

	BlobAliasLanguage::AliasConfiguration* XakeBlobAliasLanguage::getAliasConfiguration(FileArtifact& sourceArtifact,
			const Flavor&, FileArtifact&, const Flavor&, const Flavor&, Component& component) {
		map<string, string> variables, mvariables;
		variables["project"] = project.getProjectName();
		variables["module"] = component.getName();
		string sourceBasename(Pathname::basename(sourceArtifact.getPath()));
		string::size_type pos = sourceBasename.rfind('.');
		if(pos != string::npos && pos)
			sourceBasename = sourceBasename.substr(static_cast<string::size_type>(0u), pos);
		const XakeComponent* xcomponent = dynamic_cast<const XakeComponent*>(&component);
		variables["srchead"] = Pathname::stripSuffix(sourceArtifact.getPath(), sourceArtifact.getLabel());
		variables["srctail"] = sourceArtifact.getLabel();
		variables["srcbasename"] = Pathname::basename(sourceArtifact.getPath());
		variables["srcbasenamewithoutext"] = Pathname::basename(sourceArtifact.getPath());
		variables["srcdirname"] = Pathname::dirname(sourceArtifact.getPath());
		variables["srcpath"] = sourceArtifact.getPath();
		string pathPrefix, fileSuffix;
		if(xcomponent) {
			pathPrefix = XakeUtils::subst(xcomponent->getComponentConfiguration()
					.getProperty(Resources::RES_RSB_BLOB_ALIAS_PATH_PREFIX), variables);
			fileSuffix = XakeUtils::subst(xcomponent->getComponentConfiguration()
					.getProperty(Resources::RES_RSB_BLOB_ALIAS_FILE_SUFFIX), variables);
		}
		return new AliasConfiguration(pathPrefix, fileSuffix);
	}

}}}
