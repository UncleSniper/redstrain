#include "CompressionInputAfterEndError.hpp"

namespace redengine {
namespace compress {

	CompressionInputAfterEndError::CompressionInputAfterEndError() {}

	CompressionInputAfterEndError::CompressionInputAfterEndError(const CompressionInputAfterEndError& error)
			: Error(error), CompressionError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(CompressionInputAfterEndError) {
		out << "Input provided after announced end of compression/decompression";
	}

}}
