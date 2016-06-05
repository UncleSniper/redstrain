#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_GCC_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_GCC_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class CPPSymbol;

}}}

namespace redengine {
namespace redmond {

	REDSTRAIN_REDMOND_API unmangle::CPPSymbol* unmangleCPPSymbol_GCC3(const std::string&);

}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_GCC_HPP */
