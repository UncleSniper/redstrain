#ifndef REDSTRAIN_MOD_DAMNATION_UNIXUTILS_UNIXTERMINALBINDINGINPUTCONVERTER_HPP
#define REDSTRAIN_MOD_DAMNATION_UNIXUTILS_UNIXTERMINALBINDINGINPUTCONVERTER_HPP

#include "InputConverter.hpp"
#include "../UNIXTerminalBinding.hpp"

namespace redengine {
namespace damnation {
namespace unixutils {

	class REDSTRAIN_DAMNATION_API UNIXTerminalBindingInputConverter : public InputConverter {

	  private:
		class REDSTRAIN_DAMNATION_API DecoderStage : public Stage {

		  private:
			text::Decoder16& decoder;
			KeySym currentSymbol;
			bool failed;

		  private:
			DecoderStage(const DecoderStage&);

			void setCurrentSymbolGeneric(text::Char16);

		  public:
			DecoderStage(text::Decoder16&);
			virtual ~DecoderStage();

			virtual KeySym getCurrentSymbol();
			virtual bool isInputLeaf();
			virtual bool advanceStage(char);

		};

	  private:
		const UNIXTerminalBinding& terminal;

	  public:
		UNIXTerminalBindingInputConverter(const UNIXTerminalBinding&);
		UNIXTerminalBindingInputConverter(const UNIXTerminalBindingInputConverter&);

		inline const UNIXTerminalBinding& getTerminal() const {
			return terminal;
		}

		virtual Stage* newSequence();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_UNIXUTILS_UNIXTERMINALBINDINGINPUTCONVERTER_HPP */
