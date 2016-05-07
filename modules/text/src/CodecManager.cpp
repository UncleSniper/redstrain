#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "UTF8Encoder.hpp"
#include "UTF8Decoder.hpp"
#include "CodecManager.hpp"
#include "NoSuchEncoderError.hpp"
#include "NoSuchDecoderError.hpp"
#include "DefaultCodecFactory.hpp"
#include "BlobCodeTable16Registrar.hpp"

using std::set;
using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace text {

	CodecManager::CodecManager() {}

	CodecManager::CodecManager(const CodecManager& manager) : enc16reg(manager.enc16reg),
			dec16reg(manager.dec16reg), enc16resolv(manager.enc16resolv), dec16resolv(manager.dec16resolv),
			enc16cnames(manager.enc16cnames), dec16cnames(manager.dec16cnames) {
		Encoder16Iterator enc16begin(enc16reg.begin()), enc16end(enc16reg.end());
		for(; enc16begin != enc16end; ++enc16begin)
			enc16begin->second->ref();
		Decoder16Iterator dec16begin(dec16reg.begin()), dec16end(dec16reg.end());
		for(; dec16begin != dec16end; ++dec16begin)
			dec16begin->second->ref();
		Encoder16ResolverIterator enc16resbegin(enc16resolv.begin()), enc16resend(enc16resolv.end());
		for(; enc16resbegin != enc16resend; ++enc16resbegin)
			(*enc16resbegin)->ref();
		Decoder16ResolverIterator dec16resbegin(dec16resolv.begin()), dec16resend(dec16resolv.end());
		for(; dec16resbegin != dec16resend; ++dec16resbegin)
			(*dec16resbegin)->ref();
	}

	CodecManager::~CodecManager() {
		purge();
		Encoder16ResolverIterator enc16resbegin(enc16resolv.begin()), enc16resend(enc16resolv.end());
		for(; enc16resbegin != enc16resend; ++enc16resbegin)
			(*enc16resbegin)->unref();
		Decoder16ResolverIterator dec16resbegin(dec16resolv.begin()), dec16resend(dec16resolv.end());
		for(; dec16resbegin != dec16resend; ++dec16resbegin)
			(*dec16resbegin)->unref();
	}

	CodecManager::Encoder16Factory* CodecManager::getEncoder16Factory(const string& name) const {
		Encoder16Iterator it = enc16reg.find(name);
		return it == enc16reg.end() ? NULL : it->second;
	}

	CodecManager::Decoder16Factory* CodecManager::getDecoder16Factory(const string& name) const {
		Decoder16Iterator it = dec16reg.find(name);
		return it == dec16reg.end() ? NULL : it->second;
	}

	Encoder16* CodecManager::getEncoder16(const string& name) const {
		Encoder16Factory* factory = getEncoder16Factory(name);
		if(!factory)
			throw NoSuchEncoderError(name);
		Encoder16* codec = factory->newCodec();
		if(!codec)
			throw NoSuchEncoderError(name);
		return codec;
	}

	Decoder16* CodecManager::getDecoder16(const string& name) const {
		Decoder16Factory* factory = getDecoder16Factory(name);
		if(!factory)
			throw NoSuchDecoderError(name);
		Decoder16* codec = factory->newCodec();
		if(!codec)
			throw NoSuchDecoderError(name);
		return codec;
	}

	Encoder16* CodecManager::newEncoder16(const string& name) {
		Encoder16Factory* factory = getEncoder16Factory(name);
		if(!factory) {
			Encoder16ResolverIterator resbegin(enc16resolv.begin()), resend(enc16resolv.end());
			for(; resbegin != resend; ++resbegin) {
				factory = (*resbegin)->resolveCodec(name);
				if(factory) {
					enc16reg[name] = factory;
					break;
				}
			}
		}
		if(!factory)
			throw NoSuchEncoderError(name);
		Encoder16* codec = factory->newCodec();
		if(!codec)
			throw NoSuchEncoderError(name);
		return codec;
	}

	Decoder16* CodecManager::newDecoder16(const string& name) {
		Decoder16Factory* factory = getDecoder16Factory(name);
		if(!factory) {
			Decoder16ResolverIterator resbegin(dec16resolv.begin()), resend(dec16resolv.end());
			for(; resbegin != resend; ++resbegin) {
				factory = (*resbegin)->resolveCodec(name);
				if(factory) {
					dec16reg[name] = factory;
					break;
				}
			}
		}
		if(!factory)
			throw NoSuchDecoderError(name);
		Decoder16* codec = factory->newCodec();
		if(!codec)
			throw NoSuchDecoderError(name);
		return codec;
	}

	void CodecManager::setEncoder16Factory(const string& name, Encoder16Factory* factory) {
		Encoder16Registry::iterator it(enc16reg.find(name));
		if(it != enc16reg.end()) {
			it->second->unref();
			enc16reg.erase(it);
		}
		if(factory) {
			enc16reg[name] = factory;
			factory->ref();
		}
	}

	void CodecManager::setDecoder16Factory(const string& name, Decoder16Factory* factory) {
		Decoder16Registry::iterator it(dec16reg.find(name));
		if(it != dec16reg.end()) {
			it->second->unref();
			dec16reg.erase(it);
		}
		if(factory) {
			dec16reg[name] = factory;
			factory->ref();
		}
	}

	void CodecManager::getEncoder16Factories(Encoder16Iterator& begin, Encoder16Iterator& end) const {
		begin = enc16reg.begin();
		end = enc16reg.end();
	}

	void CodecManager::getDecoder16Factories(Decoder16Iterator& begin, Decoder16Iterator& end) const {
		begin = dec16reg.begin();
		end = dec16reg.end();
	}

	void CodecManager::registerBuiltins() {
		Unref<Encoder16Factory> enc16factory(new DefaultCodecFactory<Encoder16, UTF8Encoder16>);
		setEncoder16Factory("UTF-8", *enc16factory);
		enc16factory.set()->unref();
		Unref<Decoder16Factory> dec16factory(new DefaultCodecFactory<Decoder16, UTF8Decoder16>);
		setDecoder16Factory("UTF-8", *dec16factory);
		dec16factory.set()->unref();
	}

	void CodecManager::registerBlobs() {
		BlobCodeTable16Registrar::registerCodecs(*this);
	}

	void CodecManager::purge() {
		Encoder16Iterator enc16begin(enc16reg.begin()), enc16end(enc16reg.end());
		for(; enc16begin != enc16end; ++enc16begin)
			enc16begin->second->unref();
		enc16reg.clear();
		Decoder16Iterator dec16begin(dec16reg.begin()), dec16end(dec16reg.end());
		for(; dec16begin != dec16end; ++dec16begin)
			dec16begin->second->unref();
		dec16reg.clear();
	}

	void CodecManager::getEncoder16Resolvers(Encoder16ResolverIterator& begin,
			Encoder16ResolverIterator& end) const {
		begin = enc16resolv.begin();
		end = enc16resolv.end();
	}

	void CodecManager::getDecoder16Resolvers(Decoder16ResolverIterator& begin,
			Decoder16ResolverIterator& end) const {
		begin = dec16resolv.begin();
		end = dec16resolv.end();
	}

	bool CodecManager::addEncoder16Resolver(Encoder16Resolver* resolver) {
		if(!resolver)
			return false;
		Encoder16ResolverIterator it(enc16resolv.find(resolver));
		if(it != enc16resolv.end())
			return false;
		enc16resolv.insert(resolver);
		resolver->ref();
		return true;
	}

	bool CodecManager::addDecoder16Resolver(Decoder16Resolver* resolver) {
		if(!resolver)
			return false;
		Decoder16ResolverIterator it(dec16resolv.find(resolver));
		if(it != dec16resolv.end())
			return false;
		dec16resolv.insert(resolver);
		resolver->ref();
		return true;
	}

	bool CodecManager::removeEncoder16Resolver(Encoder16Resolver* resolver) {
		Encoder16ResolverIterator it(enc16resolv.find(resolver));
		if(it == enc16resolv.end())
			return false;
		enc16resolv.erase(it);
		resolver->unref();
		return true;
	}

	bool CodecManager::removeDecoder16Resolver(Decoder16Resolver* resolver) {
		Decoder16ResolverIterator it(dec16resolv.find(resolver));
		if(it == dec16resolv.end())
			return false;
		dec16resolv.erase(it);
		resolver->unref();
		return true;
	}

	void CodecManager::listKnownEncoders16(set<string>& list) const {
		Encoder16Iterator enc16begin(enc16reg.begin()), enc16end(enc16reg.end());
		for(; enc16begin != enc16end; ++enc16begin)
			list.insert(enc16begin->first);
		Encoder16ResolverIterator enc16resbegin(enc16resolv.begin()), enc16resend(enc16resolv.end());
		for(; enc16resbegin != enc16resend; ++enc16resbegin)
			(*enc16resbegin)->listKnownCodecs(list);
	}

	void CodecManager::listKnownDecoders16(set<string>& list) const {
		Decoder16Iterator dec16begin(dec16reg.begin()), dec16end(dec16reg.end());
		for(; dec16begin != dec16end; ++dec16begin)
			list.insert(dec16begin->first);
		Decoder16ResolverIterator dec16resbegin(dec16resolv.begin()), dec16resend(dec16resolv.end());
		for(; dec16resbegin != dec16resend; ++dec16resbegin)
			(*dec16resbegin)->listKnownCodecs(list);
	}

	void CodecManager::setEncoder16CanonicalName(const string& alias, const string& canonical) {
		enc16cnames[alias] = canonical;
	}

	void CodecManager::setDecoder16CanonicalName(const string& alias, const string& canonical) {
		dec16cnames[alias] = canonical;
	}

	string CodecManager::getEncoder16CanonicalName(const string& alias) const {
		CanonicalNameIterator it = enc16cnames.find(alias);
		return it == enc16cnames.end() ? "" : it->second;
	}

	string CodecManager::getDecoder16CanonicalName(const string& alias) const {
		CanonicalNameIterator it = dec16cnames.find(alias);
		return it == dec16cnames.end() ? "" : it->second;
	}

	class DefaultCodecManagerSynchronizedSingleton : public SynchronizedSingleton<CodecManager> {

	  protected:
		virtual CodecManager* newInstance() {
			Delete<CodecManager> manager(new CodecManager);
			manager->registerBuiltins();
			manager->registerBlobs();
			return manager.set();
		}

	  public:
		DefaultCodecManagerSynchronizedSingleton() {}

	};

	static DefaultCodecManagerSynchronizedSingleton singleton;

	CodecManager& CodecManager::getDefaultCodecManager() {
		return singleton.get();
	}

}}
