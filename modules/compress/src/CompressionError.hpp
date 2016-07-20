#ifndef REDSTRAIN_MOD_COMPRESS_COMPRESSIONERROR_HPP
#define REDSTRAIN_MOD_COMPRESS_COMPRESSIONERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API CompressionError : public virtual error::Error {

	  public:
		CompressionError();
		CompressionError(const CompressionError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(CompressionError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_COMPRESSIONERROR_HPP */
