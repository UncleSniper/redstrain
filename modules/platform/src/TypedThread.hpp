#ifndef REDSTRAIN_MOD_PLATFORM_TYPEDTHREAD_HPP
#define REDSTRAIN_MOD_PLATFORM_TYPEDTHREAD_HPP

#include "Thread.hpp"

namespace redengine {
namespace platform {

	template<typename ArgumentT = void, typename ResultT = void>
	class TypedThread : public Thread {

	  public:
		typedef ArgumentT Argument;
		typedef ResultT Result;

	  protected:
		virtual void* runImpl(void* user) {
			return run(reinterpret_cast<ArgumentT*>(user));
		}

		virtual ResultT* run(ArgumentT*) = 0;

	  public:
		TypedThread() {}
		TypedThread(const TypedThread& thread) : Thread(thread) {}

		void start(ArgumentT* user, bool detached = false) {
			startImpl(user, detached);
		}

		ResultT* join() {
			return reinterpret_cast<ResultT*>(joinImpl());
		}

	};

	template<typename ArgumentT>
	class TypedThread<ArgumentT, void> : public Thread {

	  public:
		typedef ArgumentT Argument;

	  protected:
		virtual void* runImpl(void* user) {
			run(reinterpret_cast<ArgumentT*>(user));
			return NULL;
		}

		virtual void run(ArgumentT*) = 0;

	  public:
		TypedThread() {}
		TypedThread(const TypedThread& thread) : Thread(thread) {}

		void start(ArgumentT* user, bool detached = false) {
			startImpl(user, detached);
		}

		void join() {
			joinImpl();
		}

	};

	template<typename ResultT>
	class TypedThread<void, ResultT> : public Thread {

	  public:
		typedef ResultT Result;

	  protected:
		virtual void* runImpl(void*) {
			return run();
		}

		virtual ResultT* run() = 0;

	  public:
		TypedThread() {}
		TypedThread(const TypedThread& thread) : Thread(thread) {}

		void start(bool detached = false) {
			startImpl(NULL, detached);
		}

		ResultT* join() {
			return reinterpret_cast<ResultT*>(joinImpl());
		}

	};

	template<>
	class TypedThread<void, void> : public Thread {

	  protected:
		virtual void* runImpl(void*) {
			run();
			return NULL;
		}

		virtual void run() = 0;

	  public:
		TypedThread() {}
		TypedThread(const TypedThread& thread) : Thread(thread) {}

		void start(bool detached = false) {
			startImpl(NULL, detached);
		}

		void join() {
			joinImpl();
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TYPEDTHREAD_HPP */
