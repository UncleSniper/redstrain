#ifndef REDSTRAIN_DATA_PLATFORM_L10N_CATALOG_HPP
#define REDSTRAIN_DATA_PLATFORM_L10N_CATALOG_HPP

#include <redstrain/locale/ComponentMessageCatalog.hpp>
#include <redstrain/platform/PropertyMutexLockingPolicy.hpp>

#include "PlatformModuleMessageKey.hpp"
#include "api.hpp"

namespace redengine {
namespace platform {
namespace l10n {

	typedef locale::ComponentMessageCatalog<
		text::Char16,
		PlatformModuleMessageKey,
		platform::PropertyMutexLockingPolicy
	> PlatformModuleMessageCatalog16;

	REDSTRAIN_PLATFORM_L10N_API PlatformModuleMessageCatalog16& getDefaultPlatformModuleMessageCatalog16();
	REDSTRAIN_PLATFORM_L10N_API locale::MessageLoader<text::Char16>* newPlatformModuleBlobMessageLoader16();

}}}

#endif /* REDSTRAIN_DATA_PLATFORM_L10N_CATALOG_HPP */
