#ifndef REDSTRAIN_MOD_ERROR_NOERRORLOCALIZERREGISTEREDERROR_HPP
#define REDSTRAIN_MOD_ERROR_NOERRORLOCALIZERREGISTEREDERROR_HPP

#include "StateError.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API NoErrorLocalizerRegisteredError : public StateError {

	  private:
		const std::string exceptionClass;

	  public:
		NoErrorLocalizerRegisteredError(const std::string&);
		NoErrorLocalizerRegisteredError(const NoErrorLocalizerRegisteredError&);
		virtual ~NoErrorLocalizerRegisteredError();

		inline const std::string& getExceptionClassName() const {
			return exceptionClass;
		}

		REDSTRAIN_DECLARE_ERROR(NoErrorLocalizerRegisteredError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_NOERRORLOCALIZERREGISTEREDERROR_HPP */
