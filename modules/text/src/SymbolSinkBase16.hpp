#ifndef REDSTRAIN_MOD_TEXT_SYMBOLSINKBASE16_HPP
#define REDSTRAIN_MOD_TEXT_SYMBOLSINKBASE16_HPP

#include <redstrain/redmond/unmangle/SymbolSinkBase8.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API SymbolSinkBase16 : public redmond::unmangle::SymbolSinkBase8 {

	  protected:
		virtual void writeString(const std::string&);
		virtual void writeString(const String16&) = 0;

	  public:
		SymbolSinkBase16(int = 0);
		SymbolSinkBase16(const SymbolSinkBase16&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_SYMBOLSINKBASE16_HPP */
