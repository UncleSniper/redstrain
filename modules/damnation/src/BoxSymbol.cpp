#include "BoxSymbol.hpp"

using redengine::text::Char16;

namespace redengine {
namespace damnation {

	static const char BS_CHARS8[] = {
		' ',
		'-',
		'|',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'=',
		'|',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'+',
		'.',
		'o',
		'O',
		'^',
		'v',
		'<',
		'>',
		'.',
		'O',
		'+',
		'&',
		'*',
		'<',
		'>',
		'^',
		'v',
		'-',
		'|',
		'>',
		'#',
		'#',
		' ',
		' ',
		' ',
		' ',
		'#',
		'#',
		'#',
		' ',
		' ',
		' ',
		' ',
		'#',
		'#',
		'#'
	};

	static const Char16 BS_CHARS16[] = {
		static_cast<Char16>(0x0020u),
		static_cast<Char16>(0x2500u),
		static_cast<Char16>(0x2502u),
		static_cast<Char16>(0x250Cu),
		static_cast<Char16>(0x2510u),
		static_cast<Char16>(0x2514u),
		static_cast<Char16>(0x2518u),
		static_cast<Char16>(0x251Cu),
		static_cast<Char16>(0x2524u),
		static_cast<Char16>(0x252Cu),
		static_cast<Char16>(0x2534u),
		static_cast<Char16>(0x253Cu),
		static_cast<Char16>(0x2550u),
		static_cast<Char16>(0x2551u),
		static_cast<Char16>(0x2554u),
		static_cast<Char16>(0x2557u),
		static_cast<Char16>(0x255Au),
		static_cast<Char16>(0x255Du),
		static_cast<Char16>(0x2560u),
		static_cast<Char16>(0x2563u),
		static_cast<Char16>(0x2566u),
		static_cast<Char16>(0x2569u),
		static_cast<Char16>(0x256Cu),
		static_cast<Char16>(0x2552u),
		static_cast<Char16>(0x2553u),
		static_cast<Char16>(0x2555u),
		static_cast<Char16>(0x2556u),
		static_cast<Char16>(0x2558u),
		static_cast<Char16>(0x2559u),
		static_cast<Char16>(0x255Bu),
		static_cast<Char16>(0x255Cu),
		static_cast<Char16>(0x255Eu),
		static_cast<Char16>(0x255Fu),
		static_cast<Char16>(0x2561u),
		static_cast<Char16>(0x2562u),
		static_cast<Char16>(0x2565u),
		static_cast<Char16>(0x2564u),
		static_cast<Char16>(0x2568u),
		static_cast<Char16>(0x2567u),
		static_cast<Char16>(0x256Au),
		static_cast<Char16>(0x256Bu),
		static_cast<Char16>(0x25A1u),
		static_cast<Char16>(0x25A3u),
		static_cast<Char16>(0x25A0u),
		static_cast<Char16>(0x25B2u),
		static_cast<Char16>(0x25BCu),
		static_cast<Char16>(0x25C4u),
		static_cast<Char16>(0x25BAu),
		static_cast<Char16>(0x2610u),
		static_cast<Char16>(0x2611u),
		static_cast<Char16>(0x2670u),
		static_cast<Char16>(0x231Bu),
		static_cast<Char16>(0x221Eu),
		static_cast<Char16>(0x2190u),
		static_cast<Char16>(0x2192u),
		static_cast<Char16>(0x2191u),
		static_cast<Char16>(0x2193u),
		static_cast<Char16>(0x2194u),
		static_cast<Char16>(0x2195u),
		static_cast<Char16>(0x21A6u),
		static_cast<Char16>(0x21B3u),
		static_cast<Char16>(0x2588u),
		static_cast<Char16>(0x2581u),
		static_cast<Char16>(0x2582u),
		static_cast<Char16>(0x2583u),
		static_cast<Char16>(0x2584u),
		static_cast<Char16>(0x2585u),
		static_cast<Char16>(0x2586u),
		static_cast<Char16>(0x2587u),
		static_cast<Char16>(0x258Fu),
		static_cast<Char16>(0x258Eu),
		static_cast<Char16>(0x258Du),
		static_cast<Char16>(0x258Cu),
		static_cast<Char16>(0x258Bu),
		static_cast<Char16>(0x258Au),
		static_cast<Char16>(0x2589u)
	};

	static const unsigned BS_CHAR_COUNT8 = static_cast<unsigned>(sizeof(BS_CHARS8) / sizeof(char));
	static const unsigned BS_CHAR_COUNT16 = static_cast<unsigned>(sizeof(BS_CHARS16) / sizeof(Char16));

	REDSTRAIN_DAMNATION_API char getBoxChar8(BoxSymbol symbol) {
		unsigned index = static_cast<unsigned>(symbol);
		return index >= BS_CHAR_COUNT8 ? '\0' : BS_CHARS8[index];
	}

	REDSTRAIN_DAMNATION_API text::Char16 getBoxChar16(BoxSymbol symbol) {
		unsigned index = static_cast<unsigned>(symbol);
		return index >= BS_CHAR_COUNT16 ? static_cast<Char16>(0u) : BS_CHARS16[index];
	}

	static const BoxSymbol LEFT_BLOCKS[] = {
		BS_BLANK,
		BS_LEFT_BLOCK_18,
		BS_LEFT_BLOCK_28,
		BS_LEFT_BLOCK_38,
		BS_LEFT_BLOCK_48,
		BS_LEFT_BLOCK_58,
		BS_LEFT_BLOCK_68,
		BS_LEFT_BLOCK_78,
		BS_FULL_BLOCK
	};

	static const BoxSymbol LOWER_BLOCKS[] = {
		BS_BLANK,
		BS_LOWER_BLOCK_18,
		BS_LOWER_BLOCK_28,
		BS_LOWER_BLOCK_38,
		BS_LOWER_BLOCK_48,
		BS_LOWER_BLOCK_58,
		BS_LOWER_BLOCK_68,
		BS_LOWER_BLOCK_78,
		BS_FULL_BLOCK
	};

	REDSTRAIN_DAMNATION_API BoxSymbol leftBlockSymbol(unsigned fraction) {
		if(fraction > 8u)
			fraction = 8u;
		return LEFT_BLOCKS[fraction];
	}

	REDSTRAIN_DAMNATION_API BoxSymbol lowerBlockSymbol(unsigned fraction) {
		if(fraction > 8u)
			fraction = 8u;
		return LOWER_BLOCKS[fraction];
	}

}}
