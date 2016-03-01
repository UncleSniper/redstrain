#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_SHORTBLOCKERROR_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_SHORTBLOCKERROR_HPP

#include <redstrain/util/types.hpp>

#include "../DatabaseError.hpp"

namespace redengine {
namespace zweigana {
namespace core {

	class REDSTRAIN_ZWEIGANA_API ShortBlockError : public DatabaseError {

	  private:
		const util::FileSize blockOffset;
		const util::MemorySize blockSize;

	  public:
		ShortBlockError(util::FileSize, util::MemorySize);
		ShortBlockError(const ShortBlockError&);

		inline util::FileSize getRequestedBlockOffset() const {
			return blockOffset;
		}

		inline util::MemorySize getRequestedBlockSize() const {
			return blockSize;
		}

		REDSTRAIN_DECLARE_ERROR(ShortBlockError)

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_SHORTBLOCKERROR_HPP */
