#ifndef REDSTRAIN_MOD_IO_PARSENUM_HPP
#define REDSTRAIN_MOD_IO_PARSENUM_HPP

#include <redstrain/util/IntegerBounds.hpp>

#include "NumberFormatError.hpp"
#include "NumberAddressSpaceError.hpp"
#include "NumberPrintingOutputStreamConfig.hpp"

namespace redengine {
namespace io {

	enum NumberCharClass {
		NCC_NONE,
		NCC_SIGN,
		NCC_DIGIT,
		NCC_PERIOD
	};

	template<typename IntT>
	class NumberTypeSignFilter {

	  private:
		template<int Dummy, bool Signed>
		struct Filter {

			static NumberCharClass filterClass(NumberCharClass cc) {
				return cc;
			}

		};

		template<int Dummy>
		struct Filter<Dummy, false> {

			static NumberCharClass filterClass(NumberCharClass cc) {
				return cc == NCC_SIGN ? NCC_NONE : cc;
			}

		};

	  public:
		static NumberCharClass filterClass(NumberCharClass cc) {
			return Filter<0, util::IntegerBounds<IntT>::SIGNED>::filterClass(cc);
		}

	};

	template<typename CharT, typename IntT>
	struct DefaultNumberCharSemantics {

		NumberCharClass getCharClass(CharT c, NumberBase base) {
			switch(c) {
				case static_cast<CharT>('+'):
				case static_cast<CharT>('-'):
					return NCC_SIGN;
				case static_cast<CharT>('.'):
					return NCC_PERIOD;
			}
			int value;
			if(c >= static_cast<CharT>('0') && c <= static_cast<CharT>('9'))
				value = static_cast<int>(c - static_cast<CharT>('0'));
			else if(c >= static_cast<CharT>('a') && c <= static_cast<CharT>('z'))
				value = static_cast<int>(c - static_cast<CharT>('a')) + 10;
			else if(c >= static_cast<CharT>('A') && c <= static_cast<CharT>('Z'))
				value = static_cast<int>(c - static_cast<CharT>('A')) + 10;
			else
				return NCC_NONE;
			return value < static_cast<int>(base) ? NCC_DIGIT : NCC_NONE;
		}

		bool isSignNegative(CharT c) {
			return c == static_cast<CharT>('-');
		}

		IntT getDigitValue(CharT c) {
			if(c >= static_cast<CharT>('0') && c <= static_cast<CharT>('9'))
				return static_cast<IntT>(c - static_cast<CharT>('0'));
			else if(c >= static_cast<CharT>('a') && c <= static_cast<CharT>('z'))
				return static_cast<IntT>(c - static_cast<CharT>('a')) + static_cast<IntT>(10);
			else if(c >= static_cast<CharT>('A') && c <= static_cast<CharT>('Z'))
				return static_cast<IntT>(c - static_cast<CharT>('A')) + static_cast<IntT>(10);
			else
				return static_cast<IntT>(0);
		}

	};

	template<typename CharT>
	struct DefaultNumberErrorRenderer;

	template<>
	struct DefaultNumberErrorRenderer<char> {

		static std::string renderNumberForError(const std::string& number) {
			return number;
		}

	};

	template<
		typename IntT,
		typename CharT,
		typename CharSemanticsT,
		typename ErrorRendererT
	>
	IntT parseInteger(
		const std::basic_string<CharT>& specifier,
		NumberBase base = DECIMAL,
		CharSemanticsT charSemantics = CharSemanticsT()
	) {
		IntT result = static_cast<IntT>(0);
		typename std::basic_string<CharT>::const_iterator begin(specifier.begin()), end(specifier.end());
		if(begin == end)
			throw NumberFormatError(ErrorRendererT::renderNumberForError(specifier));
		bool negative = false;
		NumberCharClass cc = NumberTypeSignFilter<IntT>::filterClass(charSemantics.getCharClass(*begin, base));
		if(cc == NCC_SIGN) {
			negative = charSemantics.isSignNegative(*begin);
			if(++begin == end)
				throw NumberFormatError(ErrorRendererT::renderNumberForError(specifier));
			cc = NumberTypeSignFilter<IntT>::filterClass(charSemantics.getCharClass(*begin, base));
		}
		for(;;) {
			if(cc != NCC_DIGIT)
				throw NumberFormatError(ErrorRendererT::renderNumberForError(specifier));
			IntT digit = charSemantics.getDigitValue(*begin);
			IntT next = result * static_cast<IntT>(base) + digit;
			if(next < result)
				throw NumberAddressSpaceError(ErrorRendererT::renderNumberForError(specifier));
			result = next;
			if(++begin == end)
				break;
			cc = NumberTypeSignFilter<IntT>::filterClass(charSemantics.getCharClass(*begin, base));
		}
		return negative ? -result : result;
	}

}}

#endif /* REDSTRAIN_MOD_IO_PARSENUM_HPP */
