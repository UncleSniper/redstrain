#ifndef REDSTRAIN_MOD_PARALLEL_EXECUTOR_HPP
#define REDSTRAIN_MOD_PARALLEL_EXECUTOR_HPP

#include <redstrain/util/Delete.hpp>

#include "Task.hpp"
#include "Future.hpp"
#include "UntypedTask.hpp"

namespace redengine {
namespace parallel {

	class ThreadPool;

	class REDSTRAIN_PARALLEL_API Executor {

	  private:
		template<
			typename ResultT,
			typename util::VoidableDestructorSignature<ResultT>::Destructor Destructor
		>
		class TypedTask : public UntypedTask {

		  public:
			Task<ResultT>& task;
			Future<ResultT, Destructor>& future;

		  protected:
			TypedTask(const TypedTask& task) : UntypedTask(task), task(task.task), future(task.future) {}

		  public:
			TypedTask(Task<ResultT>& task, Future<ResultT, Destructor>& future) : task(task), future(future) {}

			virtual ~TypedTask() {
				delete &task;
			}

			virtual void executeInCurrentThread() {
				try {
					future.offer(task.run());
				}
				catch(const error::Error& error) {
					future.failed(&error);
				}
				catch(...) {
					future.failed(NULL);
				}
			}

			virtual bool isTaskFinished() {
				return future.isFinished();
			}

		};

		template<void (*Destructor)()>
		class TypedTask<void, Destructor> : public UntypedTask {

		  public:
			Task<void>& task;
			Future<void, Destructor>& future;

		  protected:
			TypedTask(const TypedTask& task) : UntypedTask(task), task(task.task), future(task.future) {}

		  public:
			TypedTask(Task<void>& task, Future<void, Destructor>& future) : task(task), future(future) {}

			virtual ~TypedTask() {
				delete &task;
			}

			virtual void executeInCurrentThread() {
				try {
					task.run();
					future.offer();
				}
				catch(const error::Error& error) {
					future.failed(&error);
				}
				catch(...) {
					future.failed(NULL);
				}
			}

			virtual bool isTaskFinished() {
				return future.isFinished();
			}

		};

	  private:
		ThreadPool& threadPool;

	  protected:
		Executor(const Executor&);

	  private:
		void submitTask(UntypedTask&);

	  public:
		Executor(ThreadPool&);
		~Executor();

		inline ThreadPool& getThreadPool() {
			return threadPool;
		}

		inline const ThreadPool& getThreadPool() const {
			return threadPool;
		}

		template<typename ResultT, typename util::VoidableDestructorSignature<ResultT>::Destructor Destructor>
		Future<ResultT, Destructor>* execute(Task<ResultT>& task) {
			util::Delete<Task<ResultT> > deleteTask(&task);
			util::Delete<Future<ResultT, Destructor> > future(new Future<ResultT, Destructor>(threadPool));
			util::Delete<TypedTask<ResultT, Destructor> > tt(new TypedTask<ResultT, Destructor>(task, **future));
			deleteTask.set();
			TypedTask<ResultT, Destructor>& rawTT = *tt.set();
			future->setTask(rawTT);
			submitTask(rawTT);
			return future.set();
		}

		template<typename ResultT>
		Future<ResultT, util::ExplicitDestructor<ResultT>::destructor>* execute(Task<ResultT>& task) {
			return execute<ResultT, util::ExplicitDestructor<ResultT>::destructor>(task);
		}

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_EXECUTOR_HPP */
