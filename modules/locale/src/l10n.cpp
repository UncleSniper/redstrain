#include "l10n.hpp"
#include "BlobMessageLoader.hpp"

using redengine::text::Char16;

namespace redengine {
namespace locale {

	BlobMessageMapping* coreMessage16BlobMapping = NULL;

	MessageLoader<Char16>* newCoreBlobMessageLoader16() {
		if(!coreMessage16BlobMapping)
			return NULL;
		return new BlobMessageLoader<Char16>(*coreMessage16BlobMapping);
	}

	BlobMessageMapping* stackTraceMessage16BlobMapping = NULL;

	MessageLoader<Char16>* newStackTraceBlobMessageLoader16() {
		if(!stackTraceMessage16BlobMapping)
			return NULL;
		return new BlobMessageLoader<Char16>(*stackTraceMessage16BlobMapping);
	}

}}
