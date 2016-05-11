#ifndef REDSTRAIN_MOD_VFS_URISCHEME_HPP
#define REDSTRAIN_MOD_VFS_URISCHEME_HPP

#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/text/types.hpp>

#include "api.hpp"

namespace redengine {
namespace vfs {

	class URI;

	class REDSTRAIN_VFS_API URIScheme : public util::ReferenceCounted {

	  public:
		URIScheme();
		URIScheme(const URIScheme&);

		virtual URI* newURI(const std::string&, std::string::size_type) = 0;
		virtual URI* newURI(const text::String16&, text::String16::size_type) = 0;
		virtual URI* newURI(const text::String32&, text::String32::size_type) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URISCHEME_HPP */
