#include "NativeObjectFileLanguage.hpp"

using redengine::util::Appender;

namespace redengine {
namespace build {

	NativeObjectFileLanguage::NativeObjectFileLanguage(Linker& linker) : LinkedLanguage(linker) {}

	NativeObjectFileLanguage::NativeObjectFileLanguage(const NativeObjectFileLanguage& language)
			: LinkedLanguage(language) {}

	void NativeObjectFileLanguage::getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		sink.append(Flavor::STATIC);
		sink.append(Flavor::DYNAMIC);
		sink.doneAppending();
	}

}}
