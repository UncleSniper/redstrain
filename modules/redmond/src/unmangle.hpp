#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class CPPSymbol;

}}}

namespace redengine {
namespace redmond {

	REDSTRAIN_REDMOND_API unmangle::CPPSymbol* unmangleCPPSymbol_native(const std::string&);

}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_HPP */
