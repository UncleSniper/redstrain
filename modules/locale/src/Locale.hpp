#ifndef REDSTRAIN_MOD_LOCALE_LOCALE_HPP
#define REDSTRAIN_MOD_LOCALE_LOCALE_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API Locale {

	  private:
		const std::string language, country;

	  public:
		Locale(const std::string&, const std::string&);
		Locale(const Locale&);

		inline const std::string& getLanguage() const {
			return language;
		}

		inline const std::string& getCountry() const {
			return country;
		}

		std::string getName() const;

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_LOCALE_HPP */
