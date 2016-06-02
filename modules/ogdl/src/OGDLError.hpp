#ifndef REDSTRAIN_MOD_OGDL_OGDLERROR_HPP
#define REDSTRAIN_MOD_OGDL_OGDLERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace ogdl {

	class REDSTRAIN_OGDL_API OGDLError : public virtual error::Error {

	  public:
		OGDLError();
		OGDLError(const OGDLError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(OGDLError)

	};

}}

#endif /* REDSTRAIN_MOD_OGDL_OGDLERROR_HPP */
