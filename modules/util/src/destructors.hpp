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

}}

#endif /* REDSTRAIN_MOD_UTIL_DESTRUCTORS_HPP */
