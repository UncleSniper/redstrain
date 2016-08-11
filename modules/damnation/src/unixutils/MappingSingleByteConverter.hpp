#ifndef REDSTRAIN_MOD_DAMNATION_UNIXUTILS_MAPPINGSINGLEBYTECONVERTER_HPP
#define REDSTRAIN_MOD_DAMNATION_UNIXUTILS_MAPPINGSINGLEBYTECONVERTER_HPP

#include <map>

#include "SingleByteConverter.hpp"

namespace redengine {
namespace damnation {
namespace unixutils {

	class REDSTRAIN_DAMNATION_API MappingSingleByteConverter : public SingleByteConverter {

	  private:
		typedef std::map<char, KeySym> Keys;

	  public:
		typedef Keys::const_iterator KeyIterator;

	  private:
		Keys keys;

	  public:
		MappingSingleByteConverter();
		MappingSingleByteConverter(const MappingSingleByteConverter&);
		virtual ~MappingSingleByteConverter();

		void getMappedKeys(KeyIterator&, KeyIterator&) const;
		void mapKey(char, const KeySym&);
		bool unmapKey(char);
		void clear();

		void registerControlKeys();

		virtual KeySym byteToKeySym(char);

		static SingleByteConverter& getControlKeysConverter();
		static SingleByteConverter& getAlternateBackspaceConverter();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_UNIXUTILS_MAPPINGSINGLEBYTECONVERTER_HPP */
