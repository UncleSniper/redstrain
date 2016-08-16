#ifndef REDSTRAIN_MOD_PARALLEL_THREADPOOL_HPP
#define REDSTRAIN_MOD_PARALLEL_THREADPOOL_HPP

#include "api.hpp"

namespace redengine {
namespace parallel {

	class UntypedTask;
	class PooledThread;

	class REDSTRAIN_PARALLEL_API ThreadPool {

	  public:
		static const unsigned DEFAULT_MAX_NESTING_LEVEL = 100u;

	  private:
		unsigned maxNestingLevel;

	  public:
		ThreadPool();
		ThreadPool(const ThreadPool&);
		virtual ~ThreadPool();

		inline unsigned getMaxNestingLevel() const {
			return maxNestingLevel;
		}

		void setMaxNestingLevel(unsigned);

		virtual void scheduleIntoThread(UntypedTask&) = 0;
		virtual void force(UntypedTask&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_THREADPOOL_HPP */
