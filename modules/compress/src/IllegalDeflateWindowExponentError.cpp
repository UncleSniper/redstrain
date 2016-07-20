#include "IllegalDeflateWindowExponentError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace compress {

	IllegalDeflateWindowExponentError::IllegalDeflateWindowExponentError(MemorySize exponent)
			: exponent(exponent) {}

	IllegalDeflateWindowExponentError::IllegalDeflateWindowExponentError(const
			IllegalDeflateWindowExponentError& error)
			: Error(error), IllegalCompressionParameterError(error), exponent(error.exponent) {}

	REDSTRAIN_DEFINE_ERROR(IllegalDeflateWindowExponentError) {
		out << "Illegal window exponent for deflate compression: " << exponent;
	}

}}
