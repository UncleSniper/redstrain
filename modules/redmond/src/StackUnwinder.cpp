#include <cstddef>

#include "StackUnwinder.hpp"
#include "environment.hpp"

namespace redengine {
namespace redmond {

	static void* bottomOfCallStack = NULL;

	BottomOfCallStackSource bottomOfCallStackSource = singleThreadedBottomOfCallStackSource;

	void* singleThreadedBottomOfCallStackSource() {
		return bottomOfCallStack;
	}

	struct BottomOfStackSetter {
		BottomOfStackSetter() {
			void* dummy;
			bottomOfCallStack = reinterpret_cast<void*>(&dummy);
		}
	};

	static BottomOfStackSetter bottomOfCallStackSetter;

	StackUnwinder::StackUnwinder(void* originalReturnAddress, void* basePointer, void* bottomOfStack)
			: originalReturnAddress(originalReturnAddress), basePointer(basePointer),
			bottomOfStack(bottomOfStack), frameDepth(1ul) {
		void* bp = basePointer;
		do {
			++frameDepth;
			bp = *reinterpret_cast<void**>(bp);
		} while(bp && bp <= bottomOfStack);
	}

	StackUnwinder::StackUnwinder(const StackUnwinder& unwinder)
			: originalReturnAddress(unwinder.originalReturnAddress), basePointer(unwinder.basePointer),
			bottomOfStack(unwinder.bottomOfStack), frameDepth(unwinder.frameDepth) {}

	void* StackUnwinder::getNextReturnAddress() {
		void* result;
		if(originalReturnAddress) {
			result = originalReturnAddress;
			originalReturnAddress = NULL;
		}
		else if(basePointer && basePointer <= bottomOfStack) {
			result = reinterpret_cast<void**>(basePointer)[1];
			basePointer = *reinterpret_cast<void**>(basePointer);
		}
		else
			return NULL;
		--frameDepth;
		return result;
	}

#if defined(REDSTRAIN_ENV_ARCH_I686)

	void* _unwind_getCallerBasePointer(int dummy) {
		return reinterpret_cast<void**>(&dummy)[-2];
	}

	REDSTRAIN_REDMOND_API StackUnwinder* getStackUnwinder() {
		if(!bottomOfCallStackSource)
			return NULL;
		void* bottomOfStack = bottomOfCallStackSource();
		if(!bottomOfStack)
			return NULL;
		void* bp = _unwind_getCallerBasePointer(0);
		return new StackUnwinder(reinterpret_cast<void**>(bp)[1], *reinterpret_cast<void**>(bp), bottomOfStack);
	}

#elif defined(REDSTRAIN_ENV_ARCH_AMD64)

	void* _unwind_getLocationOfSavedBPOfEnclosingCall() {
		void* dummy;
		return reinterpret_cast<void**>(&dummy)[2];
	}

	REDSTRAIN_REDMOND_API StackUnwinder* getStackUnwinder() {
		if(!bottomOfCallStackSource)
			return NULL;
		void* bottomOfStack = bottomOfCallStackSource();
		if(!bottomOfStack)
			return NULL;
		void* mySavedBPAddr = _unwind_getLocationOfSavedBPOfEnclosingCall();
		return new StackUnwinder(reinterpret_cast<void**>(mySavedBPAddr)[1],
				*reinterpret_cast<void**>(mySavedBPAddr), bottomOfStack);
	}

#else /* unknown architecture */

	REDSTRAIN_REDMOND_API StackUnwinder* getStackUnwinder() {
		return NULL;
	}

#endif /* architectures */

}}
