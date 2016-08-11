#ifndef REDSTRAIN_MOD_DAMNATION_UNIXUTILS_SEQUENCEMAPINPUTCONVERTER_HPP
#define REDSTRAIN_MOD_DAMNATION_UNIXUTILS_SEQUENCEMAPINPUTCONVERTER_HPP

#include <redstrain/algorithm/SequenceMap.hpp>

#include "InputConverter.hpp"

namespace redengine {
namespace damnation {
namespace unixutils {

	class REDSTRAIN_DAMNATION_API SequenceMapInputConverter : public InputConverter {

	  public:
		typedef algorithm::SequenceMap<char, KeySym> Map;
		typedef Map::Node Node;

	  private:
		class REDSTRAIN_DAMNATION_API NodeStage : public Stage {

		  private:
			Node* node;

		  public:
			NodeStage(Node&);
			NodeStage(const NodeStage&);

			virtual KeySym getCurrentSymbol();
			virtual bool isInputLeaf();
			virtual bool advanceStage(char);

		};

	  private:
		Map& map;

	  public:
		SequenceMapInputConverter(Map&);
		SequenceMapInputConverter(const SequenceMapInputConverter&);

		inline Map& getSequenceMap() {
			return map;
		}

		inline const Map& getSequenceMap() const {
			return map;
		}

		virtual Stage* newSequence();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_UNIXUTILS_SEQUENCEMAPINPUTCONVERTER_HPP */
