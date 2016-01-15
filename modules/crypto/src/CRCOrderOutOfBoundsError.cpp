#include "CRCOrderOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	CRCOrderOutOfBoundsError::CRCOrderOutOfBoundsError(MemorySize order) : order(order) {}

	CRCOrderOutOfBoundsError::CRCOrderOutOfBoundsError(const CRCOrderOutOfBoundsError& error)
			: IllegalArgumentError(error), order(error.order) {}

	REDSTRAIN_DEFINE_ERROR(CRCOrderOutOfBoundsError) {
		out << "Unsupported CRC polynomial order: " << order;
	}

}}
