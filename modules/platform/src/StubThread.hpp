#ifndef REDSTRAIN_MOD_PLATFORM_STUBTHREAD_HPP
#define REDSTRAIN_MOD_PLATFORM_STUBTHREAD_HPP

#include "TypedThread.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API StubThread : public TypedThread<> {

	  protected:
		virtual void run();

	  private:
		StubThread(Handle handle);

	  public:
		StubThread(const StubThread&);

		/**
		 * TODO.
		 */
		static StubThread self();

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_STUBTHREAD_HPP */
