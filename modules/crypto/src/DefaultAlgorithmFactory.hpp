#ifndef REDSTRAIN_MOD_CRYPTO_DEFAULTALGORITHMFACTORY_HPP
#define REDSTRAIN_MOD_CRYPTO_DEFAULTALGORITHMFACTORY_HPP

#include "AlgorithmFactory.hpp"

namespace redengine {
namespace crypto {

	template<typename InterfaceT, typename InstanceT>
	class DefaultAlgorithmFactory : public AlgorithmFactory<InterfaceT> {

	  public:
		DefaultAlgorithmFactory() {}
		DefaultAlgorithmFactory(const DefaultAlgorithmFactory& factory) : AlgorithmFactory<InterfaceT>(factory) {}

		virtual InterfaceT* newAlgorithm() {
			return new InstanceT;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_DEFAULTALGORITHMFACTORY_HPP */
