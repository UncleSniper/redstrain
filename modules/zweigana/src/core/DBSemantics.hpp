#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_DBSEMANTICS_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_DBSEMANTICS_HPP

#include <redstrain/util/types.hpp>

#include "../api.hpp"

namespace redengine {
namespace zweigana {
namespace core {

	class Database;

	class REDSTRAIN_ZWEIGANA_API DBSemantics {

	  public:
		DBSemantics();
		DBSemantics(const DBSemantics&);
		virtual ~DBSemantics();

		virtual void initDatabase(Database&) = 0;
		virtual void loadDatabase(Database&, util::FileSize) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_DBSEMANTICS_HPP */
