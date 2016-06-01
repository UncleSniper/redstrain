#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "l10n.hpp"
#include "FormatMessageCatalog.hpp"

using redengine::util::Unref;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::locale::MessageLoader;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace calendar {

	class DefaultFormatMessageCatalog16Singleton : public SynchronizedSingleton<FormatMessageCatalog16> {

	  protected:
		virtual FormatMessageCatalog16* newInstance();

	  public:
		DefaultFormatMessageCatalog16Singleton();
		DefaultFormatMessageCatalog16Singleton(const DefaultFormatMessageCatalog16Singleton&);

	};

	DefaultFormatMessageCatalog16Singleton::DefaultFormatMessageCatalog16Singleton() {}

	DefaultFormatMessageCatalog16Singleton::DefaultFormatMessageCatalog16Singleton(const
			DefaultFormatMessageCatalog16Singleton& singleton)
			: SynchronizedSingleton<FormatMessageCatalog16>(singleton) {}

	FormatMessageCatalog16* DefaultFormatMessageCatalog16Singleton::newInstance() {
		Delete<FormatMessageCatalog16> catalog(new FormatMessageCatalog16);
		Unref<MessageLoader<Char16> > loader(newFormatBlobMessageLoader16());
		if(*loader)
			catalog->addMessageLoader(**loader);
		return catalog.set();
	}

	static DefaultFormatMessageCatalog16Singleton defaultFormatMessageCatalog16;

	REDSTRAIN_CALENDAR_API FormatMessageCatalog16& getDefaultFormatMessageCatalog16() {
		return defaultFormatMessageCatalog16.get();
	}

}}

