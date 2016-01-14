#include "IllegalPaddingCodeError.hpp"

namespace redengine {
namespace crypto {

	IllegalPaddingCodeError::IllegalPaddingCodeError() {}

	IllegalPaddingCodeError::IllegalPaddingCodeError(const IllegalPaddingCodeError& error)
			: CryptographyError(error) {}

	REDSTRAIN_DEFINE_ERROR(IllegalPaddingCodeError) {
		out << "Illegal padding code in message";
	}

}}
