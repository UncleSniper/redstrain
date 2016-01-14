#ifndef REDSTRAIN_MOD_ERROR_INDEXOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_ERROR_INDEXOUTOFBOUNDSERROR_HPP

#include <redstrain/util/types.hpp>

#include "IllegalArgumentError.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IndexOutOfBoundsError : public IllegalArgumentError {

	  private:
		const util::MemorySize index;

	  public:
		IndexOutOfBoundsError(util::MemorySize);
		IndexOutOfBoundsError(const IndexOutOfBoundsError&);

		inline util::MemorySize getOffendingIndex() const {
			return index;
		}

		REDSTRAIN_DECLARE_ERROR(IndexOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_INDEXOUTOFBOUNDSERROR_HPP */
