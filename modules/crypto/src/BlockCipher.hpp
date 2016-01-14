#ifndef REDSTRAIN_MOD_CRYPTO_BLOCKCIPHER_HPP
#define REDSTRAIN_MOD_CRYPTO_BLOCKCIPHER_HPP

#include "CipherAlgorithm.hpp"

namespace redengine {
namespace crypto {

	class Padding;
	class CipherMode;

	class REDSTRAIN_CRYPTO_API BlockCipher : public CipherAlgorithm {

	  private:
		Padding& padding;
		CipherMode& mode;
		char *buffer, *iv;
		util::MemorySize fill;

	  protected:
		const util::MemorySize blockSize;

	  private:
		void processBlock(char*);

	  protected:
		BlockCipher(const BlockCipher&);

	  public:
		BlockCipher(CipherMode&, Padding&, util::MemorySize);
		virtual ~BlockCipher();

		inline Padding& getPadding() {
			return padding;
		}

		inline const Padding& getPadding() const {
			return padding;
		}

		inline CipherMode& getMode() {
			return mode;
		}

		inline const CipherMode& getMode() const {
			return mode;
		}

		inline char* getInitVector() {
			return iv;
		}

		inline const char* getInitVector() const {
			return iv;
		}

		void setInitVector(const char*);

		virtual util::MemorySize getBlockSize() const;
		virtual util::MemorySize transformMessage(const char*, util::MemorySize, char*);
		virtual util::MemorySize endMessage(char*);
		virtual void reset();

		virtual void switchDirectionForMode(Direction) = 0;
		virtual void transformBlock(const char*, char*) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_BLOCKCIPHER_HPP */
