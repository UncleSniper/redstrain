#ifndef REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEHASHEXPONENTERROR_HPP
#define REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEHASHEXPONENTERROR_HPP

#include <redstrain/util/types.hpp>

#include "IllegalCompressionParameterError.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API IllegalDeflateHashExponentError : public IllegalCompressionParameterError {

	  private:
		const util::MemorySize exponent;

	  public:
		IllegalDeflateHashExponentError(util::MemorySize);
		IllegalDeflateHashExponentError(const IllegalDeflateHashExponentError&);

		inline util::MemorySize getExponent() const {
			return exponent;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalDeflateHashExponentError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEHASHEXPONENTERROR_HPP */
