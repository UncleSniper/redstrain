#ifndef REDSTRAIN_MOD_ERROR_IOERROR_HPP
#define REDSTRAIN_MOD_ERROR_IOERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IOError : public virtual Error {

	  public:
		IOError();
		IOError(const IOError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(IOError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_IOERROR_HPP */
