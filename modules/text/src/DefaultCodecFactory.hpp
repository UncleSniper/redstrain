#ifndef REDSTRAIN_MOD_TEXT_DEFAULTCODECFACTORY_HPP
#define REDSTRAIN_MOD_TEXT_DEFAULTCODECFACTORY_HPP

#include "CodecFactory.hpp"

namespace redengine {
namespace text {

	template<typename InterfaceT, typename InstanceT>
	class DefaultCodecFactory : public CodecFactory<InterfaceT> {

	  public:
		DefaultCodecFactory() {}
		DefaultCodecFactory(const DefaultCodecFactory& factory) : CodecFactory<InterfaceT>(factory) {}

		virtual InterfaceT* newCodec() {
			return new InstanceT;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DEFAULTCODECFACTORY_HPP */
