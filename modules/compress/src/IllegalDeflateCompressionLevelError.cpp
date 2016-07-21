#include "IllegalDeflateCompressionLevelError.hpp"

namespace redengine {
namespace compress {

	IllegalDeflateCompressionLevelError::IllegalDeflateCompressionLevelError(unsigned level) : level(level) {}

	IllegalDeflateCompressionLevelError::IllegalDeflateCompressionLevelError(const
			IllegalDeflateCompressionLevelError& error)
			: Error(error), IllegalCompressionParameterError(error), level(error.level) {}

	REDSTRAIN_DEFINE_ERROR(IllegalDeflateCompressionLevelError) {
		out << "Illegal compression level for deflate compression: " << level;
	}

}}
