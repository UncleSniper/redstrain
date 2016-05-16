#include <redstrain/text/Transcode.hpp>

#include "HierarchicalURISyntaxError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::util::MemorySize;

namespace redengine {
namespace vfs {

	HierarchicalURISyntaxError::HierarchicalURISyntaxError(const string& uri, MemorySize invalidPartOffset)
			: InvalidURIPartError(uri, invalidPartOffset) {}

	HierarchicalURISyntaxError::HierarchicalURISyntaxError(const String16& uri, MemorySize invalidPartOffset)
			: InvalidURIPartError(uri, invalidPartOffset) {}

	HierarchicalURISyntaxError::HierarchicalURISyntaxError(const String32& uri, MemorySize invalidPartOffset)
			: InvalidURIPartError(uri, invalidPartOffset) {}

	HierarchicalURISyntaxError::HierarchicalURISyntaxError(const HierarchicalURISyntaxError& error)
			: Error(error), InvalidURIPartError(error) {}

	REDSTRAIN_DEFINE_ERROR(HierarchicalURISyntaxError) {
		out << "Syntax error at character " << invalidPartOffset32
				<< " in hierarchical URI: " << Transcode::bmpToUTF8(getURI());
	}

}}
