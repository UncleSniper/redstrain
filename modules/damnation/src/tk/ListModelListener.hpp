#ifndef REDSTRAIN_MOD_DAMNATION_TK_LISTMODELLISTENER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LISTMODELLISTENER_HPP

#include "SequenceModelListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class ListModelListener : public SequenceModelListener<ElementT> {

	  public:
		ListModelListener() {}
		ListModelListener(const ListModelListener& listener) : SequenceModelListener<ElementT>(listener) {}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LISTMODELLISTENER_HPP */
