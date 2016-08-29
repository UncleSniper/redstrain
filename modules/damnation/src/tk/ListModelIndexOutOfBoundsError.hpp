#ifndef REDSTRAIN_MOD_DAMNATION_TK_LISTMODELINDEXOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LISTMODELINDEXOUTOFBOUNDSERROR_HPP

#include <redstrain/error/IndexOutOfBoundsError.hpp>

#include "types.hpp"
#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API ListModelIndexOutOfBoundsError
			: public DamnError, public error::IndexOutOfBoundsError {

	  private:
		const ModelElementIndex index;

	  public:
		ListModelIndexOutOfBoundsError(ModelElementIndex);
		ListModelIndexOutOfBoundsError(const ListModelIndexOutOfBoundsError&);

		inline ModelElementIndex getListIndex() const {
			return index;
		}

		REDSTRAIN_DECLARE_ERROR(ListModelIndexOutOfBoundsError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LISTMODELINDEXOUTOFBOUNDSERROR_HPP */
