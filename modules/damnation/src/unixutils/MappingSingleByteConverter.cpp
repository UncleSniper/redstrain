#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "MappingSingleByteConverter.hpp"

using redengine::util::Delete;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace damnation {
namespace unixutils {

	MappingSingleByteConverter::MappingSingleByteConverter() {}

	MappingSingleByteConverter::MappingSingleByteConverter(const MappingSingleByteConverter& converter)
			: SingleByteConverter(converter), keys(converter.keys) {}

	MappingSingleByteConverter::~MappingSingleByteConverter() {}

	void MappingSingleByteConverter::getMappedKeys(KeyIterator& begin, KeyIterator& end) const {
		begin = keys.begin();
		end = keys.end();
	}

	void MappingSingleByteConverter::mapKey(char inputByte, const KeySym& key) {
		keys[inputByte] = key;
	}

	bool MappingSingleByteConverter::unmapKey(char inputByte) {
		return !!keys.erase(inputByte);
	}

	void MappingSingleByteConverter::clear() {
		keys.clear();
	}

	void MappingSingleByteConverter::registerControlKeys() {
		char c;
		for(c = 'a'; c <= 'z'; ++c) {
			if(c != 'm')  // return has symbolic key
				mapKey(static_cast<char>(c & '\x1F'), KeySym(KeySym::T_GENERIC, KeySym::M_CONTROL, c));
		}
	}

	KeySym MappingSingleByteConverter::byteToKeySym(char inputByte) {
		KeyIterator it(keys.find(inputByte));
		return it == keys.end() ? KeySym::NONE : it->second;
	}

	class DefaultMappingSingleByteConverterSingleton : public SynchronizedSingleton<MappingSingleByteConverter> {

	  public:
		enum Semantics {
			S_CONTROL,
			S_ALTERNATE_BACKSPACE
		};

	  private:
		const Semantics semantics;

	  protected:
		virtual MappingSingleByteConverter* newInstance();

	  public:
		DefaultMappingSingleByteConverterSingleton(Semantics);
		DefaultMappingSingleByteConverterSingleton(const DefaultMappingSingleByteConverterSingleton&);

	};

	DefaultMappingSingleByteConverterSingleton::DefaultMappingSingleByteConverterSingleton(Semantics semantics)
			: semantics(semantics) {}

	DefaultMappingSingleByteConverterSingleton::DefaultMappingSingleByteConverterSingleton(const
			DefaultMappingSingleByteConverterSingleton& singleton)
			: SynchronizedSingleton<MappingSingleByteConverter>(singleton), semantics(singleton.semantics) {}

	MappingSingleByteConverter* DefaultMappingSingleByteConverterSingleton::newInstance() {
		Delete<MappingSingleByteConverter> converter(new MappingSingleByteConverter);
		switch(semantics) {
			case S_CONTROL:
				converter->registerControlKeys();
				return converter.set();
			case S_ALTERNATE_BACKSPACE:
				converter->mapKey('\x7f', KeySym::BACKSPACE);
				return converter.set();
			default:
				throw ProgrammingError("Unrecognized Semantics in "
						"DefaultMappingSingleByteConverterSingleton::newInstance(): "
						+ StringUtils::toString(static_cast<int>(semantics)));
		}
	}

	static DefaultMappingSingleByteConverterSingleton
			controlSingleton(DefaultMappingSingleByteConverterSingleton::S_CONTROL),
			alternateBackspaceSingleton(DefaultMappingSingleByteConverterSingleton::S_ALTERNATE_BACKSPACE);

	SingleByteConverter& MappingSingleByteConverter::getControlKeysConverter() {
		return controlSingleton.get();
	}

	SingleByteConverter& MappingSingleByteConverter::getAlternateBackspaceConverter() {
		return alternateBackspaceSingleton.get();
	}

}}}
