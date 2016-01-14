#ifndef REDSTRAIN_MOD_IO_FILEBASE_HPP
#define REDSTRAIN_MOD_IO_FILEBASE_HPP

#include "Stream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API FileBase : public virtual Stream {

	  protected:
		platform::File file;

	  private:
		bool closeOnDestroy;

	  protected:
		FileBase(const FileBase&);

	  public:
		FileBase(const std::string&, platform::File::Direction, bool, platform::File::TruncateMode);
		FileBase(const platform::File&);
		FileBase(platform::File::Handle, platform::File::Direction);
		virtual ~FileBase();

		inline const platform::File& getFile() const {
			return file;
		}

		inline platform::File& getFile() {
			return file;
		}

		inline bool closesOnDestroy() const {
			return closeOnDestroy;
		}

		inline void setCloseOnDestroy(bool shouldCloseOnDestroy) {
			closeOnDestroy = shouldCloseOnDestroy;
		}

		virtual void close();
		virtual void seek(util::FileOffset, SeekWhence);
		virtual util::FileSize tell() const;

	};

}}

#endif /* REDSTRAIN_MOD_IO_FILEBASE_HPP */
