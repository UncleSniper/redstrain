#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "FileBase.hpp"

using std::string;
using redengine::platform::File;
using redengine::util::StringUtils;
using redengine::error::IllegalArgumentError;

namespace redengine {
namespace io {

	FileBase::FileBase(const FileBase& stream) : Stream(stream), file(stream.file), closeOnDestroy(false) {}

	FileBase::FileBase(const string& path, File::Direction direction, bool create, File::TruncateMode truncate)
			: file(File::INVALID_HANDLE, direction) {
		file.open(path, direction, create, truncate);
	}

	FileBase::FileBase(const File& file) : file(file), closeOnDestroy(true) {}

	FileBase::FileBase(File::Handle handle, File::Direction dir) : file(handle, dir), closeOnDestroy(true) {}

	FileBase::~FileBase() {
		if(closeOnDestroy && file.getHandle() != File::INVALID_HANDLE)
			file._uncheckedClose();
	}

	void FileBase::close() {
		if(file.getHandle() != File::INVALID_HANDLE)
			file.close();
	}

	void FileBase::seek(off_t offset, SeekWhence whence) {
		File::SeekWhence platformWhence;
		switch(whence) {
			case OFFSET_FROM_START:
				platformWhence = File::OFFSET_FROM_START;
				break;
			case OFFSET_FROM_END:
				platformWhence = File::OFFSET_FROM_END;
				break;
			case OFFSET_FROM_HERE:
				platformWhence = File::OFFSET_FROM_HERE;
				break;
			default:
				throw IllegalArgumentError("Not a valid SeekWhence: "
						+ StringUtils::toString(static_cast<int>(whence)));
		}
		file.seek(offset, platformWhence);
	}

	size_t FileBase::tell() const {
		return file.tell();
	}

}}
