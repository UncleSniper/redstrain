#ifndef REDSTRAIN_MOD_IO_FORMATTEDINPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_FORMATTEDINPUTSTREAM_HPP

#include <string>

#include "LineOriented.hpp"
#include "ProxyInputStream.hpp"
#include "DefaultLinebreakRecords.hpp"

namespace redengine {
namespace io {

	template<
		typename RecordT,
		typename LinebreakRecordsT = DefaultLinebreakRecords<RecordT>
	>
	class FormattedInputStream : public ProxyInputStream<RecordT>, public LineOriented {

	  public:
		typedef std::basic_string<RecordT> String;

	  private:
		struct DeleteBuffer {

			RecordT* buffer;
			size_t offset, count;

			DeleteBuffer(RecordT* buffer, size_t offset, size_t count = static_cast<size_t>(0u))
					: buffer(buffer), offset(offset), count(count) {}

			~DeleteBuffer() {
				if(!buffer)
					return;
				RecordT* ptr = buffer + offset;
				RecordT* end = ptr + count;
				for(; ptr < end; ++ptr)
					ptr->~RecordT();
				delete[] reinterpret_cast<char*>(buffer);
			}

		};

		struct ClearBuffer {

			RecordT* buffer;
			size_t count;

			ClearBuffer(RecordT* buffer, size_t count = static_cast<size_t>(0u))
					: buffer(buffer), count(count) {}

			~ClearBuffer() {
				if(!buffer)
					return;
				RecordT* end = buffer + count;
				for(; buffer < end; ++buffer)
					buffer->~RecordT();
			}

		};

	  private:
		size_t bufferSize, bufferFill, bufferOffset;
		RecordT* lineBuffer;

	  protected:
		FormattedInputStream(const FormattedInputStream& stream)
				: Stream(stream), ProxyInputStream<RecordT>(stream), LineOriented(stream),
				bufferSize(stream.bufferSize), bufferFill(stream.bufferFill), bufferOffset(stream.bufferOffset) {
			lineBuffer = reinterpret_cast<RecordT*>(new char[bufferSize * sizeof(RecordT)]);
			DeleteBuffer delBuf(lineBuffer, bufferOffset);
			size_t index;
			for(index = bufferOffset; index < bufferFill; ++index) {
				new(lineBuffer + index) RecordT(stream.lineBuffer[index]);
				++delBuf.count;
			}
			delBuf.buffer = NULL;
		}

		virtual size_t readBlock(RecordT* buffer, size_t size) {
			size_t buffered = bufferFill - bufferOffset;
			if(!buffered) {
				size_t count = this->input.read(buffer, size);
				if(!count)
					this->atEnd = true;
				return count;
			}
			if(size > buffered)
				size = buffered;
			size_t index;
			ClearBuffer clearBuf(buffer);
			for(index = static_cast<size_t>(0u); index < buffered; ++index) {
				new(buffer + index) RecordT(lineBuffer[bufferOffset + index]);
				++bufferOffset;
				++clearBuf.count;
			}
			clearBuf.buffer = NULL;
			return clearBuf.count;
		}

	  private:
		bool fillBuffer() {
			if(bufferOffset != bufferFill)
				return true;
			bufferFill = this->input.read(lineBuffer, bufferSize);
			bufferOffset = static_cast<size_t>(0u);
			if(bufferFill)
				return true;
			this->atEnd = true;
			return false;
		}

		bool findLinebreak(size_t& offset) {
			size_t index;
			if(linebreaks == AUTO_LINEBREAKS) {
				for(index = bufferOffset; index < bufferFill; ++index) {
					if(lineBuffer[index] == LinebreakRecordsT::NEWLINE
							|| lineBuffer[index] == LinebreakRecordsT::CARRIAGE_RETURN) {
						offset = index;
						return true;
					}
				}
			}
			else {
				if(linebreaks == UNIX_LINEBREAKS) {
					for(index = bufferOffset; index < bufferFill; ++index) {
						if(lineBuffer[index] == LinebreakRecordsT::NEWLINE) {
							offset = index;
							return true;
						}
					}
				}
				else {
					for(index = bufferOffset; index < bufferFill; ++index) {
						if(lineBuffer[index] == LinebreakRecordsT::CARRIAGE_RETURN) {
							offset = index;
							return true;
						}
					}
				}
			}
			return false;
		}

	  public:
		FormattedInputStream(InputStream<RecordT>& input, LinebreakPolicy linebreaks = AUTO_LINEBREAKS,
				size_t bufferSize = static_cast<size_t>(0u)) : ProxyInputStream<RecordT>(input),
				LineOriented(linebreaks), bufferSize(bufferSize ? bufferSize
				: static_cast<size_t>(REDSTRAIN_IO_DEFAULT_LINE_BUFFER_RECORDS)),
				bufferFill(static_cast<size_t>(0u)), bufferOffset(static_cast<size_t>(0u)) {
			lineBuffer = reinterpret_cast<RecordT*>(new char[this->bufferSize * sizeof(RecordT)]);
		}

		virtual ~FormattedInputStream() {
			DeleteBuffer delBuf(lineBuffer, bufferOffset, bufferFill - bufferOffset);
		}

		inline size_t getBufferSize() const {
			return bufferSize;
		}

		void setBufferSize(size_t newBufferSize) {
			if(!newBufferSize)
				newBufferSize = static_cast<size_t>(REDSTRAIN_IO_DEFAULT_LINE_BUFFER_RECORDS);
			if(newBufferSize < bufferFill - bufferOffset)
				return;
			RecordT* newBuffer = reinterpret_cast<RecordT*>(new char[newBufferSize * sizeof(RecordT)]);
			DeleteBuffer delBuf(newBuffer, static_cast<size_t>(0u));
			size_t index;
			for(index = bufferOffset; index < bufferFill; ++index) {
				new(newBuffer + (index - bufferOffset)) RecordT(lineBuffer[index]);
				++delBuf.count;
			}
			delBuf.buffer = lineBuffer;
			delBuf.offset = bufferOffset;
			lineBuffer = newBuffer;
			bufferFill -= bufferOffset;
			bufferOffset = static_cast<size_t>(0u);
		}

		bool readLine(String& line) {
			bool haveData = false;
			for(;;) {
				if(!fillBuffer())
					return haveData;
				size_t offset;
				bool found = findLinebreak(offset);
				if(!found)
					offset = bufferFill;
				if(offset > bufferOffset) {
					line.append(lineBuffer + bufferOffset,
							static_cast<typename String::size_type>(offset - bufferOffset));
					for(; bufferOffset < offset; ++bufferOffset)
						lineBuffer[bufferOffset].~RecordT();
					haveData = true;
				}
				if(!found)
					continue;
				if(linebreaks != WINDOWS_LINEBREAKS
						&& (linebreaks != AUTO_LINEBREAKS || lineBuffer[offset] == LinebreakRecordsT::NEWLINE)) {
					lineBuffer[bufferOffset++].~RecordT();
					if(linebreaks == AUTO_LINEBREAKS)
						linebreaks = UNIX_LINEBREAKS;
					return true;
				}
				lineBuffer[bufferOffset++].~RecordT();
				if(!fillBuffer()) {
					if(linebreaks == WINDOWS_LINEBREAKS)
						line += LinebreakRecordsT::CARRIAGE_RETURN;
					else
						linebreaks = MACOS_LINEBREAKS;
					return true;
				}
				if(lineBuffer[bufferOffset] == LinebreakRecordsT::NEWLINE) {
					if(linebreaks == AUTO_LINEBREAKS)
						linebreaks = WINDOWS_LINEBREAKS;
					lineBuffer[bufferOffset++].~RecordT();
				}
				else {
					if(linebreaks == WINDOWS_LINEBREAKS) {
						line += LinebreakRecordsT::CARRIAGE_RETURN;
						haveData = true;
						continue;
					}
					linebreaks = MACOS_LINEBREAKS;
				}
				return true;
			}
		}

		virtual void seek(off_t offset, Stream::SeekWhence whence) {
			ProxyInputStream<RecordT>::seek(offset, whence);
			for(; bufferOffset < bufferFill; ++bufferOffset)
				lineBuffer[bufferOffset].~RecordT();
		}

		virtual size_t tell() const {
			return this->input.tell() - (bufferFill - bufferOffset);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_FORMATTEDINPUTSTREAM_HPP */
