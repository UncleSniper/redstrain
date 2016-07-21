#ifndef REDSTRAIN_MOD_COMPRESS_COMPRESSIONINPUTAFTERENDERROR_HPP
#define REDSTRAIN_MOD_COMPRESS_COMPRESSIONINPUTAFTERENDERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "CompressionError.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API CompressionInputAfterEndError : public CompressionError, public error::StateError {

	  public:
		CompressionInputAfterEndError();
		CompressionInputAfterEndError(const CompressionInputAfterEndError&);

		REDSTRAIN_DECLARE_ERROR(CompressionInputAfterEndError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_COMPRESSIONINPUTAFTERENDERROR_HPP */
