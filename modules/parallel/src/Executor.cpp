#include "Executor.hpp"
#include "ThreadPool.hpp"

namespace redengine {
namespace parallel {

	Executor::Executor(ThreadPool& threadPool) : threadPool(threadPool) {}

	Executor::Executor(const Executor& executor) : threadPool(executor.threadPool) {}

	Executor::~Executor() {}

	void Executor::submitTask(UntypedTask& task) {
		threadPool.scheduleIntoThread(task);
	}

}}
