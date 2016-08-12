#include <cstring>
#include <redstrain/util/Delete.hpp>

#include "SingleByteConverter.hpp"
#include "BinaryToKeySymConverter.hpp"

using redengine::util::Delete;
using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace unixutils {

	// ======== Chunk ========

	BinaryToKeySymConverter::Chunk::Chunk(const char* buffer, MemorySize size)
			: bytes(size ? new char[size] : NULL), size(size) {
		if(size)
			memcpy(bytes, buffer, static_cast<size_t>(size));
	}

	BinaryToKeySymConverter::Chunk::Chunk(const Chunk& chunk)
			: bytes(chunk.size ? new char[chunk.size] : NULL), size(chunk.size) {
		if(size)
			memcpy(bytes, chunk.bytes, static_cast<size_t>(size));
	}

	BinaryToKeySymConverter::Chunk::~Chunk() {
		if(bytes)
			delete[] bytes;
	}

	// ======== BinaryToKeySymConverter ========

	BinaryToKeySymConverter::BinaryToKeySymConverter() : stages(NULL), stageCount(static_cast<MemorySize>(0u)),
			lastSymbol(KeySym::NONE), lastLength(static_cast<MemorySize>(0u)),
			flushPointer(static_cast<MemorySize>(0u)), currentDepth(static_cast<MemorySize>(0u)),
			pathCount(static_cast<MemorySize>(0u)) {}

	BinaryToKeySymConverter::BinaryToKeySymConverter(const BinaryToKeySymConverter& converter)
			: inputConverters(converter.inputConverters), singleByteConverters(converter.singleByteConverters),
			stages(NULL), stageCount(static_cast<MemorySize>(0u)), lastSymbol(KeySym::NONE),
			lastLength(static_cast<MemorySize>(0u)), flushPointer(static_cast<MemorySize>(0u)),
			currentDepth(static_cast<MemorySize>(0u)), pathCount(static_cast<MemorySize>(0u)) {
		InputConverterIterator icbegin(inputConverters.begin()), icend(inputConverters.end());
		for(; icbegin != icend; ++icbegin)
			(*icbegin)->ref();
		SingleByteConverterIterator sbcbegin(singleByteConverters.begin()), sbcend(singleByteConverters.end());
		for(; sbcbegin != sbcend; ++sbcbegin)
			(*sbcbegin)->ref();
	}

	BinaryToKeySymConverter::~BinaryToKeySymConverter() {
		InputConverterIterator icbegin(inputConverters.begin()), icend(inputConverters.end());
		for(; icbegin != icend; ++icbegin)
			(*icbegin)->unref();
		SingleByteConverterIterator sbcbegin(singleByteConverters.begin()), sbcend(singleByteConverters.end());
		for(; sbcbegin != sbcend; ++sbcbegin)
			(*sbcbegin)->unref();
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < stageCount; ++u) {
			if(stages[u])
				delete stages[u];
		}
		if(stages)
			delete[] stages;
		Chunks::const_iterator cbegin(chunks.begin()), cend(chunks.end());
		for(; cbegin != cend; ++cbegin)
			delete *cbegin;
	}

	void BinaryToKeySymConverter::getInputConverters(InputConverterIterator& begin,
			InputConverterIterator& end) const {
		begin = inputConverters.begin();
		end = inputConverters.end();
	}

	void BinaryToKeySymConverter::addInputConverter(InputConverter& converter) {
		inputConverters.push_back(&converter);
		converter.ref();
	}

	bool BinaryToKeySymConverter::removeInputConverter(const InputConverter& converter) {
		InputConverters::iterator begin(inputConverters.begin()), end(inputConverters.end());
		for(; begin != end; ++begin) {
			if(*begin == &converter) {
				InputConverter& nonConst = **begin;
				inputConverters.erase(begin);
				nonConst.unref();
				return true;
			}
		}
		return false;
	}

	void BinaryToKeySymConverter::clearInputConverters() {
		InputConverterIterator begin(inputConverters.begin()), end(inputConverters.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		inputConverters.clear();
	}

	void BinaryToKeySymConverter::getSingleByteConverters(SingleByteConverterIterator& begin,
			SingleByteConverterIterator& end) const {
		begin = singleByteConverters.begin();
		end = singleByteConverters.end();
	}

	void BinaryToKeySymConverter::addSingleByteConverter(SingleByteConverter& converter) {
		singleByteConverters.push_back(&converter);
		converter.ref();
	}

	bool BinaryToKeySymConverter::removeSingleByteConverter(const SingleByteConverter& converter) {
		SingleByteConverters::iterator begin(singleByteConverters.begin()), end(singleByteConverters.end());
		for(; begin != end; ++begin) {
			if(*begin == &converter) {
				SingleByteConverter& nonConst = **begin;
				singleByteConverters.erase(begin);
				nonConst.unref();
				return true;
			}
		}
		return false;
	}

	void BinaryToKeySymConverter::clearSingleByteConverters() {
		SingleByteConverterIterator begin(singleByteConverters.begin()), end(singleByteConverters.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		singleByteConverters.clear();
	}

	KeySym BinaryToKeySymConverter::nextKey() {
		if(symbolQueue.empty())
			return KeySym::NONE;
		KeySym key(symbolQueue.front());
		symbolQueue.pop_front();
		return key;
	}

	void BinaryToKeySymConverter::feedInput(const char* buffer, MemorySize size) {
		if(!size)
			return;
		Delete<Chunk> chunk(new Chunk(buffer, size));
		chunks.push_back(*chunk);
		chunk.set();
		bool pushed = false;
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < size; ++u) {
			if(feedByte(buffer[u])) {
				pushed = true;
				break;
			}
		}
		if(pushed)
			while(feedChunkQueue()) {}
	}

	bool BinaryToKeySymConverter::feedChunkQueue() {
		bool first = true;
		Chunks::const_iterator begin(chunks.begin()), end(chunks.end());
		for(; begin != end; ++begin) {
			MemorySize start;
			if(first) {
				start = flushPointer;
				first = false;
			}
			else
				start = static_cast<MemorySize>(0u);
			const Chunk& chunk = **begin;
			for(; start < chunk.size; ++start) {
				if(feedByte(chunk.bytes[start]))
					return true;
			}
		}
		return false;
	}

	bool BinaryToKeySymConverter::feedByte(char input) {
		if(!currentDepth)
			beginSequences();
		advanceSequences(input);
		if(!currentDepth)
			processSingles(input);
		++currentDepth;
		if(!pathCount) {
			endPaths(input);
			return true;
		}
		else
			return false;
	}

	void BinaryToKeySymConverter::beginSequences() {
		stageCount = static_cast<MemorySize>(inputConverters.size());
		stages = stageCount ? new InputConverter::Stage*[stageCount] : NULL;
		pathCount = static_cast<MemorySize>(0u);
		InputConverterIterator begin(inputConverters.begin()), end(inputConverters.end());
		MemorySize index = static_cast<MemorySize>(0u);
		for(; begin != end; ++begin, ++index) {
			InputConverter::Stage* stage = (*begin)->newSequence();
			stages[index] = stage;
			if(!stage)
				continue;
			++pathCount;
			KeySym sym(stage->getCurrentSymbol());
			if(lastSymbol.getType() == KeySym::T_NONE && sym.getType() != KeySym::T_NONE) {
				lastSymbol = sym;
				lastLength = static_cast<MemorySize>(0u);
			}
			if(stage->isInputLeaf()) {
				delete stage;
				stages[index] = NULL;
				--pathCount;
			}
		}
	}

	void BinaryToKeySymConverter::advanceSequences(char input) {
		KeySym nextSym(KeySym::NONE);
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < stageCount; ++u) {
			InputConverter::Stage* stage = stages[u];
			if(!stage)
				continue;
			if(!stage->advanceStage(input)) {
				delete stage;
				stages[u] = NULL;
				--pathCount;
			}
			else {
				KeySym sym(stage->getCurrentSymbol());
				if(nextSym.getType() == KeySym::T_NONE && sym.getType() != KeySym::T_NONE)
					nextSym = sym;
				if(stage->isInputLeaf()) {
					delete stage;
					stages[u] = NULL;
					--pathCount;
				}
			}
		}
		if(nextSym.getType() != KeySym::T_NONE) {
			lastSymbol = nextSym;
			lastLength = currentDepth + static_cast<MemorySize>(1u);
		}
	}

	void BinaryToKeySymConverter::processSingles(char input) {
		SingleByteConverterIterator begin(singleByteConverters.begin()), end(singleByteConverters.end());
		for(; begin != end; ++begin) {
			KeySym sym((*begin)->byteToKeySym(input));
			if(lastSymbol.getType() == KeySym::T_NONE && sym.getType() != KeySym::T_NONE) {
				lastSymbol = sym;
				lastLength = static_cast<MemorySize>(1u);
			}
		}
	}

	void BinaryToKeySymConverter::flushChunks() {
		Chunk* chunk;
		while(!chunks.empty() && flushPointer + lastLength >= (chunk = chunks.front())->size) {
			chunks.pop_front();
			lastLength -= chunk->size - flushPointer;
			flushPointer = static_cast<MemorySize>(0u);
			delete chunk;
		}
		flushPointer += lastLength;
		lastLength = currentDepth = static_cast<MemorySize>(0u);
	}

	void BinaryToKeySymConverter::endPaths(char input) {
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < stageCount; ++u) {
			if(stages[u])
				delete stages[u];
		}
		if(stages)
			delete[] stages;
		if(lastSymbol.getType() == KeySym::T_NONE) {
			lastSymbol.assign(KeySym::T_GENERIC, KeySym::M_NONE, input);
			lastLength = static_cast<MemorySize>(1u);
		}
		symbolQueue.push_back(lastSymbol);
		lastSymbol = KeySym::NONE;
		flushChunks();
	}

	void BinaryToKeySymConverter::flushSequences() {
		if(lastSymbol.getType() == KeySym::T_NONE)
			return;
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < stageCount; ++u) {
			if(stages[u])
				delete stages[u];
		}
		if(stages)
			delete[] stages;
		symbolQueue.push_back(lastSymbol);
		lastSymbol = KeySym::NONE;
		flushChunks();
	}

	bool BinaryToKeySymConverter::hasPendingKey() const {
		return lastSymbol.getType() != KeySym::T_NONE;
	}

}}}
