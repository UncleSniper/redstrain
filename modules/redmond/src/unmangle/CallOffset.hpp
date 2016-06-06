#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_CALLOFFSET_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_CALLOFFSET_HPP

#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API CallOffset {

	  private:
		bool _virtual;
		int offset, virtualOffset;

	  public:
		CallOffset(int);
		CallOffset(int, int);
		CallOffset(bool, int, int);
		CallOffset(const CallOffset&);

		inline bool isVirtual() const {
			return _virtual;
		}

		inline int getOffset() const {
			return offset;
		}

		inline int getVirtualOffset() const {
			return virtualOffset;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_CALLOFFSET_HPP */
