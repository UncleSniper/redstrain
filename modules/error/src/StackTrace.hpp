#ifndef REDSTRAIN_MOD_ERROR_STACKTRACE_HPP
#define REDSTRAIN_MOD_ERROR_STACKTRACE_HPP

#include <string>
#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/redmond/StackUnwinder.hpp>
#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StackTrace : public util::ReferenceCounted {

	  private:
		const util::MemorySize frameCount;
		void* *const returnAddresses;

	  public:
		StackTrace(redmond::StackUnwinder&);
		StackTrace(const StackTrace&);
		virtual ~StackTrace();

		inline util::MemorySize getFrameCount() const {
			return frameCount;
		}

		void* getReturnAddressOfFrame(util::MemorySize) const;

		static bool getSymbolInfoForReturnAddress(void*, std::string&, void*&, std::string&, void*&);
		static std::string formatMemoryAddress(void*);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STACKTRACE_HPP */
