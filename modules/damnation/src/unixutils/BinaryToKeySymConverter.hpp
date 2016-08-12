#ifndef REDSTRAIN_MOD_DAMNATION_UNIXUTILS_BINARYTOKEYSYMCONVERTER_HPP
#define REDSTRAIN_MOD_DAMNATION_UNIXUTILS_BINARYTOKEYSYMCONVERTER_HPP

#include <list>
#include <redstrain/util/types.hpp>

#include "../KeySym.hpp"

namespace redengine {
namespace damnation {
namespace unixutils {

	class InputConverter;
	class SingleByteConverter;

	class REDSTRAIN_DAMNATION_API BinaryToKeySymConverter {

	  private:
		struct Chunk {

			char* bytes;
			util::MemorySize size;

			Chunk(const char*, util::MemorySize);
			Chunk(const Chunk&);
			~Chunk();

		};

		typedef std::list<InputConverter*> InputConverters;
		typedef std::list<SingleByteConverter*> SingleByteConverters;
		typedef std::list<Chunk*> Chunks;
		typedef std::list<KeySym> KeySyms;

	  public:
		typedef InputConverters::const_iterator InputConverterIterator;
		typedef SingleByteConverters::const_iterator SingleByteConverterIterator;

	  private:
		InputConverters inputConverters;
		SingleByteConverters singleByteConverters;
		InputConverter::Stage** stages;
		util::MemorySize stageCount;
		KeySym lastSymbol;
		util::MemorySize lastLength;
		Chunks chunks;
		KeySyms symbolQueue;
		util::MemorySize flushPointer, currentDepth, pathCount;

	  private:
		bool feedChunkQueue();
		bool feedByte(char);
		void beginSequences();
		void advanceSequences(char);
		void processSingles(char);
		void endPaths(char);
		void flushChunks();

	  public:
		BinaryToKeySymConverter();
		BinaryToKeySymConverter(const BinaryToKeySymConverter&);
		~BinaryToKeySymConverter();

		void getInputConverters(InputConverterIterator&, InputConverterIterator&) const;
		void addInputConverter(InputConverter&);
		bool removeInputConverter(InputConverter&);
		void clearInputConverters();

		void getSingleByteConverters(SingleByteConverterIterator&, SingleByteConverterIterator&) const;
		void addSingleByteConverter(SingleByteConverter&);
		bool removeSingleByteConverter(SingleByteConverter&);
		void clearSingleByteConverters();

		KeySym nextKey();
		void feedInput(const char*, util::MemorySize);
		void flushSequences();
		bool hasPendingKey() const;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_UNIXUTILS_BINARYTOKEYSYMCONVERTER_HPP */
