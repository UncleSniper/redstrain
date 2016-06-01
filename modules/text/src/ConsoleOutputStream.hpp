#ifndef REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAM_HPP

#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/IndentingOutputStreamConfig.hpp>
#include <redstrain/io/NumberPrintingOutputStreamConfig.hpp>
#include <redstrain/io/ConfigurableFormattedOutputStream.hpp>

#include "TextCodecOutputStream.hpp"
#include "api.hpp"

namespace redengine {
namespace text {

	template<
		typename RecordT,
		typename LinebreakRecordsT = typename io::FormattedOutputStream<RecordT>::LinebreakRecords
	>
	class ConsoleOutputStream : public io::ConfigurableFormattedOutputStream<
		RecordT,
		LinebreakRecordsT,
		io::StreamConfig2<
			io::NumberPrintingOutputStreamConfig<RecordT>,
			io::IndentingOutputStreamConfig<RecordT>
		>::template ConfigTemplate
	> {

	  private:
		io::FileOutputStream fileStream;
		TextCodec<RecordT, char>& codec;
		TextCodecOutputStream<RecordT, char> codecStream;

	  protected:
		ConsoleOutputStream(const ConsoleOutputStream& stream) : io::Stream(stream),
				io::ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT,
				io::StreamConfig2<io::NumberPrintingOutputStreamConfig<RecordT>,
				io::IndentingOutputStreamConfig<RecordT> >::template ConfigTemplate>(stream),
				fileStream(stream.fileStream.getFile()), codec(stream.codec),
				codecStream(fileStream, codec) {}

	  public:
		ConsoleOutputStream(platform::File::Handle handle, TextCodec<RecordT, char>& codec,
				io::LineOriented::LinebreakPolicy linebreaks = io::LineOriented::AUTO_LINEBREAKS)
				: io::ConfigurableFormattedOutputStream<RecordT, LinebreakRecordsT,
				io::StreamConfig2<io::NumberPrintingOutputStreamConfig<RecordT>,
				io::IndentingOutputStreamConfig<RecordT> >::template ConfigTemplate>(codecStream, linebreaks),
				fileStream(handle), codec(codec), codecStream(fileStream, codec) {}

		virtual ~ConsoleOutputStream() {
			delete &codec;
		}

	};

	REDSTRAIN_TEXT_API ConsoleOutputStream<Char16>& getStandardOutputStream16();
	REDSTRAIN_TEXT_API ConsoleOutputStream<Char16>& getStandardErrorStream16();

}}

#endif /* REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAM_HPP */
