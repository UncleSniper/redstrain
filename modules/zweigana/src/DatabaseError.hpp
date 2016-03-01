#ifndef REDSTRAIN_MOD_ZWEIGANA_DATABASEERROR_HPP
#define REDSTRAIN_MOD_ZWEIGANA_DATABASEERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace zweigana {

	class REDSTRAIN_ZWEIGANA_API DatabaseError : public virtual error::Error {

	  public:
		DatabaseError();
		DatabaseError(const DatabaseError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(DatabaseError)

	};

}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_DATABASEERROR_HPP */
