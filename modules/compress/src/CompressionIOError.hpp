#ifndef REDSTRAIN_MOD_COMPRESS_COMPRESSIONIOERROR_HPP
#define REDSTRAIN_MOD_COMPRESS_COMPRESSIONIOERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "CompressionError.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API CompressionIOError : public error::IOError {

	  private:
		CompressionError* cause;

	  public:
		CompressionIOError(CompressionError*);
		CompressionIOError(const CompressionIOError&);
		virtual ~CompressionIOError();

		inline CompressionError* getCause() const {
			return cause;
		}

		REDSTRAIN_DECLARE_ERROR(CompressionIOError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_COMPRESSIONIOERROR_HPP */
