#ifndef REDSTRAIN_DATA_CALENDAR_L10N_CATALOG_HPP
#define REDSTRAIN_DATA_CALENDAR_L10N_CATALOG_HPP

#include <redstrain/locale/ComponentMessageCatalog.hpp>
#include <redstrain/platform/PropertyMutexLockingPolicy.hpp>

#include "CalendarModuleMessageKey.hpp"
#include "api.hpp"

namespace redengine {
namespace calendar {
namespace l10n {

	typedef locale::ComponentMessageCatalog<
		text::Char16,
		CalendarModuleMessageKey,
		platform::PropertyMutexLockingPolicy
	> CalendarModuleMessageCatalog16;

	REDSTRAIN_CALENDAR_L10N_API CalendarModuleMessageCatalog16& getDefaultCalendarModuleMessageCatalog16();
	REDSTRAIN_CALENDAR_L10N_API locale::MessageLoader<text::Char16>* newCalendarModuleBlobMessageLoader16();

}}}

#endif /* REDSTRAIN_DATA_CALENDAR_L10N_CATALOG_HPP */
