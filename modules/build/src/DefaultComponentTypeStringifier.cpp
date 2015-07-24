#include "DefaultComponentTypeStringifier.hpp"

using std::string;

namespace redengine {
namespace build {

	DefaultComponentTypeStringifier DefaultComponentTypeStringifier::instance;

	DefaultComponentTypeStringifier::DefaultComponentTypeStringifier() {}

	DefaultComponentTypeStringifier::DefaultComponentTypeStringifier(const DefaultComponentTypeStringifier&
			stringifier) : ComponentTypeStringifier(stringifier) {}

	string DefaultComponentTypeStringifier::stringifyComponentType(Component::Type type) {
		switch(type) {
			case Component::LIBRARY:
				return "module";
			case Component::EXECUTABLE:
				return "tool";
			case Component::DATA:
				return "data";
			case Component::BLOB:
				return "blob";
			default:
				return "component";
		}
	}

}}
