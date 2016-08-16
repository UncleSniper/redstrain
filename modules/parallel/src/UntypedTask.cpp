#include "TaskCookie.hpp"
#include "UntypedTask.hpp"

namespace redengine {
namespace parallel {

	UntypedTask::UntypedTask() {}

	UntypedTask::UntypedTask(const UntypedTask&) {}

	UntypedTask::~UntypedTask() {
		if(queueCookie) {
			try {
				queueCookie->removeFromQueue();
			}
			catch(...) {}
			delete queueCookie;
		}
	}

}}
