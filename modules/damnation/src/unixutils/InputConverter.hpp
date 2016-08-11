#ifndef REDSTRAIN_MOD_DAMNATION_UNIX_INPUTCONVERTER_HPP
#define REDSTRAIN_MOD_DAMNATION_UNIX_INPUTCONVERTER_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "../KeySym.hpp"

namespace redengine {
namespace damnation {
namespace unixutils {

	class REDSTRAIN_DAMNATION_API InputConverter : public util::ReferenceCounted {

	  public:
		class REDSTRAIN_DAMNATION_API Stage {

		  public:
			Stage();
			Stage(const Stage&);
			virtual ~Stage();

			virtual KeySym getCurrentSymbol() = 0;
			virtual bool isInputLeaf() = 0;
			virtual bool advanceStage(char) = 0;

		};

	  public:
		InputConverter();
		InputConverter(const InputConverter&);

		virtual Stage* newSequence() = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_UNIX_INPUTCONVERTER_HPP */
