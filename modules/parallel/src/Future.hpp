#ifndef REDSTRAIN_MOD_PARALLEL_FUTURE_HPP
#define REDSTRAIN_MOD_PARALLEL_FUTURE_HPP

#include <redstrain/util/WithAlign.hpp>
#include <redstrain/error/UncaughtExceptionError.hpp>
#include <redstrain/util/destructors.hpp>

#include "ThreadPool.hpp"
#include "UntypedTask.hpp"

namespace redengine {
namespace parallel {

	class TaskCookie;
	class PooledThread;

	template<
		typename ValueT,
		typename util::VoidableDestructorSignature<ValueT>::Destructor Destructor
				= util::ExplicitDestructor<ValueT>::destructor
	>
	class Future {

	  private:
		typedef typename util::WithAlign<util::AlignOf<ValueT>::ALIGNMENT>::Primitive AlignValue;

		enum State {
			S_PRISTINE,
			S_SUCCEEDED,
			S_FAILED
		};

	  private:
		ThreadPool& pool;
		union {
			char buffer[sizeof(ValueT)];
			AlignValue align;
		} value;
		error::Error *volatile error;
		volatile State state;
		UntypedTask* task;

	  protected:
		Future(const Future& future)
				: pool(future.pool), error(NULL), state(S_PRISTINE), task(NULL) {}

		ValueT& getValueDirect() {
			return *reinterpret_cast<ValueT*>(value.buffer);
		}

		const ValueT& getValueDirect() const {
			return *reinterpret_cast<const ValueT*>(value.buffer);
		}

		void setValue(const ValueT& newValue) {
			switch(state) {
				case S_SUCCEEDED:
					Destructor(*reinterpret_cast<ValueT*>(value.buffer));
					state = S_PRISTINE;
					break;
				case S_FAILED:
					if(error) {
						error->unref();
						error = NULL;
					}
					state = S_PRISTINE;
					break;
				case S_PRISTINE:
				default:
					break;
			}
			new(reinterpret_cast<ValueT*>(value.buffer)) ValueT(newValue);
			state = S_SUCCEEDED;
		}

	  public:
		Future(ThreadPool& pool) : pool(pool), error(NULL), state(S_PRISTINE), task(NULL) {}

		~Future() {
			switch(state) {
				case S_SUCCEEDED:
					Destructor(*reinterpret_cast<ValueT*>(value.buffer));
					break;
				case S_FAILED:
					if(error)
						error->unref();
					break;
				case S_PRISTINE:
				default:
					break;
			}
			if(task)
				delete task;
		}

		inline bool isFinished() const {
			return state != S_PRISTINE;
		}

		inline void setTask(UntypedTask& task) {
			if(!this->task)
				this->task = &task;
		}

		ValueT& force() {
			pool.force(*task);
			if(state == S_FAILED) {
				if(error)
					error->raise();
				else
					throw error::UncaughtExceptionError();
			}
			return getValueDirect();
		}

		void offer(const ValueT& finalValue) {
			setValue(finalValue);
		}

		void failed(const error::Error* error) {
			switch(state) {
				case S_SUCCEEDED:
					Destructor(*reinterpret_cast<ValueT*>(value.buffer));
					break;
				case S_FAILED:
					if(this->error) {
						this->error->unref();
						this->error = NULL;
					}
					break;
				case S_PRISTINE:
				default:
					break;
			}
			state = S_FAILED;
			this->error = NULL;
			if(error) {
				try {
					this->error = error->clone();
				}
				catch(...) {}
			}
		}

	};

	template<void (*Destructor)()>
	class Future<void, Destructor> {

	  private:
		enum State {
			S_PRISTINE,
			S_SUCCEEDED,
			S_FAILED
		};

	  private:
		ThreadPool& pool;
		error::Error *volatile error;
		volatile State state;
		UntypedTask* task;

	  protected:
		Future(const Future& future) : pool(future.pool), error(NULL), state(S_PRISTINE), task(NULL) {}

	  public:
		Future(ThreadPool& pool) : pool(pool), error(NULL), state(S_PRISTINE), task(NULL) {}

		~Future() {
			if(error)
				error->unref();
			if(task)
				delete task;
		}

		inline bool isFinished() const {
			return state != S_PRISTINE;
		}

		inline void setTask(UntypedTask& task) {
			if(!this->task)
				this->task = &task;
		}

		void force() {
			pool.force(*task);
			if(state == S_FAILED) {
				if(error)
					error->raise();
				else
					throw error::UncaughtExceptionError();
			}
		}

		void offer() {
			if(error) {
				error->unref();
				error = NULL;
			}
			state = S_SUCCEEDED;
		}

		void failed(const error::Error* error) {
			if(this->error) {
				this->error->unref();
				this->error = NULL;
			}
			state = S_FAILED;
			this->error = NULL;
			if(error) {
				try {
					this->error = error->clone();
				}
				catch(...) {}
			}
		}

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_FUTURE_HPP */
