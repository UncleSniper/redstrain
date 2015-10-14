#include "Locale.hpp"

using std::string;

namespace redengine {
namespace locale {

	Locale::Locale(const string& language, const string& country) : language(language), country(country) {}

	Locale::Locale(const Locale& locale) : language(locale.language), country(locale.country) {}

	string Locale::getName() const {
		if(language.empty() || country.empty())
			return "";
		return language + '_' + country;
	}

}}
