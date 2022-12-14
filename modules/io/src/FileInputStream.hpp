#ifndef REDSTRAIN_MOD_IO_FILEINPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_FILEINPUTSTREAM_HPP

#include "FileBase.hpp"
#include "InputStream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API FileInputStream : public virtual FileBase, public virtual InputStream<char> {

	  protected:
		FileInputStream(const FileInputStream&);

		virtual util::MemorySize readBlock(char*, util::MemorySize);

	  public:
		FileInputStream(const std::string&, platform::File::Direction = platform::File::INPUT, bool = false,
				platform::File::TruncateMode = platform::File::TRUNCATE_IF_WRONLY);
		FileInputStream(const platform::File&);
		FileInputStream(platform::File::Handle, platform::File::Direction = platform::File::INPUT);

		virtual void seek(util::FileOffset, SeekWhence);

	};

}}

#endif /* REDSTRAIN_MOD_IO_FILEINPUTSTREAM_HPP */
