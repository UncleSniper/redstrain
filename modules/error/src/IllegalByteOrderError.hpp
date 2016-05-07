#ifndef REDSTRAIN_MOD_ERROR_ILLEGALBYTEORDERERROR_HPP
#define REDSTRAIN_MOD_ERROR_ILLEGALBYTEORDERERROR_HPP

#include <redstrain/util/ByteOrder.hpp>

#include "IllegalArgumentError.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IllegalByteOrderError : public IllegalArgumentError {

	  private:
		const util::ByteOrder byteOrder;

	  public:
		IllegalByteOrderError(util::ByteOrder);
		IllegalByteOrderError(const IllegalByteOrderError&);

		inline util::ByteOrder getByteOrder() const {
			return byteOrder;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalByteOrderError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ILLEGALBYTEORDERERROR_HPP */
