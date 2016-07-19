#ifndef REDSTRAIN_DATA_CMDLINE_L10N_CATALOG_HPP
#define REDSTRAIN_DATA_CMDLINE_L10N_CATALOG_HPP

#include <redstrain/locale/ComponentMessageCatalog.hpp>
#include <redstrain/platform/PropertyMutexLockingPolicy.hpp>

#include "CommandLineModuleMessageKey.hpp"
#include "api.hpp"

namespace redengine {
namespace cmdline {
namespace l10n {

	typedef locale::ComponentMessageCatalog<
		text::Char16,
		CommandLineModuleMessageKey,
		platform::PropertyMutexLockingPolicy
	> CommandLineModuleMessageCatalog16;

	REDSTRAIN_CMDLINE_L10N_API CommandLineModuleMessageCatalog16& getDefaultCommandLineModuleMessageCatalog16();
	REDSTRAIN_CMDLINE_L10N_API locale::MessageLoader<text::Char16>* newCommandLineModuleBlobMessageLoader16();

}}}

#endif /* REDSTRAIN_DATA_CMDLINE_L10N_CATALOG_HPP */
