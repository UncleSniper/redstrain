#ifndef REDSTRAIN_MOD_CMDLINE_STOPEXECUTION_HPP
#define REDSTRAIN_MOD_CMDLINE_STOPEXECUTION_HPP

#include "api.hpp"

namespace redengine {
namespace cmdline {

	class REDSTRAIN_CMDLINE_API StopExecution {

	  private:
		const int status;

	  public:
		StopExecution(int);
		StopExecution(const StopExecution&);

		inline int getStatus() const {
			return status;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_STOPEXECUTION_HPP */
