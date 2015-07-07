#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXPOOL_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXPOOL_HPP

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API MutexPool {

	  protected:
		virtual void lockObjectImpl(const void*) = 0;
		virtual void unlockObjectImpl(const void*) = 0;
		virtual void uncheckedUnlockObjectImpl(const void*) = 0;

	  public:
		MutexPool();
		MutexPool(const MutexPool&);
		virtual ~MutexPool();

		template<typename SubjectT>
		void lockObject(const SubjectT* object) {
			if(object)
				lockObjectImpl(reinterpret_cast<const void*>(object));
		}

		template<typename SubjectT>
		void unlockObject(const SubjectT* object) {
			if(object)
				unlockObjectImpl(reinterpret_cast<const void*>(object));
		}

		template<typename SubjectT>
		void uncheckedUnlockObject(const SubjectT* object) {
			if(object)
				uncheckedUnlockObjectImpl(reinterpret_cast<const void*>(object));
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXPOOL_HPP */
