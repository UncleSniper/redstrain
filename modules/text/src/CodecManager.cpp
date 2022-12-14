#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/ProviderLocker.hpp>
#include <redstrain/platform/MutexLockProvider.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "UTF8Encoder.hpp"
#include "UTF8Decoder.hpp"
#include "CodecManager.hpp"
#include "UTF16Encoder8.hpp"
#include "UTF16Decoder8.hpp"
#include "UTF16Encoder16.hpp"
#include "UTF16Decoder16.hpp"
#include "ChainedTextCodec.hpp"
#include "NoSuchEncoderError.hpp"
#include "NoSuchDecoderError.hpp"
#include "DefaultCodecFactory.hpp"
#include "BlobCodeTable16Registrar.hpp"

using std::set;
using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::platform::Mutex;
using redengine::platform::ProviderLocker;
using redengine::platform::MutexLockProvider;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace text {

#define doMap(itype, member, func) \
	itype ## Iterator member ## _begin(member.begin()), member ## _end(member.end()); \
	for(; member ## _begin != member ## _end; ++member ## _begin) \
		member ## _begin->second->func();
#define doSet(itype, member, func) \
	itype ## Iterator member ## _begin(member.begin()), member ## _end(member.end()); \
	for(; member ## _begin != member ## _end; ++member ## _begin) \
		(*member ## _begin)->func();

	CodecManager::CodecManager() : lockProvider(NULL) {}

	CodecManager::CodecManager(const CodecManager& manager) : lockProvider(NULL) {
		ProviderLocker<CodecManager> locker(manager.getLockProvider(), manager);
		enc16reg = manager.enc16reg;
		dec16reg = manager.dec16reg;
		enc32reg = manager.enc32reg;
		dec32reg = manager.dec32reg;
		trc1632reg = manager.trc1632reg;
		trc3216reg = manager.trc3216reg;
		enc16resolv = manager.enc16resolv;
		dec16resolv = manager.dec16resolv;
		enc32resolv = manager.enc32resolv;
		dec32resolv = manager.dec32resolv;
		trc1632resolv = manager.trc1632resolv;
		trc3216resolv = manager.trc3216resolv;
		enc16cnames = manager.enc16cnames;
		dec16cnames = manager.dec16cnames;
		enc32cnames = manager.enc32cnames;
		dec32cnames = manager.dec32cnames;
		trc1632cnames = manager.trc1632cnames;
		trc3216cnames = manager.trc3216cnames;
		locker.release();
		doMap(Encoder16, enc16reg, ref)
		doMap(Decoder16, dec16reg, ref)
		doMap(Encoder32, enc32reg, ref)
		doMap(Decoder32, dec32reg, ref)
		doMap(Transcoder1632, trc1632reg, ref)
		doMap(Transcoder3216, trc3216reg, ref)
		doSet(Encoder16Resolver, enc16resolv, ref)
		doSet(Decoder16Resolver, dec16resolv, ref)
		doSet(Encoder32Resolver, enc32resolv, ref)
		doSet(Decoder32Resolver, dec32resolv, ref)
		doSet(Transcoder1632Resolver, trc1632resolv, ref)
		doSet(Transcoder3216Resolver, trc3216resolv, ref)
	}

	CodecManager::~CodecManager() {
		doMap(Encoder16, enc16reg, unref)
		doMap(Decoder16, dec16reg, unref)
		doMap(Encoder32, enc32reg, unref)
		doMap(Decoder32, dec32reg, unref)
		doMap(Transcoder1632, trc1632reg, unref)
		doMap(Transcoder3216, trc3216reg, unref)
		doSet(Encoder16Resolver, enc16resolv, unref)
		doSet(Decoder16Resolver, dec16resolv, unref)
		doSet(Encoder32Resolver, enc32resolv, unref)
		doSet(Decoder32Resolver, dec32resolv, unref)
		doSet(Transcoder1632Resolver, trc1632resolv, unref)
		doSet(Transcoder3216Resolver, trc3216resolv, unref)
	}

#define makeGetFactory(ftype, member) \
	CodecManager::ftype ## Factory* CodecManager::get ## ftype ## Factory(const string& name) const { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		ftype ## Iterator it = member.find(name); \
		ftype ## Factory* factory = it == member.end() ? NULL : it->second; \
		locker.release(); \
		return factory; \
	}

	makeGetFactory(Encoder16, enc16reg)
	makeGetFactory(Decoder16, dec16reg)
	makeGetFactory(Encoder32, enc32reg)
	makeGetFactory(Decoder32, dec32reg)
	makeGetFactory(Transcoder1632, trc1632reg)
	makeGetFactory(Transcoder3216, trc3216reg)

#define makeLoadFactory(ftype, regMember, resolvMember) \
	CodecManager::ftype ## Factory* CodecManager::load ## ftype ## Factory(const string& name) { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		ftype ## Iterator it = regMember.find(name); \
		ftype ## Factory* factory; \
		if(it != regMember.end()) { \
			factory = it->second; \
			locker.release(); \
			return factory; \
		} \
		factory = NULL; \
		ftype ## ResolverIterator resbegin(resolvMember.begin()), resend(resolvMember.end()); \
		for(; resbegin != resend; ++resbegin) { \
			factory = (*resbegin)->resolveCodec(name); \
			if(factory) { \
				regMember[name] = factory; \
				factory->ref(); \
				break; \
			} \
		} \
		locker.release(); \
		return factory; \
	}

	makeLoadFactory(Encoder16, enc16reg, enc16resolv)
	makeLoadFactory(Decoder16, dec16reg, dec16resolv)
	makeLoadFactory(Encoder32, enc32reg, enc32resolv)
	makeLoadFactory(Decoder32, dec32reg, dec32resolv)
	makeLoadFactory(Transcoder1632, trc1632reg, trc1632resolv)
	makeLoadFactory(Transcoder3216, trc3216reg, trc3216resolv)

#define makeGet(ftype, member, except) \
	ftype* CodecManager::get ## ftype(const string& name) const { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		ftype ## Iterator it = member.find(name); \
		ftype ## Factory* factory = it == member.end() ? NULL : it->second; \
		locker.release(); \
		if(!factory) \
			throw NoSuch ## except ## Error(name); \
		ftype* codec = factory->newCodec(); \
		if(!codec) \
			throw NoSuch ## except ## Error(name); \
		return codec; \
	}

	makeGet(Encoder16, enc16reg, Encoder)
	makeGet(Decoder16, dec16reg, Decoder)
	makeGet(Transcoder1632, trc1632reg, Codec)
	makeGet(Transcoder3216, trc3216reg, Codec)

	Encoder32* CodecManager::getEncoder32(const string& name, bool allowIndirect) const {
		ProviderLocker<CodecManager> locker(lockProvider, this);
		Encoder32Iterator it32 = enc32reg.find(name);
		if(it32 != enc32reg.end()) {
			Encoder32* codec = it32->second->newCodec();
			if(codec) {
				locker.release();
				return codec;
			}
		}
		if(allowIndirect) {
			Encoder16Iterator it16 = enc16reg.find(name);
			if(it16 != enc16reg.end()) {
				Delete<Encoder16> encoder16(it16->second->newCodec());
				if(*encoder16) {
					locker.release();
					Delete<Transcoder3216> encoder32(new UTF16Encoder16);
					ChainedTextCodec<Char32, Char16, char>* chained = new ChainedTextCodec<Char32, Char16, char>(
						**encoder32,
						**encoder16,
						ChainedTextCodec<Char32, Char16, char>::FL_MANAGE_BOTH_CODECS
					);
					encoder16.set();
					encoder32.set();
					return chained;
				}
			}
		}
		locker.release();
		throw NoSuchEncoderError(name);
	}

	Decoder32* CodecManager::getDecoder32(const string& name, bool allowIndirect) const {
		ProviderLocker<CodecManager> locker(lockProvider, this);
		Decoder32Iterator it32 = dec32reg.find(name);
		if(it32 != dec32reg.end()) {
			Decoder32* codec = it32->second->newCodec();
			if(codec) {
				locker.release();
				return codec;
			}
		}
		if(allowIndirect) {
			Decoder16Iterator it16 = dec16reg.find(name);
			if(it16 != dec16reg.end()) {
				Delete<Decoder16> decoder16(it16->second->newCodec());
				if(*decoder16) {
					locker.release();
					Delete<Transcoder1632> decoder32(new UTF16Decoder16);
					ChainedTextCodec<char, Char16, Char32>* chained = new ChainedTextCodec<char, Char16, Char32>(
						**decoder16,
						**decoder32,
						ChainedTextCodec<char, Char16, Char32>::FL_MANAGE_BOTH_CODECS
					);
					decoder16.set();
					decoder32.set();
					return chained;
				}
			}
		}
		locker.release();
		throw NoSuchDecoderError(name);
	}

#define makeNew(ftype, regMember, resolvMember, except) \
	ftype* CodecManager::new ## ftype(const string& name) { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		ftype ## Iterator it = regMember.find(name); \
		ftype ## Factory* factory = it == regMember.end() ? NULL : it->second; \
		if(!factory) { \
			ftype ## ResolverIterator resbegin(resolvMember.begin()), resend(resolvMember.end()); \
			for(; resbegin != resend; ++resbegin) { \
				factory = (*resbegin)->resolveCodec(name); \
				if(factory) { \
					regMember[name] = factory; \
					factory->ref(); \
					break; \
				} \
			} \
			locker.release(); \
			if(!factory) \
				throw NoSuch ## except ## Error(name); \
		} \
		else \
			locker.release(); \
		ftype* codec = factory->newCodec(); \
		if(!codec) \
			throw NoSuch ## except ## Error(name); \
		return codec; \
	}

	makeNew(Encoder16, enc16reg, enc16resolv, Encoder)
	makeNew(Decoder16, dec16reg, dec16resolv, Decoder)
	makeNew(Transcoder1632, trc1632reg, trc1632resolv, Codec)
	makeNew(Transcoder3216, trc3216reg, trc3216resolv, Codec)

#define makeSetFactory(ftype, member) \
	void CodecManager::set ## ftype ## Factory(const string& name, ftype ## Factory* factory) { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		ftype ## Registry::iterator it(member.find(name)); \
		if(it != member.end()) { \
			it->second->unref(); \
			member.erase(it); \
		} \
		if(factory) { \
			member[name] = factory; \
			factory->ref(); \
		} \
		locker.release(); \
	}

	makeSetFactory(Encoder16, enc16reg)
	makeSetFactory(Decoder16, dec16reg)
	makeSetFactory(Encoder32, enc32reg)
	makeSetFactory(Decoder32, dec32reg)
	makeSetFactory(Transcoder1632, trc1632reg)
	makeSetFactory(Transcoder3216, trc3216reg)

#define makeGetFactories(ftype, member) \
	void CodecManager::get ## ftype ## Factories(ftype ## Iterator& begin, ftype ## Iterator& end) const { \
		begin = member.begin(); \
		end = member.end(); \
	}

	makeGetFactories(Encoder16, enc16reg)
	makeGetFactories(Decoder16, dec16reg)
	makeGetFactories(Encoder32, enc32reg)
	makeGetFactories(Decoder32, dec32reg)
	makeGetFactories(Transcoder1632, trc1632reg)
	makeGetFactories(Transcoder3216, trc3216reg)

	void CodecManager::registerBuiltins() {
		// Encoder16
		Unref<Encoder16Factory> enc16factory(new DefaultCodecFactory<Encoder16, UTF8Encoder16>);
		setEncoder16Factory("UTF-8", *enc16factory);
		setEncoder16Factory("utf8", *enc16factory);
		setEncoder16CanonicalName("utf8", "UTF-8");
		enc16factory.set()->unref();
		// Decoder16
		Unref<Decoder16Factory> dec16factory(new DefaultCodecFactory<Decoder16, UTF8Decoder16>);
		setDecoder16Factory("UTF-8", *dec16factory);
		setDecoder16Factory("utf8", *dec16factory);
		setDecoder16CanonicalName("utf8", "UTF-8");
		dec16factory.set()->unref();
		// Encoder32
		Unref<Encoder32Factory> enc32factory(new DefaultCodecFactory<Encoder32, UTF16Encoder8>);
		setEncoder32Factory("UTF-16", *enc32factory);
		setEncoder32Factory("utf16", *enc32factory);
		setEncoder32CanonicalName("utf16", "UTF-16");
		enc32factory.set()->unref();
		// Decoder32
		Unref<Decoder32Factory> dec32factory(new DefaultCodecFactory<Decoder32, UTF16Decoder8>);
		setDecoder32Factory("UTF-16", *dec32factory);
		setDecoder32Factory("utf16", *dec32factory);
		setDecoder32CanonicalName("utf16", "UTF-16");
		dec32factory.set()->unref();
		// Transcoder1632
		Unref<Transcoder1632Factory> trc1632factory(new DefaultCodecFactory<Transcoder1632, UTF16Decoder16>);
		setTranscoder1632Factory("UTF-16", *trc1632factory);
		setTranscoder1632Factory("utf16", *trc1632factory);
		setTranscoder1632CanonicalName("utf16", "UTF-16");
		trc1632factory.set()->unref();
		// Transcoder3216
		Unref<Transcoder3216Factory> trc3216factory(new DefaultCodecFactory<Transcoder3216, UTF16Encoder16>);
		setTranscoder3216Factory("UTF-16", *trc3216factory);
		setTranscoder3216Factory("utf16", *trc3216factory);
		setTranscoder3216CanonicalName("utf16", "UTF-16");
		trc3216factory.set()->unref();
	}

	void CodecManager::registerBlobs() {
		ProviderLocker<CodecManager> locker(lockProvider, this);
		BlobCodeTable16Registrar::registerCodecs(*this);
		locker.release();
	}

	void CodecManager::purge() {
		ProviderLocker<CodecManager> locker(lockProvider, this);
		doMap(Encoder16, enc16reg, unref)
		enc16reg.clear();
		doMap(Decoder16, dec16reg, unref)
		dec16reg.clear();
		doMap(Encoder32, enc32reg, unref)
		enc32reg.clear();
		doMap(Decoder32, dec32reg, unref)
		dec32reg.clear();
		doMap(Transcoder1632, trc1632reg, unref)
		trc1632reg.clear();
		doMap(Transcoder3216, trc3216reg, unref)
		trc3216reg.clear();
		locker.release();
	}

#define makeGetResolvers(ftype, member) \
	void CodecManager::get ## ftype ## Resolvers(ftype ## ResolverIterator& begin, \
			ftype ## ResolverIterator& end) const { \
		begin = member.begin(); \
		end = member.end(); \
	}

	makeGetResolvers(Encoder16, enc16resolv)
	makeGetResolvers(Decoder16, dec16resolv)
	makeGetResolvers(Encoder32, enc32resolv)
	makeGetResolvers(Decoder32, dec32resolv)
	makeGetResolvers(Transcoder1632, trc1632resolv)
	makeGetResolvers(Transcoder3216, trc3216resolv)

#define makeAddResolver(ftype, member) \
	bool CodecManager::add ## ftype ## Resolver(ftype ## Resolver* resolver) { \
		if(!resolver) \
			return false; \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		ftype ## ResolverIterator it(member.find(resolver)); \
		if(it != member.end()) {\
			locker.release(); \
			return false; \
		} \
		member.insert(resolver); \
		resolver->ref(); \
		locker.release(); \
		return true; \
	}

	makeAddResolver(Encoder16, enc16resolv)
	makeAddResolver(Decoder16, dec16resolv)
	makeAddResolver(Encoder32, enc32resolv)
	makeAddResolver(Decoder32, dec32resolv)
	makeAddResolver(Transcoder1632, trc1632resolv)
	makeAddResolver(Transcoder3216, trc3216resolv)

#define makeRemoveResolver(ftype, member) \
	bool CodecManager::remove ## ftype ## Resolver(ftype ## Resolver* resolver) { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		ftype ## ResolverIterator it(member.find(resolver)); \
		if(it == member.end()) {\
			locker.release(); \
			return false; \
		} \
		member.erase(it); \
		resolver->unref(); \
		locker.release(); \
		return true; \
	}

	makeRemoveResolver(Encoder16, enc16resolv)
	makeRemoveResolver(Decoder16, dec16resolv)
	makeRemoveResolver(Encoder32, enc32resolv)
	makeRemoveResolver(Decoder32, dec32resolv)
	makeRemoveResolver(Transcoder1632, trc1632resolv)
	makeRemoveResolver(Transcoder3216, trc3216resolv)

#define makeListKnown(ftype, ptype, regMember, resolvMember) \
	void CodecManager::listKnown ## ptype(set<string>& list) const { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		ftype ## Iterator regbegin(regMember.begin()), regend(regMember.end()); \
		for(; regbegin != regend; ++regbegin) \
			list.insert(regbegin->first); \
		ftype ## ResolverIterator resbegin(resolvMember.begin()), resend(resolvMember.end()); \
		for(; resbegin != resend; ++resbegin) \
			(*resbegin)->listKnownCodecs(list); \
		locker.release(); \
	}

	makeListKnown(Encoder16, Encoders16, enc16reg, enc16resolv)
	makeListKnown(Decoder16, Decoders16, dec16reg, dec16resolv)
	makeListKnown(Encoder32, Encoders32, enc32reg, enc32resolv)
	makeListKnown(Decoder32, Decoders32, dec32reg, dec32resolv)
	makeListKnown(Transcoder1632, Transcoders1632, trc1632reg, trc1632resolv)
	makeListKnown(Transcoder3216, Transcoders3216, trc3216reg, trc3216resolv)

#define makeSetCanonicalName(ftype, member) \
	void CodecManager::set ## ftype ## CanonicalName(const string& alias, const string& canonical) { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		member[alias] = canonical; \
		locker.release(); \
	}

	makeSetCanonicalName(Encoder16, enc16cnames)
	makeSetCanonicalName(Decoder16, dec16cnames)
	makeSetCanonicalName(Encoder32, enc32cnames)
	makeSetCanonicalName(Decoder32, dec32cnames)
	makeSetCanonicalName(Transcoder1632, trc1632cnames)
	makeSetCanonicalName(Transcoder3216, trc3216cnames)

#define makeGetCanonicalName(ftype, member) \
	string CodecManager::get ## ftype ## CanonicalName(const string& alias) const { \
		ProviderLocker<CodecManager> locker(lockProvider, this); \
		CanonicalNameIterator it = member.find(alias); \
		string result(it == member.end() ? "" : it->second); \
		locker.release(); \
		return result; \
	}

	makeGetCanonicalName(Encoder16, enc16cnames)
	makeGetCanonicalName(Decoder16, dec16cnames)
	makeGetCanonicalName(Encoder32, enc32cnames)
	makeGetCanonicalName(Decoder32, dec32cnames)
	makeGetCanonicalName(Transcoder1632, trc1632cnames)
	makeGetCanonicalName(Transcoder3216, trc3216cnames)

	class DefaultCodecManagerSynchronizedSingleton : public SynchronizedSingleton<CodecManager> {

	  protected:
		virtual CodecManager* newInstance() {
			Delete<CodecManager> manager(new CodecManager);
			manager->registerBuiltins();
			manager->registerBlobs();
			Delete<Mutex> mutex(new Mutex);
			manager->setLockProvider(new MutexLockProvider<CodecManager>(**mutex, true));
			mutex.set();
			return manager.set();
		}

	  public:
		DefaultCodecManagerSynchronizedSingleton() {}

		DefaultCodecManagerSynchronizedSingleton(const DefaultCodecManagerSynchronizedSingleton& singleton)
				: SynchronizedSingleton<CodecManager>(singleton) {}

	};

	static DefaultCodecManagerSynchronizedSingleton singleton;

	CodecManager& CodecManager::getDefaultCodecManager() {
		return singleton.get();
	}

}}
