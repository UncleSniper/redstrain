#ifndef REDSTRAIN_MOD_PLATFORM_SYNCHRONIZEDSINGLETON_HPP
#define REDSTRAIN_MOD_PLATFORM_SYNCHRONIZEDSINGLETON_HPP

#include "ObjectLocker.hpp"

namespace redengine {
namespace platform {

	template<typename SubjectT>
	class SynchronizedSingleton {

	  private:
		SubjectT* object;

	  protected:
		virtual SubjectT* newInstance() = 0;

	  public:
		SynchronizedSingleton() : object(NULL) {}
		SynchronizedSingleton(const SynchronizedSingleton& singleton) : object(singleton.object) {}

		SubjectT& get() {
			ObjectLocker<SubjectT*> locker(&object);
			if(!object)
				object = newInstance();
			locker.release();
			return *object;
		}

	};

	template<typename SubjectT, typename InstanceT = SubjectT>
	class DefaultSynchronizedSingleton : public SynchronizedSingleton<SubjectT> {

	  protected:
		virtual SubjectT* newInstance() {
			return new InstanceT;
		}

	  public:
		DefaultSynchronizedSingleton() {}

		DefaultSynchronizedSingleton(const DefaultSynchronizedSingleton& singleton)
				: SynchronizedSingleton<SubjectT>(singleton) {}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SYNCHRONIZEDSINGLETON_HPP */
