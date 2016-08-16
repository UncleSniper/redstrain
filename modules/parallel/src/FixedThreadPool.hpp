#ifndef REDSTRAIN_MOD_PARALLEL_FIXEDTHREADPOOL_HPP
#define REDSTRAIN_MOD_PARALLEL_FIXEDTHREADPOOL_HPP

#include <set>

#include "AbstractThreadPool.hpp"

namespace redengine {
namespace parallel {

	class REDSTRAIN_PARALLEL_API FixedThreadPool : public AbstractThreadPool {

	  private:
		volatile unsigned desiredThreadCount, actualThreadCount;

	  private:
		void balanceThreadCount();

	  protected:
		FixedThreadPool(const FixedThreadPool&);

		virtual void goingIdle(AbstractPooledThread&);
		virtual void goingBusy(AbstractPooledThread&);
		virtual void threadFull(AbstractPooledThread&);
		virtual void leavingForce(AbstractPooledThread&);
		virtual AbstractPooledThread* expedite(UntypedTask&);

	  public:
		FixedThreadPool(unsigned = 0u);

		inline unsigned getThreadCount() const {
			return desiredThreadCount;
		}

		void setThreadCount(unsigned);

		inline unsigned getActualThreadCount() const {
			return actualThreadCount;
		}

		virtual void scheduleIntoThread(UntypedTask&);

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_FIXEDTHREADPOOL_HPP */
