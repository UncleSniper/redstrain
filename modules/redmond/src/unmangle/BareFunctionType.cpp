#include "Type.hpp"
#include "BareFunctionType.hpp"
#include "../unmangle-utils.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	BareFunctionType::BareFunctionType() {}

	BareFunctionType::BareFunctionType(const BareFunctionType& bare) {
		UnmangleCollectionPtr<Types> clearTypes(&types);
		TypeIterator begin(bare.types.begin()), end(bare.types.end());
		for(; begin != end; ++begin)
			types.push_back((*begin)->cloneType());
		clearTypes.ptr = NULL;
	}

	BareFunctionType::~BareFunctionType() {
		TypeIterator begin(types.begin()), end(types.end());
		for(; begin != end; ++begin)
			delete *begin;
	}

	unsigned BareFunctionType::getTypeCount() const {
		return static_cast<unsigned>(types.size());
	}

	void BareFunctionType::getTypes(TypeIterator& begin, TypeIterator& end) const {
		begin = types.begin();
		end = types.end();
	}

	Type* BareFunctionType::getFirstType() const {
		return types.empty() ? NULL : types.front();
	}

	void BareFunctionType::getRestTypes(TypeIterator& begin, TypeIterator& end) const {
		begin = types.begin();
		end = types.end();
		if(begin != end)
			++begin;
	}

	void BareFunctionType::addType(Type& type) {
		types.push_back(&type);
	}

}}}
