#include "IllegalByteOrderError.hpp"

using redengine::util::ByteOrder;

namespace redengine {
namespace error {

	IllegalByteOrderError::IllegalByteOrderError(ByteOrder byteOrder) : byteOrder(byteOrder) {}

	IllegalByteOrderError::IllegalByteOrderError(const IllegalByteOrderError& error)
			: Error(error), IllegalArgumentError(error), byteOrder(error.byteOrder) {}

	REDSTRAIN_DEFINE_ERROR(IllegalByteOrderError) {
		out << "Not a valid ByteOrder: " << static_cast<int>(byteOrder);
	}

}}
