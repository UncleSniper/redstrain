#ifndef REDSTRAIN_MOD_ERROR_ERRORHANDLINGERROR_HPP
#define REDSTRAIN_MOD_ERROR_ERRORHANDLINGERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API ErrorHandlingError : public Error {

	  private:
		Error *const originalError;
		Error *const subsequentError;

	  public:
		ErrorHandlingError(Error*, Error*);
		ErrorHandlingError(const ErrorHandlingError&);
		virtual ~ErrorHandlingError();

		inline Error* getOriginalError() const {
			return originalError;
		}

		inline Error* getSubsequentError() const {
			return subsequentError;
		}

		REDSTRAIN_DECLARE_ERROR(ErrorHandlingError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ERRORHANDLINGERROR_HPP */
