#ifndef REDSTRAIN_MOD_PARALLEL_POOLEDTHREAD_HPP
#define REDSTRAIN_MOD_PARALLEL_POOLEDTHREAD_HPP

#include "api.hpp"

namespace redengine {
namespace parallel {

	class UntypedTask;

	class REDSTRAIN_PARALLEL_API PooledThread {

	  public:
		PooledThread();
		PooledThread(const PooledThread&);
		virtual ~PooledThread();

		virtual bool isCallingThread() = 0;
		virtual void waitForTaskCompletion(UntypedTask&) = 0;
		virtual void forcingTask(UntypedTask&, bool) = 0;
		virtual bool requestNotifyOnTaskCompletion(PooledThread&, UntypedTask&) = 0;
		virtual void notifyTaskCompleted(UntypedTask&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_POOLEDTHREAD_HPP */
