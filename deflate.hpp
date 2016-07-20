#ifndef REDSTRAIN_DEFLATE_HPP
#define REDSTRAIN_DEFLATE_HPP

namespace redengine {
namespace compress {

	enum DeflateBlockType {
		DBT_NO_COMPRESSION,
		DBT_FIXED_HUFFMAN,
		DBT_DYNAMIC_HUFFMAN,
		DBT_RESERVED
	};

}}

#endif /* REDSTRAIN_DEFLATE_HPP */
