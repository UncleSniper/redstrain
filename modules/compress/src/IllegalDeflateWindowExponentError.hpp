#ifndef REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEWINDOWEXPONENTERROR_HPP
#define REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEWINDOWEXPONENTERROR_HPP

#include <redstrain/util/types.hpp>

#include "IllegalCompressionParameterError.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API IllegalDeflateWindowExponentError : public IllegalCompressionParameterError {

	  private:
		const util::MemorySize exponent;

	  public:
		IllegalDeflateWindowExponentError(util::MemorySize);
		IllegalDeflateWindowExponentError(const IllegalDeflateWindowExponentError&);

		inline util::MemorySize getExponent() const {
			return exponent;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalDeflateWindowExponentError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEWINDOWEXPONENTERROR_HPP */
