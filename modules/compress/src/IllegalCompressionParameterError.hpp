#ifndef REDSTRAIN_MOD_COMPRESS_ILLEGALCOMPRESSIONPARAMETERERROR_HPP
#define REDSTRAIN_MOD_COMPRESS_ILLEGALCOMPRESSIONPARAMETERERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "CompressionError.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API IllegalCompressionParameterError
			: public CompressionError, public error::IllegalArgumentError {

	  public:
		IllegalCompressionParameterError();
		IllegalCompressionParameterError(const IllegalCompressionParameterError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(IllegalCompressionParameterError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_ILLEGALCOMPRESSIONPARAMETERERROR_HPP */
