#ifndef REDSTRAIN_MOD_VFS_URIACQUIRER_HPP
#define REDSTRAIN_MOD_VFS_URIACQUIRER_HPP

#include <redstrain/io/InputStream.hpp>

#include "URIAcquisition.hpp"

namespace redengine {
namespace vfs {

	class VFS;

	template<typename SpecifyingURIT>
	class URIAcquirer {

	  public:
		typedef SpecifyingURIT SpecifyingURI;

	  public:
		URIAcquirer() {}
		URIAcquirer(const URIAcquirer&) {}
		virtual ~URIAcquirer() {}

		virtual URIAcquisition<io::InputStream<char> >* acquireResource(const SpecifyingURIT&) const = 0;
		virtual URIAcquisition<VFS>* acquireVFS(const SpecifyingURIT&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URIACQUIRER_HPP */
