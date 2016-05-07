#ifndef REDSTRAIN_MOD_TEXT_TRANSCODE_HPP
#define REDSTRAIN_MOD_TEXT_TRANSCODE_HPP

#include <redstrain/io/MemoryInputStream.hpp>
#include <redstrain/io/StringOutputStream.hpp>

#include "TextCodec.hpp"
#include "api.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	struct REDSTRAIN_TEXT_API Transcode {

		template<typename SourceT, typename TargetT>
		static util::FileSize transcodeStream2(io::InputStream<SourceT>& input, io::OutputStream<TargetT>& output,
				TextCodec<SourceT, TargetT>& codec) {
			const util::MemorySize SOURCE_BUFFER_SIZE
					= static_cast<util::MemorySize>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES
					/ static_cast<unsigned>(sizeof(SourceT)));
			const util::MemorySize TARGET_BUFFER_SIZE
					= static_cast<util::MemorySize>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES
					/ static_cast<unsigned>(sizeof(TargetT)));
			SourceT inbuf[SOURCE_BUFFER_SIZE];
			TargetT outbuf[TARGET_BUFFER_SIZE];
			util::MemorySize consumed, outcount, c;
			util::FileSize total = static_cast<util::FileSize>(0u);
			for(;;) {
				util::MemorySize incount = input.read(inbuf, SOURCE_BUFFER_SIZE);
				if(!incount)
					break;
				for(consumed = static_cast<util::MemorySize>(0u); consumed < incount; consumed += c) {
					c = codec.transcodeBlock(inbuf + consumed, incount - consumed,
							outbuf, TARGET_BUFFER_SIZE, outcount);
					output.write(outbuf, outcount);
					total += static_cast<util::FileSize>(outcount);
				}
			}
			for(;;) {
				codec.transcodeBlock(NULL, static_cast<util::MemorySize>(0u), outbuf, TARGET_BUFFER_SIZE, outcount);
				if(!outcount)
					break;
				output.write(outbuf, outcount);
				total += static_cast<util::FileSize>(outcount);
			}
			codec.endCodeUnit();
			return total;
		}

		template<typename SourceT, typename IntermediateT, typename TargetT>
		static util::FileSize transcodeStream3(io::InputStream<SourceT>& input, io::OutputStream<TargetT>& output,
				TextCodec<SourceT, IntermediateT>& firstCodec, TextCodec<IntermediateT, TargetT>& secondCodec) {
			const util::MemorySize SOURCE_BUFFER_SIZE
					= static_cast<util::MemorySize>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES
					/ static_cast<unsigned>(sizeof(SourceT)));
			const util::MemorySize INTERMEDIATE_BUFFER_SIZE
					= static_cast<util::MemorySize>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES
					/ static_cast<unsigned>(sizeof(IntermediateT)));
			const util::MemorySize TARGET_BUFFER_SIZE
					= static_cast<util::MemorySize>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES
					/ static_cast<unsigned>(sizeof(TargetT)));
			SourceT inbuf[SOURCE_BUFFER_SIZE];
			IntermediateT tmpbuf[INTERMEDIATE_BUFFER_SIZE];
			TargetT outbuf[TARGET_BUFFER_SIZE];
			util::MemorySize inconsumed, ic, tmpcount, tmpconsumed, tc, outcount;
			util::FileSize total = static_cast<util::FileSize>(0u);
			for(;;) {
				util::MemorySize incount = input.read(inbuf, SOURCE_BUFFER_SIZE);
				if(!incount)
					break;
				for(inconsumed = static_cast<util::MemorySize>(0u); inconsumed < incount; inconsumed += ic) {
					ic = firstCodec.transcodeBlock(inbuf + inconsumed, incount - inconsumed,
							tmpbuf, INTERMEDIATE_BUFFER_SIZE, tmpcount);
					for(tmpconsumed = static_cast<util::MemorySize>(0u); tmpconsumed < tmpcount; tmpconsumed += tc) {
						tc = secondCodec.transcodeBlock(tmpbuf + tmpconsumed, tmpcount - tmpconsumed,
								outbuf, TARGET_BUFFER_SIZE, outcount);
						output.write(outbuf, outcount);
						total += static_cast<util::FileSize>(outcount);
					}
				}
			}
			for(;;) {
				firstCodec.transcodeBlock(NULL, static_cast<util::MemorySize>(0u),
						tmpbuf, INTERMEDIATE_BUFFER_SIZE, tmpcount);
				if(!tmpcount)
					break;
				for(tmpconsumed = static_cast<util::MemorySize>(0u); tmpconsumed < tmpcount; tmpconsumed += tc) {
					tc = secondCodec.transcodeBlock(tmpbuf + tmpconsumed, tmpcount - tmpconsumed,
							outbuf, TARGET_BUFFER_SIZE, outcount);
					output.write(outbuf, outcount);
					total += static_cast<util::FileSize>(outcount);
				}
			}
			firstCodec.endCodeUnit();
			for(;;) {
				secondCodec.transcodeBlock(NULL, static_cast<util::MemorySize>(0u),
						outbuf, TARGET_BUFFER_SIZE, outcount);
				if(!outcount)
					break;
				output.write(outbuf, outcount);
				total += static_cast<util::FileSize>(outcount);
			}
			secondCodec.endCodeUnit();
			return total;
		}

		template<typename SourceT, typename TargetT>
		static void transcodeString2(const std::basic_string<SourceT>& input, std::basic_string<TargetT>& output,
				TextCodec<SourceT, TargetT>& codec) {
			io::MemoryInputStream<SourceT> source(input.data(), static_cast<util::MemorySize>(input.length()));
			io::StringOutputStream<TargetT> sink(output);
			Transcode::transcodeStream2<SourceT, TargetT>(source, sink, codec);
		}

		template<typename SourceT, typename IntermediateT, typename TargetT>
		static void transcodeString3(const std::basic_string<SourceT>& input, std::basic_string<TargetT>& output,
				TextCodec<SourceT, IntermediateT>& firstCodec, TextCodec<IntermediateT, TargetT>& secondCodec) {
			io::MemoryInputStream<SourceT> source(input.data(), static_cast<util::MemorySize>(input.length()));
			io::StringOutputStream<TargetT> sink(output);
			Transcode::transcodeStream3<SourceT, IntermediateT, TargetT>(source, sink, firstCodec, secondCodec);
		}

		template<typename SourceT, typename TargetT>
		static std::basic_string<TargetT> transcodeShortString2(const std::basic_string<SourceT>& input,
				TextCodec<SourceT, TargetT>& codec) {
			std::basic_string<TargetT> output;
			io::MemoryInputStream<SourceT> source(input.data(), static_cast<util::MemorySize>(input.length()));
			io::StringOutputStream<TargetT> sink(output);
			Transcode::transcodeStream2<SourceT, TargetT>(source, sink, codec);
			return output;
		}

		template<typename SourceT, typename IntermediateT, typename TargetT>
		static std::basic_string<TargetT> transcodeShortString3(const std::basic_string<SourceT>& input,
				TextCodec<SourceT, IntermediateT>& firstCodec, TextCodec<IntermediateT, TargetT>& secondCodec) {
			std::basic_string<TargetT> output;
			io::MemoryInputStream<SourceT> source(input.data(), static_cast<util::MemorySize>(input.length()));
			io::StringOutputStream<TargetT> sink(output);
			Transcode::transcodeStream3<SourceT, IntermediateT, TargetT>(source, sink, firstCodec, secondCodec);
			return output;
		}

		static String16 utf8ToBMP(const std::string&);
		static std::string bmpToUTF8(const String16&);

		static String32 utf8ToUnicode(const std::string&);
		static std::string unicodeToUTF8(const String32&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TRANSCODE_HPP */
