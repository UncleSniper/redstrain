#ifndef REDSTRAIN_MOD_BUILD_NATIVEOBJECTFILELANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_NATIVEOBJECTFILELANGUAGE_HPP

#include "LinkedLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API NativeObjectFileLanguage : public LinkedLanguage {

	  public:
		NativeObjectFileLanguage(Linker&);
		NativeObjectFileLanguage(const NativeObjectFileLanguage&);

		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_NATIVEOBJECTFILELANGUAGE_HPP */
