#include <string>
#include <redstrain/util/Unref.hpp>

#include "TableCodec16.hpp"
#include "CodecManager.hpp"
#include "BlobTableCodecFactory.hpp"
#include "BlobCodeTable16Registrar.hpp"

using std::map;
using std::string;
using redengine::util::Unref;

namespace redengine {
namespace text {

	typedef BlobTableCodecFactory<Char16, Encoder16, TableCodec16> EncoderFactory;
	typedef BlobTableCodecFactory<Char16, Decoder16, TableCodec16> DecoderFactory;
	typedef map<string, EncoderFactory*> EncoderFactories;
	typedef map<string, DecoderFactory*> DecoderFactories;

	static EncoderFactories* encoderFactories = NULL;
	static DecoderFactories* decoderFactories = NULL;

	BlobCodeTable16Registrar::BlobCodeTable16Registrar(const char* data, size_t size, const char *const* names) {
		if(!encoderFactories)
			encoderFactories = new EncoderFactories;
		Unref<EncoderFactory> encoder(new EncoderFactory(data, size));
		for(; *names; ++names) {
			EncoderFactories::iterator it = encoderFactories->find(*names);
			if(it == encoderFactories->end())
				(*encoderFactories)[*names] = *encoder;
			else {
				it->second->unref();
				it->second = *encoder;
			}
			encoder->ref();
		}
		if(!decoderFactories)
			decoderFactories = new DecoderFactories;
		Unref<DecoderFactory> decoder(new DecoderFactory(data, size));
		for(; *names; ++names) {
			DecoderFactories::iterator it = decoderFactories->find(*names);
			if(it == decoderFactories->end())
				(*decoderFactories)[*names] = *decoder;
			else {
				it->second->unref();
				it->second = *decoder;
			}
			decoder->ref();
		}
	}

	void BlobCodeTable16Registrar::registerCodecs(CodecManager& manager) {
		if(encoderFactories) {
			EncoderFactories::const_iterator begin(encoderFactories->begin()), end(encoderFactories->end());
			for(; begin != end; ++begin)
				manager.setEncoder16Factory(begin->first, begin->second);
		}
		if(decoderFactories) {
			DecoderFactories::const_iterator begin(decoderFactories->begin()), end(decoderFactories->end());
			for(; begin != end; ++begin)
				manager.setDecoder16Factory(begin->first, begin->second);
		}
	}

}}
