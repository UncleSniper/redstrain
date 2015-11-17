#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "l10n.hpp"
#include "CoreMessageCatalog.hpp"

using redengine::util::Unref;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace locale {

	class DefaultCoreMessageCatalog16Singleton : public SynchronizedSingleton<CoreMessageCatalog16> {

	  protected:
		virtual CoreMessageCatalog16* newInstance();

	  public:
		DefaultCoreMessageCatalog16Singleton();
		DefaultCoreMessageCatalog16Singleton(const DefaultCoreMessageCatalog16Singleton&);

	};

	DefaultCoreMessageCatalog16Singleton::DefaultCoreMessageCatalog16Singleton() {}

	DefaultCoreMessageCatalog16Singleton::DefaultCoreMessageCatalog16Singleton(const
			DefaultCoreMessageCatalog16Singleton& singleton)
			: SynchronizedSingleton<CoreMessageCatalog16>(singleton) {}

	CoreMessageCatalog16* DefaultCoreMessageCatalog16Singleton::newInstance() {
		Delete<CoreMessageCatalog16> catalog(new CoreMessageCatalog16);
		Unref<MessageLoader<Char16> > loader(newCoreBlobMessageLoader16());
		if(*loader)
			catalog->addMessageLoader(**loader);
		return catalog.set();
	}

	static DefaultCoreMessageCatalog16Singleton defaultCoreMessageCatalog16;

	CoreMessageCatalog16& getDefaultCoreMessageCatalog16() {
		return defaultCoreMessageCatalog16.get();
	}

}}
