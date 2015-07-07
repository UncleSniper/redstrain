#ifndef REDSTRAIN_MOD_PLATFORM_SIMPLEMUTEXPOOL_HPP
#define REDSTRAIN_MOD_PLATFORM_SIMPLEMUTEXPOOL_HPP

#include "MutexPool.hpp"

namespace redengine {
namespace platform {

	class Mutex;

	class REDSTRAIN_PLATFORM_API SimpleMutexPool : public MutexPool {

	  private:
		unsigned size;
		Mutex* mutexes;

	  private:
		unsigned indexByAddress(const void*) const;

	  protected:
		virtual void lockObjectImpl(const void*);
		virtual void unlockObjectImpl(const void*);
		virtual void uncheckedUnlockObjectImpl(const void*);

	  public:
		SimpleMutexPool(unsigned = 0u);
		SimpleMutexPool(const SimpleMutexPool&);
		virtual ~SimpleMutexPool();

		inline unsigned getSize() const {
			return size;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SIMPLEMUTEXPOOL_HPP */
