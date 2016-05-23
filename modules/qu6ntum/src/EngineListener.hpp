#ifndef REDSTRAIN_MOD_QU6NTUM_ENGINELISTENER_HPP
#define REDSTRAIN_MOD_QU6NTUM_ENGINELISTENER_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace qu6ntum {

	class Q6Engine;
	class ProviderSource;

	class REDSTRAIN_QU6NTUM_API EngineListener {

	  public:
		EngineListener();
		EngineListener(const EngineListener&);
		virtual ~EngineListener();

		virtual void startingUp(Q6Engine&) = 0;
		virtual void startupComplete(Q6Engine&) = 0;
		virtual void shuttingDown(Q6Engine&) = 0;
		virtual void shutdownComplete(Q6Engine&) = 0;
		virtual void startingProviderSource(Q6Engine&, ProviderSource&) = 0;
		virtual void startingProviderSourceFailed(Q6Engine&, ProviderSource&, const error::Error&) = 0;
		virtual void stoppingProviderSource(Q6Engine&, ProviderSource&) = 0;
		virtual void stoppingProviderSourceFailed(Q6Engine&, ProviderSource&, const error::Error&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_ENGINELISTENER_HPP */
