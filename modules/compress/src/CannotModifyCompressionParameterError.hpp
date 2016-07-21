#ifndef REDSTRAIN_MOD_COMPRESS_CANNOTMODIFYCOMPRESSIONPARAMETERERROR_HPP
#define REDSTRAIN_MOD_COMPRESS_CANNOTMODIFYCOMPRESSIONPARAMETERERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "CompressionError.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API CannotModifyCompressionParameterError
			: public CompressionError, public error::StateError {

	  public:
		CannotModifyCompressionParameterError();
		CannotModifyCompressionParameterError(const CannotModifyCompressionParameterError&);

		REDSTRAIN_DECLARE_ERROR(CannotModifyCompressionParameterError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_CANNOTMODIFYCOMPRESSIONPARAMETERERROR_HPP */
