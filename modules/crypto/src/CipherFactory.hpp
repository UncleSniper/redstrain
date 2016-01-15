#ifndef REDSTRAIN_MOD_CRYPTO_CIPHERFACTORY_HPP
#define REDSTRAIN_MOD_CRYPTO_CIPHERFACTORY_HPP

#include "AlgorithmFactory.hpp"
#include "api.hpp"

namespace redengine {
namespace crypto {

	class Padding;
	class CipherMode;
	class CipherAlgorithm;

	class REDSTRAIN_CRYPTO_API CipherFactory : public util::ReferenceCounted {

	  public:
		CipherFactory();
		CipherFactory(const CipherFactory&);

		virtual CipherAlgorithm* newCipher(AlgorithmFactory<CipherMode>&, AlgorithmFactory<Padding>&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CIPHERFACTORY_HPP */
