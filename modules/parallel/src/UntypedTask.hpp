#ifndef REDSTRAIN_MOD_PARALLEL_UNTYPEDTASK_HPP
#define REDSTRAIN_MOD_PARALLEL_UNTYPEDTASK_HPP

#include "api.hpp"

namespace redengine {
namespace parallel {

	class TaskCookie;
	class PooledThread;

	class REDSTRAIN_PARALLEL_API UntypedTask {

		PooledThread *volatile executingThread;
		TaskCookie *volatile queueCookie;

	  protected:
		UntypedTask(const UntypedTask&);

	  public:
		UntypedTask();
		virtual ~UntypedTask();

		inline PooledThread* getExecutingThread() const {
			return executingThread;
		}

		void setExecutingThread(PooledThread* thread) {
			executingThread = thread;
		}

		inline TaskCookie* getQueueCookie() const {
			return queueCookie;
		}

		inline void setQueueCookie(TaskCookie* cookie) {
			queueCookie = cookie;
		}

		virtual void executeInCurrentThread() = 0;
		virtual bool isTaskFinished() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_UNTYPEDTASK_HPP */
