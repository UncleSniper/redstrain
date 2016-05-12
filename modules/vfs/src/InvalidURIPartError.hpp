#ifndef REDSTRAIN_MOD_VFS_INVALIDURIPARTERROR_HPP
#define REDSTRAIN_MOD_VFS_INVALIDURIPARTERROR_HPP

#include <redstrain/util/types.hpp>

#include "InvalidURIError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API InvalidURIPartError : public InvalidURIError {

	  private:
		const util::MemorySize invalidPartOffset;

	  protected:
		const util::MemorySize invalidPartOffset32;

	  public:
		InvalidURIPartError(const std::string&, util::MemorySize);
		InvalidURIPartError(const text::String16&, util::MemorySize);
		InvalidURIPartError(const text::String32&, util::MemorySize);
		InvalidURIPartError(const InvalidURIPartError&);

		inline util::MemorySize getInvalidPartOffset() const {
			return invalidPartOffset;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(InvalidURIPartError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_INVALIDURIPARTERROR_HPP */
