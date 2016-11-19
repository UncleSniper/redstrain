#ifndef REDSTRAIN_MOD_UTIL_MEMBERFUNCTIONPOINTER_HPP
#define REDSTRAIN_MOD_UTIL_MEMBERFUNCTIONPOINTER_HPP

namespace redengine {
namespace util {

	template<
		typename BaseT,
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
	class MemberFunctionPointer {

	  public:
		typedef ReturnT (BaseT::*Type)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T,
			Arg7T
		);

		typedef ReturnT (BaseT::*ConstType)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T,
			Arg7T
		) const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5,
			Arg6T arg6,
			Arg7T arg7
		) {
			return (base.*function)(
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

		static inline ReturnT call(
			ConstType function,
			const BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5,
			Arg6T arg6,
			Arg7T arg7
		) {
			return (base.*function)(
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
		typename BaseT,
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T,
		typename Arg4T,
		typename Arg5T,
		typename Arg6T
	>
	class MemberFunctionPointer<
		BaseT,
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
		typedef ReturnT (BaseT::*Type)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T
		);

		typedef ReturnT (BaseT::*ConstType)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T,
			Arg6T
		) const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5,
			Arg6T arg6
		) {
			return (base.*function)(
				arg0,
				arg1,
				arg2,
				arg3,
				arg4,
				arg5,
				arg6
			);
		}

		static inline ReturnT call(
			ConstType function,
			const BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5,
			Arg6T arg6
		) {
			return (base.*function)(
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
		typename BaseT,
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T,
		typename Arg4T,
		typename Arg5T
	>
	class MemberFunctionPointer<
		BaseT,
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
		typedef ReturnT (BaseT::*Type)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T
		);

		typedef ReturnT (BaseT::*ConstType)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T,
			Arg5T
		) const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5
		) {
			return (base.*function)(
				arg0,
				arg1,
				arg2,
				arg3,
				arg4,
				arg5
			);
		}

		static inline ReturnT call(
			ConstType function,
			const BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4,
			Arg5T arg5
		) {
			return (base.*function)(
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
		typename BaseT,
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T,
		typename Arg4T
	>
	class MemberFunctionPointer<
		BaseT,
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
		typedef ReturnT (BaseT::*Type)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T
		);

		typedef ReturnT (BaseT::*ConstType)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T,
			Arg4T
		) const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4
		) {
			return (base.*function)(
				arg0,
				arg1,
				arg2,
				arg3,
				arg4
			);
		}

		static inline ReturnT call(
			ConstType function,
			const BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3,
			Arg4T arg4
		) {
			return (base.*function)(
				arg0,
				arg1,
				arg2,
				arg3,
				arg4
			);
		}

	};

	template<
		typename BaseT,
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T,
		typename Arg3T
	>
	class MemberFunctionPointer<
		BaseT,
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
		typedef ReturnT (BaseT::*Type)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T
		);

		typedef ReturnT (BaseT::*ConstType)(
			Arg0T,
			Arg1T,
			Arg2T,
			Arg3T
		) const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3
		) {
			return (base.*function)(
				arg0,
				arg1,
				arg2,
				arg3
			);
		}

		static inline ReturnT call(
			ConstType function,
			const BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2,
			Arg3T arg3
		) {
			return (base.*function)(
				arg0,
				arg1,
				arg2,
				arg3
			);
		}

	};

	template<
		typename BaseT,
		typename ReturnT,
		typename Arg0T,
		typename Arg1T,
		typename Arg2T
	>
	class MemberFunctionPointer<
		BaseT,
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
		typedef ReturnT (BaseT::*Type)(
			Arg0T,
			Arg1T,
			Arg2T
		);

		typedef ReturnT (BaseT::*ConstType)(
			Arg0T,
			Arg1T,
			Arg2T
		) const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2
		) {
			return (base.*function)(
				arg0,
				arg1,
				arg2
			);
		}

		static inline ReturnT call(
			ConstType function,
			const BaseT& base,
			Arg0T arg0,
			Arg1T arg1,
			Arg2T arg2
		) {
			return (base.*function)(
				arg0,
				arg1,
				arg2
			);
		}

	};

	template<
		typename BaseT,
		typename ReturnT,
		typename Arg0T,
		typename Arg1T
	>
	class MemberFunctionPointer<
		BaseT,
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
		typedef ReturnT (BaseT::*Type)(
			Arg0T,
			Arg1T
		);

		typedef ReturnT (BaseT::*ConstType)(
			Arg0T,
			Arg1T
		) const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base,
			Arg0T arg0,
			Arg1T arg1
		) {
			return (base.*function)(
				arg0,
				arg1
			);
		}

		static inline ReturnT call(
			ConstType function,
			const BaseT& base,
			Arg0T arg0,
			Arg1T arg1
		) {
			return (base.*function)(
				arg0,
				arg1
			);
		}

	};

	template<
		typename BaseT,
		typename ReturnT,
		typename Arg0T
	>
	class MemberFunctionPointer<
		BaseT,
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
		typedef ReturnT (BaseT::*Type)(
			Arg0T
		);

		typedef ReturnT (BaseT::*ConstType)(
			Arg0T
		) const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base,
			Arg0T arg0
		) {
			return (base.*function)(
				arg0
			);
		}

		static inline ReturnT call(
			ConstType function,
			const BaseT& base,
			Arg0T arg0
		) {
			return (base.*function)(
				arg0
			);
		}

	};

	template<
		typename BaseT,
		typename ReturnT
	>
	class MemberFunctionPointer<
		BaseT,
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
		typedef ReturnT (BaseT::*Type)();

		typedef ReturnT (BaseT::*ConstType)() const;

	  public:
		static inline ReturnT call(
			Type function,
			BaseT& base
		) {
			return (base.*function)();
		}

		static inline ReturnT call(
			ConstType function,
			const BaseT& base
		) {
			return (base.*function)();
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_MEMBERFUNCTIONPOINTER_HPP */
