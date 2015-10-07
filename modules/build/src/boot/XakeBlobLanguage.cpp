#include <algorithm>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeBlobLanguage.hpp"

using std::map;
using std::string;
using std::transform;
using redengine::util::StringUtils;
using redengine::platform::Pathname;

namespace redengine {
namespace build {
namespace boot {

	XakeBlobLanguage::XakeBlobLanguage(const XakeProject& project) : project(project) {}

	XakeBlobLanguage::XakeBlobLanguage(const XakeBlobLanguage& language)
			: BlobLanguage(language), project(language.project) {}

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

	BlobLanguage::BlobConfiguration* XakeBlobLanguage::getBlobConfiguration(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor&,
			const Flavor& transformFlavor, Component& component) {
		map<string, string> variables, mvariables;
		variables["project"] = project.getProjectName();
		variables["module"] = component.getName();
		string sourceBasename(Pathname::basename(sourceArtifact.getPath()));
		string::size_type pos = sourceBasename.rfind('.');
		if(pos != string::npos && pos)
			sourceBasename = sourceBasename.substr(static_cast<string::size_type>(0u), pos);
		transform(sourceBasename.begin(), sourceBasename.end(), sourceBasename.begin(), slugify);
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
		mvariables["project"] = project.getProjectGuard();
		if(xcomponent)
			mvariables["module"] = xcomponent->getComponentGuard();
		else
			mvariables["module"] = XakeUtils::slugifyMacro(component.getName());
		string expmac;
		if(xcomponent)
			expmac = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_EXPORT_MACRO);
		if(expmac.empty())
			expmac = project.getProjectConfiguration().getProperty(Resources::RES_RSB_EXPORT_MACRO);
		if(!expmac.empty())
			expmac = XakeUtils::subst(expmac, mvariables);
		string bpath;
		if(xcomponent)
			bpath = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_BLOB_PATH);
		if(bpath.empty())
			bpath = project.getProjectConfiguration().getProperty(Resources::RES_RSB_BLOB_PATH);
		if(!bpath.empty()) {
			variables["srchead"] = Pathname::stripSuffix(sourceArtifact.getPath(), sourceArtifact.getLabel());
			variables["srctail"] = sourceArtifact.getLabel();
			variables["srcbasename"] = Pathname::basename(sourceArtifact.getPath());
			variables["srcdirname"] = Pathname::dirname(sourceArtifact.getPath());
			variables["srcpath"] = sourceArtifact.getPath();
			bpath = XakeUtils::subst(bpath, variables);
		}
		string xinclude;
		if(xcomponent)
			xinclude = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_BLOB_INCLUDE);
		if(xinclude.empty())
			xinclude = project.getProjectConfiguration().getProperty(Resources::RES_RSB_BLOB_INCLUDE);
		string gmac;
		if(xcomponent)
			gmac = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_GUARD_MACRO);
		if(gmac.empty())
			gmac = project.getProjectConfiguration().getProperty(Resources::RES_RSB_GUARD_MACRO);
		if(!gmac.empty()) {
			mvariables["file"] = XakeUtils::slugifyMacro(targetArtifact.getLabel());
			gmac = XakeUtils::subst(gmac, mvariables);
		}
		return new GenericBlobConfiguration(ns + sourceBasename, expmac, bpath, xinclude, gmac);
	}

}}}
