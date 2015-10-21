#ifndef REDSTRAIN_MOD_TEXT_PROTOSTR_HPP
#define REDSTRAIN_MOD_TEXT_PROTOSTR_HPP

#include <redstrain/protostr/ProtocolWriter.hpp>
#include <redstrain/protostr/ProtocolReader.hpp>
#include <redstrain/util/StandardIntegerMapping.hpp>

#include "types.hpp"

namespace redengine {
namespace text {

	template<typename SizeT, typename CharT>
	void writeMultiByteString(protostr::ProtocolWriter& writer, const std::basic_string<CharT>& value) {
		typedef typename util::StandardIntegerMapping<SizeT>::StandardType StdSizeT;
		typedef typename util::StandardIntegerMapping<CharT>::StandardType StdCharT;
		protostr::writeProtocolPrimitive<StdSizeT>(writer, static_cast<StdSizeT>(value.length()));
		typename std::basic_string<CharT>::const_iterator begin(value.begin()), end(value.end());
		for(; begin != end; ++begin)
			protostr::writeProtocolPrimitive<StdCharT>(writer, static_cast<StdCharT>(*begin));
	}

	template<typename SizeT, typename CharT>
	void readMultiByteString(protostr::ProtocolReader& reader, std::basic_string<CharT>& value) {
		typedef typename util::StandardIntegerMapping<SizeT>::StandardType StdSizeT;
		typedef typename util::StandardIntegerMapping<CharT>::StandardType StdCharT;
		value.clear();
		SizeT size = static_cast<SizeT>(protostr::readProtocolPrimitive<StdSizeT>(reader));
		value.reserve(static_cast<typename std::basic_string<CharT>::size_type>(size));
		for(; size; --size)
			value += static_cast<CharT>(protostr::readProtocolPrimitive<StdCharT>(reader));
	}

}}

#endif /* REDSTRAIN_MOD_TEXT_PROTOSTR_HPP */
