#include <set>
#include <redstrain/platform/MutexLocker.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "Locale.hpp"

using std::set;
using std::string;
using redengine::platform::Mutex;
using redengine::platform::MutexLocker;
using redengine::platform::DefaultSynchronizedSingleton;

namespace redengine {
namespace locale {

	Locale::Locale(const string& language, const string& country) : language(language), country(country) {}

	Locale::Locale(const Locale& locale) : language(locale.language), country(locale.country) {}

	string Locale::getName() const {
		if(language.empty() || country.empty())
			return "";
		return language + '_' + country;
	}

	bool Locale::operator==(const Locale& other) const {
		return language == other.language && country == other.country;
	}

	bool Locale::operator!=(const Locale& other) const {
		return language != other.language || country != other.country;
	}

	bool Locale::operator<(const Locale& other) const {
		if(language < other.language)
			return true;
		if(other.language < language)
			return false;
		return country < other.country;
	}

	struct LocaleInterner {

		Mutex mutex;
		set<Locale> locales;

	};

	static DefaultSynchronizedSingleton<LocaleInterner> interner;

	const Locale& Locale::intern() const {
		LocaleInterner& i = interner.get();
		MutexLocker locker(i.mutex);
		set<Locale>::iterator it = i.locales.find(*this);
		if(it != i.locales.end()) {
			const Locale& locale = *it;
			locker.release();
			return locale;
		}
		else {
			const Locale& locale = *i.locales.insert(*this).first;
			locker.release();
			return locale;
		}
	}

}}
