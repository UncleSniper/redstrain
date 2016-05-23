#ifndef REDSTRAIN_MOD_QU6NTUM_PROVIDERSOURCE_HPP
#define REDSTRAIN_MOD_QU6NTUM_PROVIDERSOURCE_HPP

#include <redstrain/text/types.hpp>

#include "api.hpp"

namespace redengine {
namespace qu6ntum {

	class Q6Engine;

	class REDSTRAIN_QU6NTUM_API ProviderSource {

	  public:
		ProviderSource();
		ProviderSource(const ProviderSource&);
		virtual ~ProviderSource();

		virtual void startProviderSource(Q6Engine&) = 0;
		virtual void stopProviderSource(Q6Engine&) = 0;
		virtual text::String16 getProviderSourceName() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_PROVIDERSOURCE_HPP */
