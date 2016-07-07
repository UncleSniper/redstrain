#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "CodecManager.hpp"
#include "ConsoleOutputStream.hpp"
#include "l10n.hpp"

using std::string;
using redengine::util::Delete;
using redengine::platform::File;
using redengine::platform::Console;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace text {

	class StandardOutputStream16Singleton : public SynchronizedSingleton<ConsoleOutputStream<Char16> > {

	  private:
		const bool isErrorStream;

	  protected:
		virtual ConsoleOutputStream<Char16>* newInstance();

	  public:
		StandardOutputStream16Singleton(bool);
		StandardOutputStream16Singleton(const StandardOutputStream16Singleton&);

	};

	StandardOutputStream16Singleton::StandardOutputStream16Singleton(bool isErrorStream)
			: isErrorStream(isErrorStream) {}

	StandardOutputStream16Singleton::StandardOutputStream16Singleton(const StandardOutputStream16Singleton& singleton)
			: SynchronizedSingleton<ConsoleOutputStream<Char16> >(singleton),
			isErrorStream(singleton.isErrorStream) {}

	ConsoleOutputStream<Char16>* StandardOutputStream16Singleton::newInstance() {
		File::Handle handle = Console::getStandardHandle(isErrorStream
				? Console::STANDARD_ERROR : Console::STANDARD_OUTPUT);
		string tcs(guessTerminalCharset());
		if(tcs.empty())
			tcs = "UTF-8";
		Delete<Encoder16> encoder(CodecManager::getDefaultCodecManager().newEncoder16(tcs));
		ConsoleOutputStream<Char16>* stream = new ConsoleOutputStream<Char16>(handle, **encoder);
		encoder.set();
		return stream;
	}

	static StandardOutputStream16Singleton standardOutputSteam16Singleton(false);
	static StandardOutputStream16Singleton standardErrorSteam16Singleton(true);

	REDSTRAIN_TEXT_API ConsoleOutputStream<Char16>& getStandardOutputStream16() {
		return standardOutputSteam16Singleton.get();
	}

	REDSTRAIN_TEXT_API ConsoleOutputStream<Char16>& getStandardErrorStream16() {
		return standardErrorSteam16Singleton.get();
	}

}}
