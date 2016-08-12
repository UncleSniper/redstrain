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
					setCurrentSymbolGeneric(outputChar);
			} while(!consumed);
			for(;;) {
				decoder.transcodeBlock(NULL, static_cast<MemorySize>(0u),
						&outputChar, static_cast<MemorySize>(1u), outcount);
				if(!outcount)
					return true;
				setCurrentSymbolGeneric(outputChar);
			}
		}
		catch(const TextError&) {
			failed = true;
			return false;
		}
	}

	void UNIXTerminalBindingInputConverter::DecoderStage::setCurrentSymbolGeneric(Char16 c) {
#define c16(x) static_cast<Char16>(x)
		switch(c) {
			case c16(1u):
			case c16(2u):
			case c16(3u):
			case c16(4u):
			case c16(5u):
			case c16(6u):
			case c16(7u):
			case c16(11u):
			case c16(12u):
			case c16(14u):
			case c16(15u):
			case c16(16u):
			case c16(17u):
			case c16(18u):
			case c16(19u):
			case c16(20u):
			case c16(21u):
			case c16(22u):
			case c16(23u):
			case c16(24u):
			case c16(25u):
			case c16(26u):
				currentSymbol.assign(KeySym::T_GENERIC, KeySym::M_CONTROL,
						static_cast<Char16>(c + static_cast<Char16>(96u)));
				break;
			default:
				currentSymbol.assign(KeySym::T_GENERIC, KeySym::M_NONE, c);
				break;
		}
#undef c16
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
