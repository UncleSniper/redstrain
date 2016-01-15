#ifndef REDSTRAIN_MOD_CRYPTO_RENDITIONTRANSCODER_HPP
#define REDSTRAIN_MOD_CRYPTO_RENDITIONTRANSCODER_HPP

#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API RenditionTranscoder : public io::OutputStream<char> {

	  protected:
		io::OutputStream<char>& sink;

	  public:
		RenditionTranscoder(io::OutputStream<char>&);
		RenditionTranscoder(const RenditionTranscoder&);

		inline io::OutputStream<char>& getSink() {
			return sink;
		}

		inline const io::OutputStream<char>& getSink() const {
			return sink;
		}

		virtual void flush();
		virtual void close();

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_RENDITIONTRANSCODER_HPP */
