#ifndef REDSTRAIN_MOD_ERROR_UNSUPPORTEDOPERATIONERROR_HPP
#define REDSTRAIN_MOD_ERROR_UNSUPPORTEDOPERATIONERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API UnsupportedOperationError : public Error {

	  private:
		const std::string operation;

	  public:
		UnsupportedOperationError(const std::string&);
		UnsupportedOperationError(const UnsupportedOperationError&);
		virtual ~UnsupportedOperationError();

		inline const std::string& getOperation() const {
			return operation;
		}

		REDSTRAIN_DECLARE_ERROR(UnsupportedOperationError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_UNSUPPORTEDOPERATIONERROR_HPP */
