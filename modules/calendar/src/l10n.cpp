#include <redstrain/locale/BlobMessageLoader.hpp>
#include <redstrain/text/types.hpp>

using redengine::text::Char16;
using redengine::locale::MessageLoader;
using redengine::locale::BlobMessageLoader;
using redengine::locale::BlobMessageMapping;

namespace redengine {
namespace calendar {

	BlobMessageMapping* formatMessage16BlobMapping = NULL;

	MessageLoader<Char16>* newFormatBlobMessageLoader16() {
		if(!formatMessage16BlobMapping)
			return NULL;
		return new BlobMessageLoader<Char16>(*formatMessage16BlobMapping);
	}

}}
