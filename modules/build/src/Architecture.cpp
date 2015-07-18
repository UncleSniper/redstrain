#include "Architecture.hpp"

using redengine::redmond::Architecture;
using redengine::redmond::ARCH_I686;

namespace redengine {
namespace build {

	REDSTRAIN_BUILD_API bool requiresPositionIndependentCode(Architecture architecture) {
		switch(architecture) {
			case ARCH_I686:
				return true;
			default:
				return false;
		}
	}

}}
