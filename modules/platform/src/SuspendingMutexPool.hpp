#ifndef REDSTRAIN_MOD_PLATFORM_SUSPENDINGMUTEXPOOL_HPP
#define REDSTRAIN_MOD_PLATFORM_SUSPENDINGMUTEXPOOL_HPP

#include <set>

#include "MutexPool.hpp"
#include "ConditionVariable.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SuspendingMutexPool : public MutexPool {

	  private:
		typedef std::set<const void*> PointerSet;

		struct Shard {

			ConditionVariable condvar;
			PointerSet locked;

		};

	  private:
		unsigned size;
		Shard* shards;

	  private:
		unsigned indexByAddress(const void*) const;

	  protected:
		virtual void lockObjectImpl(const void*);
		virtual void unlockObjectImpl(const void*);
		virtual void uncheckedUnlockObjectImpl(const void*);

	  public:
		SuspendingMutexPool(unsigned = 0u);
		SuspendingMutexPool(const SuspendingMutexPool&);
		virtual ~SuspendingMutexPool();

		inline unsigned getSize() const {
			return size;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SUSPENDINGMUTEXPOOL_HPP */
