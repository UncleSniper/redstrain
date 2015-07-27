#ifndef REDSTRAIN_MOD_BUILD_DEBUGMODE_HPP
#define REDSTRAIN_MOD_BUILD_DEBUGMODE_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API DebugMode {

	  private:
		const bool debug;

	  public:
		DebugMode();
		DebugMode(const DebugMode&);

		inline bool isDebug() const {
			return debug;
		}

		static DebugMode& getInstance();

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DEBUGMODE_HPP */
