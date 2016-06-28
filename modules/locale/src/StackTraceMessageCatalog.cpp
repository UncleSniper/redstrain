#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "StackTraceMessageCatalog.hpp"
#include "l10n.hpp"

using redengine::util::Unref;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace locale {

	class DefaultStackTraceMessageCatalog16Singleton : public SynchronizedSingleton<StackTraceMessageCatalog16> {

	  protected:
		virtual StackTraceMessageCatalog16* newInstance();

	  public:
		DefaultStackTraceMessageCatalog16Singleton();
		DefaultStackTraceMessageCatalog16Singleton(const DefaultStackTraceMessageCatalog16Singleton&);

	};

	DefaultStackTraceMessageCatalog16Singleton::DefaultStackTraceMessageCatalog16Singleton() {}

	DefaultStackTraceMessageCatalog16Singleton::DefaultStackTraceMessageCatalog16Singleton(const
			DefaultStackTraceMessageCatalog16Singleton& singleton)
			: SynchronizedSingleton<StackTraceMessageCatalog16>(singleton) {}

	StackTraceMessageCatalog16* DefaultStackTraceMessageCatalog16Singleton::newInstance() {
		Delete<StackTraceMessageCatalog16> catalog(new StackTraceMessageCatalog16);
		Unref<MessageLoader<Char16> > loader(newStackTraceBlobMessageLoader16());
		if(*loader)
			catalog->addMessageLoader(**loader);
		return catalog.set();
	}

	static DefaultStackTraceMessageCatalog16Singleton defaultStackTraceMessageCatalog16;

	REDSTRAIN_LOCALE_API StackTraceMessageCatalog16& getDefaultStackTraceMessageCatalog16() {
		return defaultStackTraceMessageCatalog16.get();
	}

}}
