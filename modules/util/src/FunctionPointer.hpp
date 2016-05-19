#ifndef REDSTRAIN_MOD_UTIL_FUNCTIONPOINTER_HPP
#define REDSTRAIN_MOD_UTIL_FUNCTIONPOINTER_HPP

#include "Function.hpp"

namespace redengine {
namespace util {

	template<
		typename ReturnT,
		typename Arg0T = void,
		typename Arg1T = void,
		typename Arg2T = void,
		typename Arg3T = void,
		typename Arg4T = void,
		typename Arg5T = void,
		typename Arg6T = void,
		typename Arg7T = void
	>
	class FunctionPointer : public Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		Arg5T,
		Arg6T,
		Arg7T
	> {

	  public:
		typedef ReturnT (*Pointer)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T,
			Arg7T
		);

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT,
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T,
			Arg7T
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call(
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5,
			Arg6T arg6,
			Arg7T arg7
		) {
			return pointer(
				arg0,
				arg1,
				arg2,
				arg3,
				arg4,
				arg5,
				arg6,
				arg7
			);
		}

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T,
		typename Arg4T,
		typename Arg5T,
		typename Arg6T
	>
	class FunctionPointer<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		Arg5T,
		Arg6T,
		void
	> : public Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		Arg5T,
		Arg6T
	> {

	  public:
		typedef ReturnT (*Pointer)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T
		);

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT,
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call(
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5,
			Arg6T arg6
		) {
			return pointer(
				arg0,
				arg1,
				arg2,
				arg3,
				arg4,
				arg5,
				arg6
			);
		}

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T,
		typename Arg4T,
		typename Arg5T
	>
	class FunctionPointer<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		Arg5T,
		void,
		void
	> : public Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		Arg5T
	> {

	  public:
		typedef ReturnT (*Pointer)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T
		);

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT,
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call(
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5
		) {
			return pointer(
				arg0,
				arg1,
				arg2,
				arg3,
				arg4,
				arg5
			);
		}

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T,
		typename Arg4T
	>
	class FunctionPointer<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		void,
		void,
		void
	> : public Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T
	> {

	  public:
		typedef ReturnT (*Pointer)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T
		);

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT,
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call(
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4
		) {
			return pointer(
				arg0,
				arg1,
				arg2,
				arg3,
				arg4
			);
		}

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T
	>
	class FunctionPointer<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		void,
		void,
		void,
		void
	> : public Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T
	> {

	  public:
		typedef ReturnT (*Pointer)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T
		);

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT,
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call(
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3
		) {
			return pointer(
				arg0,
				arg1,
				arg2,
				arg3
			);
		}

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T
	>
	class FunctionPointer<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		void,
		void,
		void,
		void,
		void
	> : public Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T
	> {

	  public:
		typedef ReturnT (*Pointer)(
			Arg0T,
			Arg1T,
			Arg2T
		);

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT,
			Arg0T,
			Arg1T,
			Arg2T
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call(
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2
		) {
			return pointer(
				arg0,
				arg1,
				arg2
			);
		}

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T
	>
	class FunctionPointer<
		ReturnT,
		Arg0T,
		Arg1T,
		void,
		void,
		void,
		void,
		void,
		void
	> : public Function<
		ReturnT,
		Arg0T,
		Arg1T
	> {

	  public:
		typedef ReturnT (*Pointer)(
			Arg0T,
			Arg1T
		);

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT,
			Arg0T,
			Arg1T
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call(
			Arg0T arg0,
			Arg1T arg1
		) {
			return pointer(
				arg0,
				arg1
			);
		}

	};

	template<
		typename ReturnT,
		typename Arg0T
	>
	class FunctionPointer<
		ReturnT,
		Arg0T,
		void,
		void,
		void,
		void,
		void,
		void,
		void
	> : public Function<
		ReturnT,
		Arg0T
	> {

	  public:
		typedef ReturnT (*Pointer)(
			Arg0T
		);

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT,
			Arg0T
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call(
			Arg0T arg0
		) {
			return pointer(
				arg0
			);
		}

	};

	template<
		typename ReturnT
	>
	class FunctionPointer<
		ReturnT,
		void,
		void,
		void,
		void,
		void,
		void,
		void,
		void
	> : public Function<
		ReturnT
	> {

	  public:
		typedef ReturnT (*Pointer)();

	  private:
		Pointer pointer;

	  public:
		FunctionPointer(Pointer pointer) : pointer(pointer) {}

		FunctionPointer(const FunctionPointer& function) : Function<
			ReturnT
		>(function), pointer(function.pointer) {}

		inline Pointer getPointer() const {
			return pointer;
		}

		inline void setPointer(Pointer pointer) {
			this->pointer = pointer;
		}

		virtual ReturnT call() {
			return pointer();
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_FUNCTIONPOINTER_HPP */
