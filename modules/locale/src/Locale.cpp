#include <set>
#include <redstrain/text/l10n.hpp>
#include <redstrain/platform/MutexLocker.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "Locale.hpp"

using std::set;
using std::string;
using redengine::platform::Mutex;
using redengine::platform::MutexLocker;
using redengine::platform::SynchronizedSingleton;
using redengine::platform::DefaultSynchronizedSingleton;
using redengine::text::guessSystemLocale;

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

	class SystemLocaleSingleton : public SynchronizedSingleton<const Locale> {

	  protected:
		virtual const Locale* newInstance();

	  public:
		SystemLocaleSingleton();
		SystemLocaleSingleton(const SystemLocaleSingleton&);

	};

	SystemLocaleSingleton::SystemLocaleSingleton() {}

	SystemLocaleSingleton::SystemLocaleSingleton(const SystemLocaleSingleton& singleton)
			: SynchronizedSingleton<const Locale>(singleton) {}

	const Locale* SystemLocaleSingleton::newInstance() {
		string spec(guessSystemLocale());
		string language, country;
		string::size_type pos = spec.find('_');
		if(pos == string::npos)
			language = spec;
		else {
			language = spec.substr(static_cast<string::size_type>(0u), pos);
			country = spec.substr(static_cast<string::size_type>(pos + static_cast<string::size_type>(1u)));
		}
		Locale locale(language, country);
		return &locale.intern();
	}

	static SystemLocaleSingleton systemLocaleSingleton;

	const Locale& Locale::getSystemLocale() {
		return systemLocaleSingleton.get();
	}

}}
