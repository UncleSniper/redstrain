#ifndef REDSTRAIN_MOD_ERROR_INDEXOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_ERROR_INDEXOUTOFBOUNDSERROR_HPP

#include "IllegalArgumentError.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IndexOutOfBoundsError : public IllegalArgumentError {

	  private:
		const size_t index;

	  public:
		IndexOutOfBoundsError(size_t);
		IndexOutOfBoundsError(const std::string&, size_t);
		IndexOutOfBoundsError(const IndexOutOfBoundsError&);

		inline size_t getOffendingIndex() const {
			return index;
		}

		REDSTRAIN_DECLARE_ERROR(IndexOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_INDEXOUTOFBOUNDSERROR_HPP */
