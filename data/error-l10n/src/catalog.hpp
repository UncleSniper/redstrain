#ifndef REDSTRAIN_DATA_ERROR_L10N_CATALOG_HPP
#define REDSTRAIN_DATA_ERROR_L10N_CATALOG_HPP

#include <redstrain/locale/ComponentMessageCatalog.hpp>
#include <redstrain/platform/PropertyMutexLockingPolicy.hpp>

#include "ErrorModuleMessageKey.hpp"
#include "api.hpp"

namespace redengine {
namespace error {
namespace l10n {

	typedef locale::ComponentMessageCatalog<
		text::Char16,
		ErrorModuleMessageKey,
		platform::PropertyMutexLockingPolicy
	> ErrorModuleMessageCatalog16;

	REDSTRAIN_ERROR_L10N_API ErrorModuleMessageCatalog16& getDefaultErrorModuleMessageCatalog16();
	REDSTRAIN_ERROR_L10N_API locale::MessageLoader<text::Char16>* newErrorModuleBlobMessageLoader16();

}}}

#endif /* REDSTRAIN_DATA_ERROR_L10N_CATALOG_HPP */
