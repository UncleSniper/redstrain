#ifndef REDSTRAIN_MOD_TEXT_CODECRESOLVER_HPP
#define REDSTRAIN_MOD_TEXT_CODECRESOLVER_HPP

#include <set>
#include <string>

#include "CodecFactory.hpp"

namespace redengine {
namespace text {

	template<typename CodecT>
	class CodecResolver : public util::ReferenceCounted {

	  public:
		typedef CodecT Codec;

	  public:
		CodecResolver() {}
		CodecResolver(const CodecResolver& resolver) : ReferenceCounted(resolver) {}

		virtual CodecFactory<CodecT>* resolveCodec(const std::string&) = 0;
		virtual void listKnownCodecs(std::set<std::string>&) {}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CODECRESOLVER_HPP */
