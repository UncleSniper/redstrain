#ifndef REDSTRAIN_MOD_PARALLEL_TASK_HPP
#define REDSTRAIN_MOD_PARALLEL_TASK_HPP

namespace redengine {
namespace parallel {

	template<typename ResultT>
	class Task {

	  public:
		Task();
		Task(const Task&);
		virtual ~Task();

		virtual ResultT run() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_TASK_HPP */
