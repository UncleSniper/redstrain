#include "BlobMessageMapping.hpp"
#include "MessageBlobRegistrar.hpp"

using std::string;

namespace redengine {
namespace locale {

	MessageBlobRegistrar::MessageBlobRegistrar(BlobMessageMapping*& mapping,
			const string& language, const string& country, const char* data, size_t size) {
		if(!mapping)
			mapping = new BlobMessageMapping;
		mapping->addBlob(language, country, data, size);
	}

}}
