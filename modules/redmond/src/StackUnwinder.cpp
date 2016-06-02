#include <cstddef>

#include "StackUnwinder.hpp"

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
			bottomOfCallStack = static_cast<void*>(&dummy);
		}
	};

	static BottomOfStackSetter bottomOfCallStackSetter;

	StackUnwinder::StackUnwinder(void* originalReturnAddress, void* basePointer, void* bottomOfStack)
			: originalReturnAddress(originalReturnAddress), basePointer(basePointer),
			bottomOfStack(bottomOfStack), frameDepth(1ul) {
		void* bp = basePointer;
		do {
			++frameDepth;
			bp = *static_cast<void**>(bp);
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
			result = static_cast<void**>(basePointer)[1];
			basePointer = *static_cast<void**>(basePointer);
		}
		else
			return NULL;
		--frameDepth;
		return result;
	}

	REDSTRAIN_REDMOND_API StackUnwinder* getStackUnwinder() {
		//TODO
		return NULL;
	}

}}
