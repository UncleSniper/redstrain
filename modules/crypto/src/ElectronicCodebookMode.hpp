#ifndef REDSTRAIN_MOD_CRYPTO_ELECTRONICCODEBOOKMODE_HPP
#define REDSTRAIN_MOD_CRYPTO_ELECTRONICCODEBOOKMODE_HPP

#include "CipherMode.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API ElectronicCodebookMode : public CipherMode {

	  public:
		ElectronicCodebookMode();
		ElectronicCodebookMode(const ElectronicCodebookMode&);

		virtual const char* preprocessBlock(CipherAlgorithm::Direction&,
				const char*, char*, util::MemorySize);
		virtual const char* postprocessBlock(CipherAlgorithm::Direction,
				const char*, char*, char*, util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_ELECTRONICCODEBOOKMODE_HPP */
