#ifndef REDSTRAIN_MOD_CRYPTO_CRCDIGESTALGORITHMFACTORY_HPP
#define REDSTRAIN_MOD_CRYPTO_CRCDIGESTALGORITHMFACTORY_HPP

#include "CRCDigest.hpp"
#include "AlgorithmFactory.hpp"

namespace redengine {
namespace crypto {

	template<typename InterfaceT = DigestAlgorithm, typename InstanceT = CRCDigest>
	class CRCDigestAlgorithmFactory : public AlgorithmFactory<InterfaceT> {

	  private:
		const CRCDigest::Polynomial polynomial;

	  public:
		CRCDigestAlgorithmFactory(CRCDigest::Polynomial polynomial) : polynomial(polynomial) {}
		CRCDigestAlgorithmFactory(const CRCDigestAlgorithmFactory& factory)
				: AlgorithmFactory<InterfaceT>(factory), polynomial(factory.polynomial) {}

		inline CRCDigest::Polynomial getPolynomial() const {
			return polynomial;
		}

		virtual InterfaceT* newAlgorithm() {
			return new InstanceT(polynomial);
		}

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CRCDIGESTALGORITHMFACTORY_HPP */
