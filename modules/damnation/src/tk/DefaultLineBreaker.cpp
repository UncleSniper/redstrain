#include <redstrain/util/IntegerBounds.hpp>

#include "DefaultLineBreaker.hpp"

using redengine::text::Char16;
using redengine::text::String16;
using redengine::util::Appender;
using redengine::util::IntegerBounds;
using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace tk {

	DefaultLineBreaker DefaultLineBreaker::instance;

	DefaultLineBreaker::DefaultLineBreaker() {}

	DefaultLineBreaker::DefaultLineBreaker(const DefaultLineBreaker& breaker) : LineBreaker(breaker) {}

	static const MemorySize NONE = IntegerBounds<MemorySize>::MAX;

	void DefaultLineBreaker::breakIntoLines(const String16& longLine, MemorySize width, Appender<String16>& sink) {
		if(!width) {
			sink.doneAppending();
			return;
		}
		typedef String16::size_type Length;
		const MemorySize length = static_cast<MemorySize>(longLine.length());
		MemorySize start = static_cast<MemorySize>(0u), current;
		MemorySize lastWhite = NONE, lastHyphen = NONE, whiteRun = static_cast<MemorySize>(0u);
		for(current = static_cast<MemorySize>(0u); current < length; ++current) {
			if(current - start + static_cast<MemorySize>(1u) > width) {
				if(lastWhite != NONE && lastWhite > start) {
					if(lastHyphen != NONE && lastHyphen > lastWhite) {
						sink.append(longLine.substr(static_cast<Length>(start),
								static_cast<Length>(lastHyphen + static_cast<MemorySize>(1u) - start)));
						start = lastHyphen + static_cast<MemorySize>(1u);
					}
					else {
						sink.append(longLine.substr(static_cast<Length>(start),
								static_cast<Length>(lastWhite - start)));
						start = lastWhite + whiteRun;
					}
				}
				else if(lastHyphen != NONE && lastHyphen > start) {
					sink.append(longLine.substr(static_cast<Length>(start),
							static_cast<Length>(lastHyphen + static_cast<MemorySize>(1u) - start)));
					start = lastHyphen + static_cast<MemorySize>(1u);
				}
				else {
					sink.append(longLine.substr(static_cast<Length>(start), static_cast<Length>(width)));
					start += width;
				}
				lastWhite = lastHyphen = NONE;
			}
			switch(longLine[static_cast<Length>(current)]) {
				case static_cast<Char16>(' '):
					if(current == start) {
						++start;
						break;
					}
					if(lastWhite != NONE && lastWhite && current == lastWhite + whiteRun)
						++whiteRun;
					else {
						lastWhite = current;
						whiteRun = static_cast<MemorySize>(1u);
					}
					break;
				case static_cast<Char16>('-'):
					lastHyphen = current;
					break;
			}
		}
		if(start < length)
			sink.append(longLine.substr(static_cast<Length>(start)));
		sink.doneAppending();
	}

}}}
