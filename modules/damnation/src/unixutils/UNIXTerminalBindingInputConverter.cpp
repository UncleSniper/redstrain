#include <redstrain/util/Delete.hpp>
#include <redstrain/text/TextError.hpp>

#include "UNIXTerminalBindingInputConverter.hpp"

using redengine::util::Delete;
using redengine::text::Char16;
using redengine::text::Decoder16;
using redengine::text::TextError;
using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace unixutils {

	// ======== DecoderStage ========

	UNIXTerminalBindingInputConverter::DecoderStage::DecoderStage(Decoder16& decoder)
			: decoder(decoder), failed(false) {}

	UNIXTerminalBindingInputConverter::DecoderStage::DecoderStage(const DecoderStage& stage)
			: Stage(stage), decoder(stage.decoder) {}

	UNIXTerminalBindingInputConverter::DecoderStage::~DecoderStage() {
		delete &decoder;
	}

	KeySym UNIXTerminalBindingInputConverter::DecoderStage::getCurrentSymbol() {
		return currentSymbol;
	}

	bool UNIXTerminalBindingInputConverter::DecoderStage::isInputLeaf() {
		return failed || currentSymbol.getType() != KeySym::T_NONE;
	}

	bool UNIXTerminalBindingInputConverter::DecoderStage::advanceStage(char inputByte) {
		if(failed)
			return false;
		try {
			Char16 outputChar;
			MemorySize outcount, consumed;
			do {
				consumed = decoder.transcodeBlock(&inputByte, static_cast<MemorySize>(1u),
						&outputChar, static_cast<MemorySize>(1u), outcount);
				if(outcount)
					currentSymbol.assign(KeySym::T_GENERIC, KeySym::M_NONE, outputChar);
			} while(!consumed);
			for(;;) {
				decoder.transcodeBlock(NULL, static_cast<MemorySize>(0u),
						&outputChar, static_cast<MemorySize>(1u), outcount);
				if(!outcount)
					return true;
				currentSymbol.assign(KeySym::T_GENERIC, KeySym::M_NONE, outputChar);
			}
		}
		catch(const TextError&) {
			failed = true;
			return false;
		}
	}

	// ======== UNIXTerminalBindingInputConverter ========

	UNIXTerminalBindingInputConverter::UNIXTerminalBindingInputConverter(const UNIXTerminalBinding& terminal)
			: terminal(terminal) {}

	UNIXTerminalBindingInputConverter::UNIXTerminalBindingInputConverter(const
			UNIXTerminalBindingInputConverter& converter)
			: InputConverter(converter), terminal(converter.terminal) {}

	InputConverter::Stage* UNIXTerminalBindingInputConverter::newSequence() {
		Delete<Decoder16> decoder(terminal.getDecoderFactory().newCodec());
		DecoderStage* stage = new DecoderStage(**decoder);
		decoder.set();
		return stage;
	}

}}}
