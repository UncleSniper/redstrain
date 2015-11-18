#include "Linker.hpp"
#include "UnrecognizedOperatingSystemNameError.hpp"

using std::string;
using redengine::redmond::Architecture;
using redengine::redmond::OperatingSystem;
using redengine::redmond::OS_LINUX;
using redengine::redmond::OS_WINDOWS;

namespace redengine {
namespace build {

	Linker::Linker(Architecture architecture, OperatingSystem targetOS)
			: architecture(architecture), targetOS(targetOS) {}

	Linker::Linker(const Linker& linker) : architecture(linker.architecture), targetOS(linker.targetOS) {}

	Linker::~Linker() {}

	OperatingSystem Linker::parseOperatingSystem(const string& specifier) {
		if(specifier == "linux" || specifier == "Linux")
			return OS_LINUX;
		if(specifier == "windows" || specifier == "Windows")
			return OS_WINDOWS;
		throw UnrecognizedOperatingSystemNameError(specifier);
	}

}}
