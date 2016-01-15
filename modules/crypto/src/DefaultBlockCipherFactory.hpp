#ifndef REDSTRAIN_MOD_CRYPTO_DEFAULTBLOCKCIPHERFACTORY_HPP
#define REDSTRAIN_MOD_CRYPTO_DEFAULTBLOCKCIPHERFACTORY_HPP

#include <redstrain/util/Delete.hpp>

#include "Padding.hpp"
#include "CipherMode.hpp"
#include "CipherFactory.hpp"

namespace redengine {
namespace crypto {

	template<
		typename AlgorithmT,
		typename KeySizeT = void,
		typename BlockSizeT = void
	>
	class DefaultBlockCipherFactory : public CipherFactory {

	  private:
		const KeySizeT keySize;
		const BlockSizeT blockSize;

	  public:
		DefaultBlockCipherFactory(KeySizeT keySize, BlockSizeT blockSize)
				: keySize(keySize), blockSize(blockSize) {}

		DefaultBlockCipherFactory(const DefaultBlockCipherFactory& factory)
				: CipherFactory(factory), keySize(factory.keySize), blockSize(factory.blockSize) {}

		inline KeySizeT getKeySize() const {
			return keySize;
		}

		inline BlockSizeT getBlockSize() const {
			return blockSize;
		}

		virtual CipherAlgorithm* newCipher(AlgorithmFactory<CipherMode>& modeFactory,
				AlgorithmFactory<Padding>& paddingFactory) {
			util::Delete<CipherMode> mode(modeFactory.newAlgorithm());
			util::Delete<Padding> padding(paddingFactory.newAlgorithm());
			CipherAlgorithm* cipher = new AlgorithmT(**mode, **padding, keySize, blockSize);
			mode.set();
			padding.set();
			return cipher;
		}

	};

	template<
		typename AlgorithmT,
		typename KeySizeT,
		typename DefaultModeT,
		typename DefaultPaddingT
	>
	class DefaultBlockCipherFactory<
		AlgorithmT,
		KeySizeT,
		void,
		DefaultModeT,
		DefaultPaddingT
	> : public CipherFactory {

	  private:
		const KeySizeT keySize;

	  public:
		DefaultBlockCipherFactory(KeySizeT keySize) : keySize(keySize) {}

		DefaultBlockCipherFactory(const DefaultBlockCipherFactory& factory)
				: CipherFactory(factory), keySize(factory.keySize) {}

		inline KeySizeT getKeySize() const {
			return keySize;
		}

		virtual CipherAlgorithm* newCipher(AlgorithmFactory<CipherMode>& modeFactory,
				AlgorithmFactory<Padding>& paddingFactory) {
			usdo::util::Delete<CipherMode> mode(modeFactory.newAlgorithm());
			usdo::util::Delete<Padding> padding(paddingFactory.newAlgorithm());
			CipherAlgorithm* cipher = new AlgorithmT(**mode, **padding, keySize);
			mode.set();
			padding.set();
			return cipher;
		}

	};

	template<
		typename AlgorithmT,
		typename BlockSizeT,
		typename DefaultModeT,
		typename DefaultPaddingT
	>
	class DefaultBlockCipherFactory<
		AlgorithmT,
		void,
		BlockSizeT,
		DefaultModeT,
		DefaultPaddingT
	> : public CipherFactory {

	  private:
		const BlockSizeT blockSize;

	  public:
		DefaultBlockCipherFactory(BlockSizeT blockSize) : blockSize(blockSize) {}

		DefaultBlockCipherFactory(const DefaultBlockCipherFactory& factory)
				: CipherFactory(factory), blockSize(factory.blockSize) {}

		inline BlockSizeT getBlockSize() const {
			return blockSize;
		}

		virtual CipherAlgorithm* newCipher(AlgorithmFactory<CipherMode>& modeFactory,
				AlgorithmFactory<Padding>& paddingFactory) {
			usdo::util::Delete<CipherMode> mode(modeFactory.newAlgorithm());
			usdo::util::Delete<Padding> padding(paddingFactory.newAlgorithm());
			CipherAlgorithm* cipher = new AlgorithmT(**mode, **padding, blockSize);
			mode.set();
			padding.set();
			return cipher;
		}

	};

	template<
		typename AlgorithmT,
		typename DefaultModeT,
		typename DefaultPaddingT
	>
	class DefaultBlockCipherFactory<
		AlgorithmT,
		void,
		void,
		DefaultModeT,
		DefaultPaddingT
	> : public CipherFactory {

	  public:
		DefaultBlockCipherFactory() {}

		DefaultBlockCipherFactory(const DefaultBlockCipherFactory& factory) : CipherFactory(factory) {}

		virtual CipherAlgorithm* newCipher(AlgorithmFactory<CipherMode>& modeFactory,
				AlgorithmFactory<Padding>& paddingFactory) {
			usdo::util::Delete<CipherMode> mode(modeFactory.newAlgorithm());
			usdo::util::Delete<Padding> padding(paddingFactory.newAlgorithm());
			CipherAlgorithm* cipher = new AlgorithmT(**mode, **padding);
			mode.set();
			padding.set();
			return cipher;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_DEFAULTBLOCKCIPHERFACTORY_HPP */
