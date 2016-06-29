#ifndef REDSTRAIN_MOD_ERROR_ERRORHANDLER_HPP
#define REDSTRAIN_MOD_ERROR_ERRORHANDLER_HPP

#include "api.hpp"

namespace redengine {
namespace error {

	class Error;

	class REDSTRAIN_ERROR_API ErrorHandler {

	  public:
		ErrorHandler();
		ErrorHandler(const ErrorHandler&);
		virtual ~ErrorHandler();

		void handleErrorSafely(const Error&);
		bool handleErrorCompletely(const Error&);

		virtual void handleError(const Error&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ERRORHANDLER_HPP */
