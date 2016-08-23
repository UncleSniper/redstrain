#ifndef REDSTRAIN_MOD_DAMNATION_TK_LAYERDEPTHOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LAYERDEPTHOUTOFBOUNDSERROR_HPP

#include <redstrain/error/IndexOutOfBoundsError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API LayerDepthOutOfBoundsError
			: public DamnError, public error::IndexOutOfBoundsError {

	  public:
		LayerDepthOutOfBoundsError(util::MemorySize);
		LayerDepthOutOfBoundsError(const LayerDepthOutOfBoundsError&);

		REDSTRAIN_DECLARE_ERROR(LayerDepthOutOfBoundsError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LAYERDEPTHOUTOFBOUNDSERROR_HPP */
