#ifndef REDSTRAIN_DEFLATE_HPP
#define REDSTRAIN_DEFLATE_HPP

namespace redengine {
namespace compress {

	enum DeflateFunction {
		DFL_F_STORE,
		DFL_F_FAST,
		DFL_F_SLOW
	};

	enum DeflateBlockType {
		DFL_BT_NO_COMPRESSION,
		DFL_BT_FIXED_HUFFMAN,
		DFL_BT_DYNAMIC_HUFFMAN,
		DFL_BT_RESERVED
	};

}}

#endif /* REDSTRAIN_DEFLATE_HPP */
