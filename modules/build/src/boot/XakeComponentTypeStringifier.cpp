#include "XakeProject.hpp"
#include "XakeComponentTypeStringifier.hpp"
#include "../DefaultComponentTypeStringifier.hpp"

using std::string;

namespace redengine {
namespace build {
namespace boot {

	XakeComponentTypeStringifier::XakeComponentTypeStringifier(const XakeProject& project) : project(project) {}

	XakeComponentTypeStringifier::XakeComponentTypeStringifier(const XakeComponentTypeStringifier& stringifier)
			: ComponentTypeStringifier(stringifier), project(stringifier.project) {}

	string XakeComponentTypeStringifier::stringifyComponentType(Component::Type type) {
		switch(type) {
			#define clamp(c, r) \
				case Component::c: \
					return project.getProjectConfiguration().getProperty(Resources::r);
			clamp(LIBRARY, RES_MODULE_TAG)
			clamp(EXECUTABLE, RES_TOOL_TAG)
			clamp(DATA, RES_RSB_DATA_TAG)
			clamp(BLOB, RES_RSB_BLOB_TAG)
			#undef clamp
			default:
				return "";
		}
	}

}}}
