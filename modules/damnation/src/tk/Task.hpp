#ifndef REDSTRAIN_MOD_DAMNATION_TK_TASK_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_TASK_HPP

#include <stdint.h>
#include <redstrain/util/ReferenceCounted.hpp>

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API Task : public util::ReferenceCounted {

	  public:
		Task();
		Task(const Task&);

		virtual uint64_t getNextExecutionTime() = 0;
		virtual bool executeTask() = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_TASK_HPP */
