#include <redstrain/util/StringUtils.hpp>

#include "FileBase.hpp"
#include "IllegalSeekWhenceError.hpp"

using std::string;
using redengine::platform::File;
using redengine::util::FileSize;
using redengine::util::FileOffset;
using redengine::util::StringUtils;

namespace redengine {
namespace io {

	FileBase::FileBase(const FileBase& stream) : Stream(stream), file(stream.file), closeOnDestroy(false) {}

	FileBase::FileBase(const string& path, File::Direction direction, bool create, File::TruncateMode truncate)
			: file(File::INVALID_HANDLE, direction), closeOnDestroy(true) {
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

	void FileBase::seek(FileOffset offset, SeekWhence whence) {
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
				throw IllegalSeekWhenceError(whence);
		}
		file.seek(offset, platformWhence);
	}

	FileSize FileBase::tell() const {
		return file.tell();
	}

}}
