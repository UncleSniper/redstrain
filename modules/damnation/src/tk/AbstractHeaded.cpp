#include "AbstractHeaded.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	AbstractHeaded::AbstractHeaded(bool inlineHeader) : inlineHeader(inlineHeader), externalCount(0u) {}

	AbstractHeaded::AbstractHeaded(const AbstractHeaded& headed)
			: Widget(headed), Headed(headed), inlineHeader(headed.inlineHeader), externalCount(0u) {}

	void AbstractHeaded::setInlineHeader(bool inlineHeader) {
		if(!inlineHeader == !this->inlineHeader)
			return;
		this->inlineHeader = inlineHeader;
		notifyHeaderInlinenessChanged();
	}

	bool AbstractHeaded::isInlineHeader() {
		return inlineHeader;
	}

	bool AbstractHeaded::isHeaderExternallyDrawn() {
		return !!externalCount;
	}

	void AbstractHeaded::setHeaderExternallyDrawn(bool drawn) {
		if(drawn)
			++externalCount;
		else if(externalCount)
			--externalCount;
	}

}}}
