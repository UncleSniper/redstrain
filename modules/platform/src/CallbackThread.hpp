#ifndef REDSTRAIN_MOD_PLATFORM_CALLBACKTHREAD_HPP
#define REDSTRAIN_MOD_PLATFORM_CALLBACKTHREAD_HPP

#include "TypedThread.hpp"

namespace redengine {
namespace platform {

	template<typename ArgumentT = void, typename ResultT = void>
	class CallbackThread : public TypedThread<ArgumentT, ResultT> {

	  public:
		typedef ResultT* (*Callback)(ArgumentT*);

	  private:
		Callback callback;

	  protected:
		virtual ResultT* run(ArgumentT* user) {
			return callback ? callback(user) : NULL;
		}

	  public:
		CallbackThread(Callback callback = NULL) : callback(callback) {}
		CallbackThread(const CallbackThread& thread)
				: TypedThread<ArgumentT, ResultT>(thread), callback(callback) {}

		inline Callback getCallback() const {
			return callback;
		}

		inline void setCallback(Callback newCallback) {
			callback = newCallback;
		};

	};

	template<typename ArgumentT>
	class CallbackThread<ArgumentT, void> : public TypedThread<ArgumentT, void> {

	  public:
		typedef void (*Callback)(ArgumentT*);

	  private:
		Callback callback;

	  protected:
		virtual void run(ArgumentT* user) {
			if(callback)
				callback(user);
		}

	  public:
		CallbackThread(Callback callback = NULL) : callback(callback) {}
		CallbackThread(const CallbackThread& thread)
				: TypedThread<ArgumentT, void>(thread), callback(callback) {}

		inline Callback getCallback() const {
			return callback;
		}

		inline void setCallback(Callback newCallback) {
			callback = newCallback;
		};

	};

	template<typename ResultT>
	class CallbackThread<void, ResultT> : public TypedThread<void, ResultT> {

	  public:
		typedef ResultT* (*Callback)();

	  private:
		Callback callback;

	  protected:
		virtual ResultT* run() {
			return callback ? callback() : NULL;
		}

	  public:
		CallbackThread(Callback callback = NULL) : callback(callback) {}
		CallbackThread(const CallbackThread& thread)
				: TypedThread<void, ResultT>(thread), callback(callback) {}

		inline Callback getCallback() const {
			return callback;
		}

		inline void setCallback(Callback newCallback) {
			callback = newCallback;
		};

	};

	template<>
	class CallbackThread<void, void> : public TypedThread<void, void> {

	  public:
		typedef void (*Callback)();

	  private:
		Callback callback;

	  protected:
		virtual void run() {
			if(callback)
				callback();
		}

	  public:
		CallbackThread(Callback callback = NULL) : callback(callback) {}
		CallbackThread(const CallbackThread& thread)
				: TypedThread<void, void>(thread), callback(callback) {}

		inline Callback getCallback() const {
			return callback;
		}

		inline void setCallback(Callback newCallback) {
			callback = newCallback;
		};

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_CALLBACKTHREAD_HPP */
