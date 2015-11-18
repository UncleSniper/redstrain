#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/locale/BlobMessageLoader.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "catalog.hpp"

using redengine::util::Unref;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::locale::MessageLoader;
using redengine::locale::BlobMessageLoader;
using redengine::locale::BlobMessageMapping;
using redengine::platform::SynchronizedSingleton;
using redengine::locale::getDefaultCoreMessageCatalog16;

namespace redengine {
namespace error {
namespace l10n {

	class DefaultErrorModuleMessageCatalog16Singleton : public SynchronizedSingleton<ErrorModuleMessageCatalog16> {

	  protected:
		virtual ErrorModuleMessageCatalog16* newInstance();

	  public:
		DefaultErrorModuleMessageCatalog16Singleton();
		DefaultErrorModuleMessageCatalog16Singleton(const DefaultErrorModuleMessageCatalog16Singleton&);

	};

	DefaultErrorModuleMessageCatalog16Singleton::DefaultErrorModuleMessageCatalog16Singleton() {}

	DefaultErrorModuleMessageCatalog16Singleton::DefaultErrorModuleMessageCatalog16Singleton(const
			DefaultErrorModuleMessageCatalog16Singleton& singleton)
			: SynchronizedSingleton<ErrorModuleMessageCatalog16>(singleton) {}

	ErrorModuleMessageCatalog16* DefaultErrorModuleMessageCatalog16Singleton::newInstance() {
		Delete<ErrorModuleMessageCatalog16>
				catalog(new ErrorModuleMessageCatalog16(getDefaultCoreMessageCatalog16()));
		Unref<MessageLoader<Char16> > loader(newErrorModuleBlobMessageLoader16());
		if(*loader)
			catalog->addMessageLoader(**loader);
		return catalog.set();
	}

	static DefaultErrorModuleMessageCatalog16Singleton defaultErrorModuleMessageCatalog16;

	ErrorModuleMessageCatalog16& getDefaultErrorModuleMessageCatalog16() {
		return defaultErrorModuleMessageCatalog16.get();
	}

	BlobMessageMapping* TODO = NULL;

	MessageLoader<Char16>* newErrorModuleBlobMessageLoader16() {
		if(!TODO)
			return NULL;
		return new BlobMessageLoader<Char16>(*TODO);
	}

}}}
