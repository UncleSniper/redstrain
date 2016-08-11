#ifndef REDSTRAIN_MOD_DAMNATION_UNIXUTILS_SINGLEBYTECONVERTER_HPP
#define REDSTRAIN_MOD_DAMNATION_UNIXUTILS_SINGLEBYTECONVERTER_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "../KeySym.hpp"

namespace redengine {
namespace damnation {
namespace unixutils {

	class REDSTRAIN_DAMNATION_API SingleByteConverter : public util::ReferenceCounted {

	  public:
		SingleByteConverter();
		SingleByteConverter(const SingleByteConverter&);

		virtual KeySym byteToKeySym(char) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_UNIXUTILS_SINGLEBYTECONVERTER_HPP */
