#include "CallOffset.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	CallOffset::CallOffset(int offset) : _virtual(false), offset(offset), virtualOffset(0) {}

	CallOffset::CallOffset(int offset, int virtualOffset)
			: _virtual(true), offset(offset), virtualOffset(virtualOffset) {}

	CallOffset::CallOffset(bool _virtual, int offset, int virtualOffset)
			: _virtual(_virtual), offset(offset), virtualOffset(virtualOffset) {}

	CallOffset::CallOffset(const CallOffset& call)
			: _virtual(call._virtual), offset(call.offset), virtualOffset(call.virtualOffset) {}

}}}
