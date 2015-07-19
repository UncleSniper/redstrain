#include "Language.hpp"

using std::string;

namespace redengine {
namespace build {

	Language::Language(const string& name) : name(name) {}

	Language::Language(const Language& language) : name(language.name) {}

	Language::~Language() {}

}}
