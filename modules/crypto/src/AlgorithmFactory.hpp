#ifndef REDSTRAIN_MOD_CRYPTO_ALGORITHMFACTORY_HPP
#define REDSTRAIN_MOD_CRYPTO_ALGORITHMFACTORY_HPP

#include <redstrain/util/ReferenceCounted.hpp>

namespace redengine {
namespace crypto {

	template<typename AlgorithmT>
	class AlgorithmFactory : public util::ReferenceCounted {

	  public:
		typedef AlgorithmT Algorithm;

	  public:
		AlgorithmFactory() {}
		AlgorithmFactory(const AlgorithmFactory& factory) : ReferenceCounted(factory) {}

		virtual AlgorithmT* newAlgorithm() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_ALGORITHMFACTORY_HPP */
