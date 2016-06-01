#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "CodecManager.hpp"
#include "ConsoleInputStream.hpp"
#include "l10n.hpp"

using std::string;
using redengine::util::Delete;
using redengine::platform::File;
using redengine::platform::Console;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace text {

	class StandardInputStream16Singleton : public SynchronizedSingleton<ConsoleInputStream<Char16> > {

	  protected:
		virtual ConsoleInputStream<Char16>* newInstance();

	  public:
		StandardInputStream16Singleton();
		StandardInputStream16Singleton(const StandardInputStream16Singleton&);

	};

	StandardInputStream16Singleton::StandardInputStream16Singleton() {}

	StandardInputStream16Singleton::StandardInputStream16Singleton(const StandardInputStream16Singleton& singleton)
			: SynchronizedSingleton<ConsoleInputStream<Char16> >(singleton) {}

	ConsoleInputStream<Char16>* StandardInputStream16Singleton::newInstance() {
		File::Handle handle = Console::getStandardHandle(Console::STANDARD_INPUT);
		string tcs(guessTerminalCharset());
		if(tcs.empty())
			tcs = "UTF-8";
		Delete<Decoder16> decoder(CodecManager::getDefaultCodecManager().newDecoder16(tcs));
		ConsoleInputStream<Char16>* stream = new ConsoleInputStream<Char16>(handle, **decoder);
		decoder.set();
		return stream;
	}

	static StandardInputStream16Singleton standardInputStream16Singleton;

	REDSTRAIN_TEXT_API ConsoleInputStream<Char16>& getStandardInputStream16() {
		return standardInputStream16Singleton.get();
	}

}}
