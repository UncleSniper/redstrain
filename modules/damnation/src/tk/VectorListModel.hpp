#ifndef REDSTRAIN_MOD_DAMNATION_TK_VECTORLISTMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_VECTORLISTMODEL_HPP

#include <vector>
#include <redstrain/util/destructors.hpp>

#include "MutableListModel.hpp"
#include "AbstractListModel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class VectorListModel : public AbstractListModel<ElementT>, public MutableListModel<ElementT> {

	  public:
		typedef std::vector<ElementT> Vector;

	  private:
		Vector vector;

	  public:
		VectorListModel() {}
		VectorListModel(const Vector& vector) : vector(vector) {}
		//TODO

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_VECTORLISTMODEL_HPP */
