#ifndef REDSTRAIN_MOD_ALGORITHM_DESTRUCTORS_HPP
#define REDSTRAIN_MOD_ALGORITHM_DESTRUCTORS_HPP

namespace redengine {
namespace algorithm {

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

#endif /* REDSTRAIN_MOD_ALGORITHM_DESTRUCTORS_HPP */
