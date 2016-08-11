#include "SequenceMapInputConverter.hpp"

namespace redengine {
namespace damnation {
namespace unixutils {

	// ======== NodeStage ========

	SequenceMapInputConverter::NodeStage::NodeStage(Node& node) : node(&node) {}

	SequenceMapInputConverter::NodeStage::NodeStage(const NodeStage& stage) : Stage(stage), node(stage.node) {}

	KeySym SequenceMapInputConverter::NodeStage::getCurrentSymbol() {
		return node->hasValue() ? node->getValue() : KeySym::NONE;
	}

	bool SequenceMapInputConverter::NodeStage::isInputLeaf() {
		return node->isLeaf();
	}

	bool SequenceMapInputConverter::NodeStage::advanceStage(char inputByte) {
		Node* next = node->get(inputByte);
		if(!next)
			return false;
		node = next;
		return true;
	}

	// ======== SequenceMapInputConverter ========

	SequenceMapInputConverter::SequenceMapInputConverter(Map& map) : map(map) {}

	SequenceMapInputConverter::SequenceMapInputConverter(const SequenceMapInputConverter& converter)
			: InputConverter(converter), map(converter.map) {}

	InputConverter::Stage* SequenceMapInputConverter::newSequence() {
		return new NodeStage(map.getRoot());
	}

}}}
