#ifndef REDSTRAIN_MOD_ERROR_PROGRAMMINGERROR_HPP
#define REDSTRAIN_MOD_ERROR_PROGRAMMINGERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API ProgrammingError : public Error {

	  private:
		const std::string message;

	  public:
		ProgrammingError(const std::string&);
		ProgrammingError(const ProgrammingError&);

		inline const std::string& getRawMessage() const {
			return message;
		}

		REDSTRAIN_DECLARE_ERROR(ProgrammingError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_PROGRAMMINGERROR_HPP */
