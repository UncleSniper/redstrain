#ifndef REDSTRAIN_MOD_IO_FILESTREAM_HPP
#define REDSTRAIN_MOD_IO_FILESTREAM_HPP

#include "FileInputStream.hpp"
#include "FileOutputStream.hpp"
#include "BidirectionalStream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API FileStream
			: public virtual FileInputStream, public virtual FileOutputStream, public BidirectionalStream<char> {

	  protected:
		FileStream(const FileStream&);

	  public:
		FileStream(const std::string&, bool = true);
		FileStream(const platform::File&);
		FileStream(platform::File::Handle);

	};

}}

#endif /* REDSTRAIN_MOD_IO_FILESTREAM_HPP */
