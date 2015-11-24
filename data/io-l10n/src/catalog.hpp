#ifndef REDSTRAIN_DATA_IO_L10N_CATALOG_HPP
#define REDSTRAIN_DATA_IO_L10N_CATALOG_HPP

#include <redstrain/locale/ComponentMessageCatalog.hpp>
#include <redstrain/platform/PropertyMutexLockingPolicy.hpp>

#include "IOModuleMessageKey.hpp"
#include "api.hpp"

namespace redengine {
namespace io {
namespace l10n {

	typedef locale::ComponentMessageCatalog<
		text::Char16,
		IOModuleMessageKey,
		platform::PropertyMutexLockingPolicy
	> IOModuleMessageCatalog16;

	REDSTRAIN_IO_L10N_API IOModuleMessageCatalog16& getDefaultIOModuleMessageCatalog16();
	REDSTRAIN_IO_L10N_API locale::MessageLoader<text::Char16>* newIOModuleBlobMessageLoader16();

}}}

#endif /* REDSTRAIN_DATA_IO_L10N_CATALOG_HPP */
