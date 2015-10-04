#include "DefaultComponentTypeStringifier.hpp"

using std::string;

namespace redengine {
namespace build {

	const char *const DefaultComponentTypeStringifier::DEFAULT_LIBRARY_TAG = "module";
	const char *const DefaultComponentTypeStringifier::DEFAULT_EXECUTABLE_TAG = "tool";
	const char *const DefaultComponentTypeStringifier::DEFAULT_DATA_TAG = "data";
	const char *const DefaultComponentTypeStringifier::DEFAULT_BLOB_TAG = "blob";

	DefaultComponentTypeStringifier::DefaultComponentTypeStringifier() {}

	DefaultComponentTypeStringifier::DefaultComponentTypeStringifier(const DefaultComponentTypeStringifier&
			stringifier) : ComponentTypeStringifier(stringifier), libraryTag(stringifier.libraryTag),
			executableTag(stringifier.executableTag), dataTag(stringifier.dataTag), blobTag(stringifier.blobTag) {}

	DefaultComponentTypeStringifier::~DefaultComponentTypeStringifier() {}

	void DefaultComponentTypeStringifier::setLibraryTag(const string& tag) {
		libraryTag = tag;
	}

	void DefaultComponentTypeStringifier::setExecutableTag(const string& tag) {
		executableTag = tag;
	}

	void DefaultComponentTypeStringifier::setDataTag(const string& tag) {
		dataTag = tag;
	}

	void DefaultComponentTypeStringifier::setBlobTag(const string& tag) {
		blobTag = tag;
	}

	string DefaultComponentTypeStringifier::stringifyComponentType(Component::Type type) {
		switch(type) {
			#define clamp(c, v) \
				case Component::c: \
					if(v ## Tag.empty()) \
						return DefaultComponentTypeStringifier::DEFAULT_ ## c ## _TAG; \
					else \
						return v ## Tag;
			clamp(LIBRARY, library)
			clamp(EXECUTABLE, executable)
			clamp(DATA, data)
			clamp(BLOB, blob)
			#undef clamp
			default:
				return "";
		}
	}

}}
