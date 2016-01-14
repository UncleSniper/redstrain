#ifndef REDSTRAIN_MOD_IO_STREAM_HPP
#define REDSTRAIN_MOD_IO_STREAM_HPP

#include <redstrain/platform/File.hpp>

#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API Stream {

	  public:
		enum SeekWhence {
			OFFSET_FROM_START,
			OFFSET_FROM_END,
			OFFSET_FROM_HERE
		};

	  protected:
		Stream(const Stream&);

	  public:
		Stream();
		virtual ~Stream();

		virtual void close();
		virtual void seek(util::FileOffset, SeekWhence);
		virtual util::FileSize tell() const;

	};

}}

#endif /* REDSTRAIN_MOD_IO_STREAM_HPP */
