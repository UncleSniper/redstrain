#ifndef REDSTRAIN_MOD_TEXT_DEFAULTFORMATSTRINGRENDITION_HPP
#define REDSTRAIN_MOD_TEXT_DEFAULTFORMATSTRINGRENDITION_HPP

namespace redengine {
namespace text {

	template<typename CharT>
	class DefaultFormatStringRendition {

	  public:
		static const CharT FORMATTING_INITIATOR = static_cast<CharT>('%');
		static const CharT BEGIN_SUBGROUP = static_cast<CharT>('{');
		static const CharT END_SUBGROUP = static_cast<CharT>('}');
		static const CharT IF_INITIATOR = static_cast<CharT>('?');

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DEFAULTFORMATSTRINGRENDITION_HPP */
