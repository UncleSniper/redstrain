#include "CryptographyError.hpp"

namespace redengine {
namespace crypto {

	CryptographyError::CryptographyError() {}

	CryptographyError::CryptographyError(const CryptographyError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(CryptographyError)

}}
