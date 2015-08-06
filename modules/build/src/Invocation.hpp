#ifndef REDSTRAIN_MOD_BUILD_INVOCATION_HPP
#define REDSTRAIN_MOD_BUILD_INVOCATION_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Invocation {

	  public:
		Invocation();
		Invocation(const Invocation&);
		virtual ~Invocation();

		virtual void invoke() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_INVOCATION_HPP */
