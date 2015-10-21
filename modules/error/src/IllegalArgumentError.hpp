#ifndef REDSTRAIN_MOD_ERROR_ILLEGALARGUMENTERROR_HPP
#define REDSTRAIN_MOD_ERROR_ILLEGALARGUMENTERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IllegalArgumentError : public virtual Error {

	  private:
		const std::string message;

	  public:
		IllegalArgumentError(const std::string&);
		IllegalArgumentError(const IllegalArgumentError&);

		inline const std::string& getRawMessage() const {
			return message;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalArgumentError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ILLEGALARGUMENTERROR_HPP */
