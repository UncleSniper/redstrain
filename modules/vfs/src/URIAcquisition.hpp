#ifndef REDSTRAIN_MOD_VFS_URIACQUISITION_HPP
#define REDSTRAIN_MOD_VFS_URIACQUISITION_HPP

#include <redstrain/io/OutputStream.hpp>

namespace redengine {
namespace vfs {

	template<typename ResourceT>
	class URIAcquisition {

	  public:
		URIAcquisition() {}
		URIAcquisition(const URIAcquisition&) {}
		virtual ~URIAcquisition() {}

		virtual void requestResource() = 0;
		virtual io::OutputStream<char>* getRequestStream() = 0;
		virtual ResourceT* getAcquiredResource() = 0;
		virtual void releaseAcquiredResource() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URIACQUISITION_HPP */
