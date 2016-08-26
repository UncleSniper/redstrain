#ifndef REDSTRAIN_MOD_DAMNATION_TK_SEQUENCEMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_SEQUENCEMODEL_HPP

#include <redstrain/util/Appender.hpp>

#include "DamnModel.hpp"
#include "types.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class SequenceModel : public DamnModel {

	  public:
		typedef ElementT Element;

	  public:
		SequenceModel() {}
		SequenceModel(const SequenceModel& model) : DamnModel(model) {}

		virtual ModelElementIndex getElementCount() = 0;
		virtual ElementT& getElementAt(ModelElementIndex) = 0;
		virtual void getElements(util::Appender<ElementT>&) = 0;
		virtual void notifyElementModified(ModelElementIndex) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_SEQUENCEMODEL_HPP */
