#ifndef REDSTRAIN_MOD_VFS_UNSUPPORTEDFILETYPEERROR_HPP
#define REDSTRAIN_MOD_VFS_UNSUPPORTEDFILETYPEERROR_HPP

#include "Stat.hpp"
#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API UnsupportedFileTypeError : public VFSIOError {

	  private:
		const Stat::Type type;

	  public:
		UnsupportedFileTypeError(Stat::Type);
		UnsupportedFileTypeError(const UnsupportedFileTypeError&);

		inline Stat::Type getFileType() const {
			return type;
		}

		REDSTRAIN_DECLARE_ERROR(UnsupportedFileTypeError)

	};

}}


#endif /* REDSTRAIN_MOD_VFS_UNSUPPORTEDFILETYPEERROR_HPP */
