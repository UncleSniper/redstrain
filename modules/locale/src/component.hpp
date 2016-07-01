#ifndef REDSTRAIN_MOD_LOCALE_COMPONENT_HPP
#define REDSTRAIN_MOD_LOCALE_COMPONENT_HPP

#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "BlobMessageLoader.hpp"

#define REDSTRAIN_DEFINE_DEFAULT_MESSAGE_CATALOG16(M_CatalogT, M_SingletonT, \
		m_messageLoaderConstructor, m_singletonObject, m_blobMappingObject, m_singletonFrontend) \
	class M_SingletonT : public ::redengine::platform::SynchronizedSingleton<M_CatalogT> { \
	  protected: \
		virtual M_CatalogT* newInstance(); \
	  public: \
		M_SingletonT(); \
		M_SingletonT(const M_SingletonT&); \
	}; \
	M_SingletonT::M_SingletonT() {} \
	M_SingletonT::M_SingletonT(const M_SingletonT& singleton) : SynchronizedSingleton<M_CatalogT>(singleton) {} \
	M_CatalogT* M_SingletonT::newInstance() { \
		::redengine::util::Delete<M_CatalogT> \
				catalog(new M_CatalogT(::redengine::locale::getDefaultCoreMessageCatalog16())); \
		::redengine::util::Unref< ::redengine::locale::MessageLoader< ::redengine::text::Char16> > \
				loader(m_messageLoaderConstructor()); \
		if(*loader) \
			catalog->addMessageLoader(**loader); \
		return catalog.set(); \
	} \
	static M_SingletonT m_singletonObject; \
	M_CatalogT& m_singletonFrontend() { \
		return m_singletonObject.get(); \
	} \
	::redengine::locale::BlobMessageMapping* m_blobMappingObject = NULL; \
	::redengine::locale::MessageLoader< ::redengine::text::Char16>* m_messageLoaderConstructor() { \
		if(!m_blobMappingObject) \
			return NULL; \
		return new ::redengine::locale::BlobMessageLoader< ::redengine::text::Char16>(*m_blobMappingObject); \
	}

#define REDSTRAIN_DEFINE_DEFAULT_MODULE_MESSAGE_CATALOG16(m_moduleName) \
	REDSTRAIN_DEFINE_DEFAULT_MESSAGE_CATALOG16( \
		m_moduleName ## ModuleMessageCatalog16, \
		Default ## m_moduleName ## ModuleMessageCatalog16Singleton, \
		new ## m_moduleName ## ModuleBlobMessageLoader16, \
		default ## m_moduleName ## ModuleMessageCatalog16, \
		messageBlobMapping, \
		getDefault ## m_moduleName ## ModuleMessageCatalog16 \
	)

#endif /* REDSTRAIN_MOD_LOCALE_COMPONENT_HPP */
