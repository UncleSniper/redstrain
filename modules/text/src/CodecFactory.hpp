#ifndef REDSTRAIN_MOD_TEXT_CODECFACTORY_HPP
#define REDSTRAIN_MOD_TEXT_CODECFACTORY_HPP

#include <redstrain/util/ReferenceCounted.hpp>

namespace redengine {
namespace text {

	template<typename CodecT>
	class CodecFactory : public util::ReferenceCounted {

	  public:
		typedef CodecT Codec;

	  public:
		CodecFactory() {}
		CodecFactory(const CodecFactory& factory) : ReferenceCounted(factory) {}

		virtual CodecT* newCodec() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CODECFACTORY_HPP */
