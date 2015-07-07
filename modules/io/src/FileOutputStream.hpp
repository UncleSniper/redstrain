#ifndef REDSTRAIN_MOD_IO_FILEOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_FILEOUTPUTSTREAM_HPP

#include "FileBase.hpp"
#include "OutputStream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API FileOutputStream : public virtual FileBase, public virtual OutputStream<char> {

	  protected:
		FileOutputStream(const FileOutputStream&);

		virtual void writeBlock(const char*, size_t);

	  public:
		FileOutputStream(const std::string&, platform::File::Direction = platform::File::OUTPUT, bool = true,
				platform::File::TruncateMode = platform::File::TRUNCATE_IF_WRONLY);
		FileOutputStream(const platform::File&);
		FileOutputStream(const platform::File::Handle handle, platform::File::Direction = platform::File::OUTPUT);

	};

}}

#endif /* REDSTRAIN_MOD_IO_FILEOUTPUTSTREAM_HPP */
