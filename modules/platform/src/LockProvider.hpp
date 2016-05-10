#ifndef REDSTRAIN_MOD_PLATFORM_LOCKPROVIDER_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCKPROVIDER_HPP

namespace redengine {
namespace platform {

	template<typename SubjectT>
	class LockProvider {

	  public:
		LockProvider() {}
		LockProvider(const LockProvider&) {}
		virtual ~LockProvider() {}

		virtual void lockObject(const SubjectT*) = 0;
		virtual void unlockObject(const SubjectT*) = 0;
		virtual void uncheckedUnlockObject(const SubjectT*) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCKPROVIDER_HPP */
