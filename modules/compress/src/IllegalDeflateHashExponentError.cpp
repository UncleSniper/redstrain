#include "IllegalDeflateHashExponentError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace compress {

	IllegalDeflateHashExponentError::IllegalDeflateHashExponentError(MemorySize exponent)
			: exponent(exponent) {}

	IllegalDeflateHashExponentError::IllegalDeflateHashExponentError(const
			IllegalDeflateHashExponentError& error)
			: Error(error), IllegalCompressionParameterError(error), exponent(error.exponent) {}

	REDSTRAIN_DEFINE_ERROR(IllegalDeflateHashExponentError) {
		out << "Illegal hash exponent for deflate compression: " << exponent;
	}

}}
