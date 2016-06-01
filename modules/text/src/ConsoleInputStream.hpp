#ifndef REDSTRAIN_MOD_TEXT_CONSOLEINPUTSTREAM_HPP
#define REDSTRAIN_MOD_TEXT_CONSOLEINPUTSTREAM_HPP

#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "TextCodecInputStream.hpp"
#include "api.hpp"

namespace redengine {
namespace text {

	template<
		typename RecordT,
		typename LinebreakRecordsT = typename io::FormattedInputStream<RecordT>::LinebreakRecords
	>
	class ConsoleInputStream : public io::FormattedInputStream<RecordT, LinebreakRecordsT> {

	  private:
		io::FileInputStream fileStream;
		TextCodec<char, RecordT>& codec;
		TextCodecInputStream<char, RecordT> codecStream;

	  protected:
		ConsoleInputStream(const ConsoleInputStream& stream) : io::Stream(stream),
				io::FormattedInputStream<RecordT, LinebreakRecordsT>(stream),
				fileStream(stream.fileStream.getFile().getHandle()), codec(stream.codec),
				codecStream(fileStream, codec) {}

	  public:
		ConsoleInputStream(platform::File::Handle handle, TextCodec<char, RecordT>& codec,
				io::LineOriented::LinebreakPolicy linebreaks = io::LineOriented::AUTO_LINEBREAKS,
				util::MemorySize bufferSize = static_cast<util::MemorySize>(0u))
				: io::FormattedInputStream<RecordT, LinebreakRecordsT>(codecStream, linebreaks, bufferSize),
				fileStream(handle), codec(codec), codecStream(fileStream, codec) {}

		virtual ~ConsoleInputStream() {
			delete &codec;
		}

	};

	REDSTRAIN_TEXT_API ConsoleInputStream<Char16>& getStandardInputStream16();

}}

#endif /* REDSTRAIN_MOD_TEXT_CONSOLEINPUTSTREAM_HPP */
