#ifndef REDSTRAIN_MOD_IO_FORMATTEDOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_FORMATTEDOUTPUTSTREAM_HPP

#include <string>

#include "LineOriented.hpp"
#include "ProxyOutputStream.hpp"
#include "DefaultLinebreakRecords.hpp"

namespace redengine {
namespace io {

	template<
		typename RecordT,
		typename LinebreakRecordsT = DefaultLinebreakRecords<RecordT>
	>
	class FormattedOutputStream : public ProxyOutputStream<RecordT>, public LineOriented {

	  public:
		typedef std::basic_string<RecordT> String;

	  protected:
		FormattedOutputStream(const FormattedOutputStream& stream)
				: Stream(stream), ProxyOutputStream<RecordT>(stream), LineOriented(stream) {}

	  public:
		FormattedOutputStream(OutputStream<RecordT>& output, LinebreakPolicy linebreaks = AUTO_LINEBREAKS)
				: ProxyOutputStream<RecordT>(output), LineOriented(linebreaks) {}

		void endLine() {
			if(linebreaks == AUTO_LINEBREAKS)
				linebreaks = PLATFORM_DEFAULT_LINEBREAK_POLICY;
			RecordT chars[2] = {LinebreakRecordsT::CARRIAGE_RETURN, LinebreakRecordsT::NEWLINE};
			switch(linebreaks) {
				case MACOS_LINEBREAKS:
					this->writeBlock(chars, static_cast<util::MemorySize>(1u));
					break;
				case WINDOWS_LINEBREAKS:
					this->writeBlock(chars, static_cast<util::MemorySize>(2u));
					break;
				default:
					this->writeBlock(chars + 1, static_cast<util::MemorySize>(1u));
					break;
			}
		}

		void print(const RecordT& record) {
			this->writeBlock(&record, static_cast<util::MemorySize>(1u));
		}

		void print(const String& text) {
			this->writeBlock(text.data(), static_cast<util::MemorySize>(text.length()));
		}

		void println(const String& text) {
			print(text);
			endLine();
		}

		void print(const RecordT* text) {
			util::MemorySize size = static_cast<util::MemorySize>(0u);
			const RecordT* ptr = text;
			for(; *ptr; ++ptr)
				++size;
			this->writeBlock(text, size);
		}

		void println(const RecordT* text) {
			print(text);
			endLine();
		}

		void print(const RecordT* text, util::MemorySize size) {
			this->writeBlock(text, size);
		}

		void println(const RecordT* text, util::MemorySize size) {
			this->writeBlock(text, size);
			endLine();
		}

		void printMany(const RecordT& record, util::MemorySize count) {
			for(; count; --count)
				this->writeBlock(&record, static_cast<util::MemorySize>(1u));
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_FORMATTEDOUTPUTSTREAM_HPP */
