#include <redstrain/platform/Process.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "DebugMode.hpp"

using redengine::platform::Process;
using redengine::platform::DefaultSynchronizedSingleton;

namespace redengine {
namespace build {

	static DefaultSynchronizedSingleton<DebugMode> singleton;

	DebugMode::DebugMode() : debug(!!Process::getenv("REDBUILD_DEBUG")) {}

	DebugMode::DebugMode(const DebugMode& mode) : debug(mode.debug) {}

	DebugMode& DebugMode::getInstance() {
		return singleton.get();
	}
}}
