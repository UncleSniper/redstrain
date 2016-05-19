#ifndef REDSTRAIN_MOD_UTIL_FUNCTION_HPP
#define REDSTRAIN_MOD_UTIL_FUNCTION_HPP

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
	class Function {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T,
			Arg7T
		) = 0;

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
	class Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		Arg5T,
		Arg6T,
		void
	> {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T
		) = 0;

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
	class Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		Arg5T,
		void,
		void
	> {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T
		) = 0;

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T,
		typename Arg4T
	>
	class Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		Arg4T,
		void,
		void,
		void
	> {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T
		) = 0;

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T
	>
	class Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		Arg3T,
		void,
		void,
		void,
		void
	> {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T
		) = 0;

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T
	>
	class Function<
		ReturnT,
		Arg0T,
		Arg1T,
		Arg2T,
		void,
		void,
		void,
		void,
		void
	> {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call(
			Arg0T,
			Arg1T,
			Arg2T
		) = 0;

	};

	template<
		typename ReturnT,
		typename Arg0T,
		typename Arg1T
	>
	class Function<
		ReturnT,
		Arg0T,
		Arg1T,
		void,
		void,
		void,
		void,
		void,
		void
	> {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call(
			Arg0T,
			Arg1T
		) = 0;

	};

	template<
		typename ReturnT,
		typename Arg0T
	>
	class Function<
		ReturnT,
		Arg0T,
		void,
		void,
		void,
		void,
		void,
		void,
		void
	> {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call(
			Arg0T
		) = 0;

	};

	template<
		typename ReturnT
	>
	class Function<
		ReturnT,
		void,
		void,
		void,
		void,
		void,
		void,
		void,
		void
	> {

	  public:
		Function() {}
		Function(const Function&) {}
		virtual ~Function() {}

		virtual ReturnT call() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_FUNCTION_HPP */
