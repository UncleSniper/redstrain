#ifndef REDSTRAIN_MOD_ALGORITHM_CREATEORMODIFY_HPP
#define REDSTRAIN_MOD_ALGORITHM_CREATEORMODIFY_HPP

namespace redengine {
namespace algorithm {

	template<typename ElementT>
	class CreateOrModify {

	  public:
		CreateOrModify() {}
		CreateOrModify(const CreateOrModify&) {}
		virtual ~CreateOrModify() {}

		virtual void createElement(ElementT&) = 0;
		virtual void modifyElement(ElementT&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_CREATEORMODIFY_HPP */
