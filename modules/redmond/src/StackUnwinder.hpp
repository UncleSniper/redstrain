#ifndef REDSTRAIN_MOD_REDMOND_STACKUNWINDER_HPP
#define REDSTRAIN_MOD_REDMOND_STACKUNWINDER_HPP

#include "api.hpp"

namespace redengine {
namespace redmond {

	class REDSTRAIN_REDMOND_API StackUnwinder {

	  private:
		void *originalReturnAddress, *basePointer, *bottomOfStack;
		unsigned long frameDepth;

	  public:
		StackUnwinder(void*, void*, void*);
		StackUnwinder(const StackUnwinder&);

		inline unsigned long getFrameDepth() const {
			return frameDepth;
		}

		void* getNextReturnAddress();

	};

	typedef void* (*BottomOfCallStackSource)();

	REDSTRAIN_REDMOND_API extern BottomOfCallStackSource bottomOfCallStackSource;

	REDSTRAIN_REDMOND_API void* singleThreadedBottomOfCallStackSource();
	REDSTRAIN_REDMOND_API StackUnwinder* getStackUnwinder();

}}

#endif /* REDSTRAIN_MOD_REDMOND_STACKUNWINDER_HPP */
