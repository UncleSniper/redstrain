#ifndef REDSTRAIN_MOD_PARALLEL_TASKCOOKIE_HPP
#define REDSTRAIN_MOD_PARALLEL_TASKCOOKIE_HPP

#include "api.hpp"

namespace redengine {
namespace parallel {

	class REDSTRAIN_PARALLEL_API TaskCookie {

	  public:
		TaskCookie();
		TaskCookie(const TaskCookie&);
		virtual ~TaskCookie();

		virtual void removeFromQueue() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_TASKCOOKIE_HPP */
