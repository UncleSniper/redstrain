#include <redstrain/redmond/blobs.hpp>

using redengine::redmond::hasBlobModule;

bool areAllBlobDependenciesPresent() {
	if(!hasBlobModule("RedStrain/Platform/L10N"))
		return false;
	if(!hasBlobModule("RedStrain/Error/L10N"))
		return false;
	if(!hasBlobModule("RedStrain/Text/Charsets"))
		return false;
	if(!hasBlobModule("RedStrain/CommandLine/L10N"))
		return false;
	if(!hasBlobModule("RedStrain/IO/L10N"))
		return false;
	if(!hasBlobModule("RedStrain/Locale/Core"))
		return false;
	if(!hasBlobModule("RedStrain/Error/StackTrace-L10N"))
		return false;
	return true;
}
