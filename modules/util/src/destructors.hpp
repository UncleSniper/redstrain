#ifndef REDSTRAIN_MOD_UTIL_DESTRUCTORS_HPP
#define REDSTRAIN_MOD_UTIL_DESTRUCTORS_HPP

namespace redengine {
namespace util {

	template<typename SubjectT>
	void noDestructor(SubjectT&) {}

	template<typename SubjectT>
	void explicitDestructor(SubjectT& subject) {
		subject.~SubjectT();
	}

	template<typename SubjectT>
	void deleteDestructor(SubjectT*& subject) {
		if(subject)
			delete subject;
	}

	template<typename SubjectT>
	void unrefDestructor(SubjectT*& subject) {
		if(subject)
			subject->unref();
	}

	template<typename SubjectT>
	struct VoidableDestructorSignature {
		typedef void (*Destructor)(SubjectT&);
	};

	template<>
	struct VoidableDestructorSignature<void> {
		typedef void (*Destructor)();
	};

	template<typename SubjectT>
	struct NoDestructor {
		static void destructor(SubjectT&) {}
	};

	template<>
	struct NoDestructor<void> {
		static void destructor() {}
	};

	template<typename SubjectT>
	struct ExplicitDestructor {
		static void destructor(SubjectT& subject) {
			subject.~SubjectT();
		}
	};

	template<>
	struct ExplicitDestructor<void> {
		static void destructor() {}
	};

	template<typename SubjectT>
	struct DeleteDestructor {
		static void destructor(SubjectT*& subject) {
			if(subject)
				delete subject;
		}
	};

	template<>
	struct DeleteDestructor<void> {
		static void destructor() {}
	};

	template<typename SubjectT>
	struct UnrefDestructor {
		static void destructor(SubjectT*& subject) {
			if(subject)
				subject->unref();
		}
	};

	template<>
	struct UnrefDestructor<void> {
		static void destructor() {}
	};

}}

#endif /* REDSTRAIN_MOD_UTIL_DESTRUCTORS_HPP */
