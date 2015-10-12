#ifndef REDSTRAIN_MOD_TEXT_BLOBCODETABLE16REGISTRAR_HPP
#define REDSTRAIN_MOD_TEXT_BLOBCODETABLE16REGISTRAR_HPP

#include <map>

#include "api.hpp"

namespace redengine {
namespace text {

	class CodecManager;

	class REDSTRAIN_TEXT_API BlobCodeTable16Registrar {

	  public:
		BlobCodeTable16Registrar(const char*, size_t, const char *const*);

		static void registerCodecs(CodecManager&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_BLOBCODETABLE16REGISTRAR_HPP */
