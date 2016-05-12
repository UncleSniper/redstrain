#include <redstrain/text/Transcode.hpp>

#include "IllegalURIHeadError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::util::MemorySize;

namespace redengine {
namespace vfs {

	IllegalURIHeadError::IllegalURIHeadError(const string& uri, MemorySize illegalCharOffset)
			: InvalidURIPartError(uri, illegalCharOffset) {}

	IllegalURIHeadError::IllegalURIHeadError(const String16& uri, MemorySize illegalCharOffset)
			: InvalidURIPartError(uri, illegalCharOffset) {}

	IllegalURIHeadError::IllegalURIHeadError(const String32& uri, MemorySize illegalCharOffset)
			: InvalidURIPartError(uri, illegalCharOffset) {}

	IllegalURIHeadError::IllegalURIHeadError(const IllegalURIHeadError& error)
			: Error(error), InvalidURIPartError(error) {}

	REDSTRAIN_DEFINE_ERROR(IllegalURIHeadError) {
		out << "Illegal character in scheme-unspecific part of URI at character " << invalidPartOffset32
				<< " of '" << Transcode::bmpToUTF8(getURI()) << '\'';
	}

}}
