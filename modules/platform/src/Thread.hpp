#ifndef REDSTRAIN_MOD_PLATFORM_THREAD_HPP
#define REDSTRAIN_MOD_PLATFORM_THREAD_HPP

#include <string>
#include <stdint.h>

#include "api.hpp"
#include "platform.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <pthread.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <windows.h>
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

	/**
	 * Representation of a single thread of control.
	 * Such a representation allows the client to deal in
	 * threads as objects, including the creation of new
	 * threads by @link startImpl(void*, bool) starting@endlink
	 * execution of code asynchronously and
	 * @link joinImpl() waiting@endlink for execution to
	 * finish. The underlying platform is ignorant of
	 * such representation and shall remain so; in particular,
	 * the lifecycles of an actual thread and an instance
	 * of Thread are independent of each other:
	 * - An instance is typically created ahead of time,
	 *   when the corresponding thread has not yet been
	 *   created (although this is
	 *   @link StubThread::self() not a necessity@endlink).
	 *   In this case, such an instance does not represent
	 *   an actual thread until one is created by calling
	 *   @link startImpl(void*, bool) startImpl@endlink.
	 * - When an instance is destroyed, the thread it
	 *   represents (if any) remains unaffected.
	 * - When a thread terminates, any instances representing
	 *   it (if any) remain unaffected.
	 *
	 * @platassume
	 * - A thread is created by providing a callback function
	 *   representing the "body" of the thread; that function
	 *   is immediately (i.e. without the need for further
	 *   interaction) called asynchronously upon creation.
	 *   When that call to the function returns, the thread
	 *   terminates.
	 * - Thread creation may be requested to be @em joinable
	 *   or <tt>detached</tt>. If a thread is <tt>joinable</tt>,
	 *   another thread may wait for it to terminate by calling
	 *   @link joinImpl() joinImpl@endlink; if the thread to be
	 *   waited for is <em>detached</em>, that operation is
	 *   not permitted. Depending on the platform, @em detached
	 *   threads might be lighter on system resources; it
	 *   is therefore recommended to create a thread as
	 *   @em detached if and only if it is known at design time
	 *   that it will never be joined. Note that not all
	 *   platforms support @em detached threads and the
	 *   implementation may ignore the state request, always
	 *   creating @em joinable threads.
	 * - The body function of a thread takes an untyped pointer
	 *   (@c void*) as its sole argument and returns such a
	 *   pointer. The argument is specified by the creating
	 *   thread in calling
	 *   @link startImpl(void*, bool) startImpl@endlink; the
	 *   return value may be obtained by the joining thread
	 *   in calling @link joinImpl() joinImpl@endlink. For
	 *   platforms that do not support this behavior, the
	 *   implementation adds in this support; clients may
	 *   therefore rely on this setup.
	 *
	 * @statemodel
	 * - An instance is @link Thread() typically@endlink
	 *   created in @c PRISTINE state, at which point it does
	 *   not represent an actual thread.
	 * - Starting a thread can only be achieved by calling
	 *   @link startImpl(void*, bool) startImpl@endlink, which
	 *   is only permitted if the instance is in @c PRISTINE
	 *   state. Depending on the requested detach state (and
	 *   platform support), the instance is put either in
	 *   @c JOINABLE or @c DETACHED state upon successful
	 *   completion of that call.
	 * - If the running thread terminates asynchronously,
	 *   instances representing it remain unaffected and
	 *   no nominal state change occurs.
	 * - Another thread may synchronize on the termination
	 *   of the thread in question by calling
	 *   @link joinImpl() joinImpl@endlink. This operation is
	 *   only permitted if the instance is in @c JOINABLE state.
	 *   Upon successful completion of the call, the instance
	 *   is put in @c TERMINATED state.
	 * - Another thread may forcibly asynchronously terminate
	 *   the thread in question (although this is
	 *   <b>strongly discouraged</b>) by calling
	 *   @link kill() kill@endlink. This operation is only
	 *   permitted if the instance is in @c JOINABLE or
	 *   @c DETACHED state. Upon successful completion of the
	 *   call, the instance is put in @c TERMINATED state.
	 * - Once in @c TERMINATED state, no further thread
	 *   control operations (start, join, kill) are permitted
	 *   and the instance should be destroyed.
	 *
	 * Note that having multiple instances represent the same
	 * thread may cause the nominal state as defined above to
	 * become out of sync with the platform-internal state of
	 * the actual thread. It is the responsibility of the client
	 * to ensure that nominal state remains suitable for the
	 * desired control operations.
	 *
	 * @hiernotes
	 * - The Thread class is an abstract base encapsulating
	 *   the thread control and state tracking mechanisms
	 *   only. Derived classes must implement
	 *   @link runImpl(void*) runImpl@endlink, which
	 *   represents the thread body function as defined above.
	 * - The untyped pointer semantics are exposed (albeit
	 *   only to derived classes) by this encapsulation,
	 *   making the argument and return value passing interface
	 *   non-type-safe. Clients should use/inherit
	 *   @link TypedThread@endlink/@link CallbackThread@endlink
	 *   instead as a type-safe alternative.
	 *
	 * @since 0.1.0
	 */
	class REDSTRAIN_PLATFORM_API Thread {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef pthread_t Handle;
		typedef int ErrorCode;
		typedef void* BodyReturn;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		typedef DWORD Handle;
		typedef DWORD ErrorCode;
		typedef DWORD BodyReturn;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_DOXYGEN
		/**
		 * Platform thread identifier. An instance of this type
		 * indicates which actual thread an instance of Thread
		 * represents. On all modern platforms, this is a
		 * primitive integral type, but clients should treat
		 * it as opaque in the interest of portability. As
		 * such, the only operations guaranteed to be supported
		 * by this type are copy-construction (including
		 * pass-by-value) and destruction.
		 *
		 * @see getHandle()
		 * @since 0.1.0
		 */
		typedef PlatformSpecific Handle;
		/**
		 * Thread operation error code. If a thread control
		 * operation fails at the platform level, the platform
		 * will supply an error code indicating what prevented
		 * the operation from succeeding. Such an error code
		 * will be exposed to the client by the exception thrown
		 * in such a case. On all modern platforms, this is a
		 * primitive integral type, but clients should treat
		 * it as opaque in the interest of portability. As
		 * such, the only operations guaranteed to be supported
		 * by this type are copy-construction (including
		 * pass-by-value), destruction and mapping an error
		 * code to a human-readable error message via
		 * @link getErrorMessage(ErrorCode) getErrorMessage@endlink.
		 *
		 * @see getErrorMessage(ErrorCode), ThreadOperationError
		 * @since 0.1.0
		 */
		typedef PlatformSpecific ErrorCode;
		/**
		 * Return type of platform native thread bodies. That
		 * is, the actual body function supplied to the thread
		 * creation operation at the platform level returns
		 * an instance of this type. Note that this function is
		 * an implementation detail and will call the client-supplied
		 * body function returning @c void* appropriately.
		 * Therefore, the definition of this type is of no
		 * consequence to clients and is merely exposed for
		 * completeness. Also note that, as a consequence, it
		 * is unportable to rely on this type.
		 *
		 * @since 0.1.0
		 */
		typedef PlatformSpecific BodyReturn;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

		enum State {
			PRISTINE,
			JOINABLE,
			DETACHED,
			TERMINATED
		};

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
	  private:
		void* bodyResult;
#endif /* indirect runImpl() result */

	  protected:
		/**
		 * Platform identifier of represented thread. If this
		 * Thread is in @link getState() nominal state@endlink
		 * @c JOINABLE or <tt>DETACHED</tt>, this value identifies
		 * the thread it represents in a manner suitable for
		 * the implementation of thread control operations.
		 * In any other nominal state, the value is undefined
		 * and must not be used to perform such control operations.
		 * Note that this means the value is subject to change;
		 * in particular, @link startImpl(void*, bool) starting@endlink
		 * the thread will set this to the resulting identifier.
		 *
		 * @see getHandle()
		 * @since 0.1.0
		 */
		Handle handle;
		/**
		 * Nominal state of this representation. This value is
		 * subject to change by thread control operations;
		 * see the section "State Model" in the
		 * @link Thread class description@endlink.
		 *
		 * @see getState()
		 * @since 0.1.0
		 */
		State state;

	  public:
		/**
		 * Symbolic body return value for uncaught exceptions.
		 * If the thread body throws an exception that would
		 * otherwise propagate out of the thread, the thread
		 * result as reported by @link joinImpl() joinImpl@endlink
		 * will be @c UNCAUGHT_EXCEPTION to indicate that
		 * situation to other threads. Note that the actual
		 * value of this constant is meaningless and the
		 * pointer may not be dereferenced; the only guarantee
		 * is that it is unique.
		 *
		 * A typical use case is therefore:
		 * @code
		   CallbackThread<void, string> thread(sayHello);
		   thread.start();
		   string* result = thread.join();
		   if(result == Thread::UNCAUGHT_EXCEPTION)
		       cerr << "Thread threw an exception" << endl;
		   else {
		       cout << "Thread said: " << *result << endl;
		       delete result;
		   }
		 * @endcode
		 *
		 * @since 0.1.0
		 */
		static void *const UNCAUGHT_EXCEPTION;

	  protected:
		Thread(Handle, State);

		/**
		 * TODO.
		 */
		void startImpl(void*, bool);
		/**
		 * TODO.
		 */
		void* joinImpl();

		/**
		 * TODO.
		 */
		virtual void* runImpl(void*) = 0;

	  private:
		static void setupBottomOfCallStack();
		static BodyReturn bootstrap(void*);

	  public:
		/**
		 * TODO.
		 */
		Thread();
		Thread(const Thread&);
		virtual ~Thread();

		/**
		 * TODO.
		 */
		inline Handle getHandle() const {
			return handle;
		}

		/**
		 * TODO.
		 */
		inline State getState() const {
			return state;
		}

		void kill();
		void interrupt();

		/**
		 * TODO.
		 */
		static std::string getErrorMessage(ErrorCode);

		static void sleep(uint64_t);
		static Handle selfHandle();

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_THREAD_HPP */
