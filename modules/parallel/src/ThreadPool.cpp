#include "ThreadPool.hpp"

namespace redengine {
namespace parallel {

	ThreadPool::ThreadPool() : maxNestingLevel(ThreadPool::DEFAULT_MAX_NESTING_LEVEL) {}

	ThreadPool::ThreadPool(const ThreadPool& pool) : maxNestingLevel(pool.maxNestingLevel) {}

	ThreadPool::~ThreadPool() {}

	void ThreadPool::setMaxNestingLevel(unsigned level) {
		maxNestingLevel = level ? level : ThreadPool::DEFAULT_MAX_NESTING_LEVEL;
	}

}}
