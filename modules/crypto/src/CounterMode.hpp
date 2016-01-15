#ifndef REDSTRAIN_MOD_CRYPTO_COUNTERMODE_HPP
#define REDSTRAIN_MOD_CRYPTO_COUNTERMODE_HPP

#include "BufferingCipherMode.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CounterMode : public BufferingCipherMode {

	  private:
		char* stage;

	  public:
		CounterMode();
		CounterMode(const CounterMode&);
		virtual ~CounterMode();

		virtual const char* preprocessBlock(CipherAlgorithm::Direction&,
				const char*, char*, util::MemorySize);
		virtual const char* postprocessBlock(CipherAlgorithm::Direction,
				const char*, char*, char*, util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_COUNTERMODE_HPP */
