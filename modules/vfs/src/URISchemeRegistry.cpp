#ifndef REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_CPP
#define REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_CPP

#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Encoder.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>
#include <redstrain/platform/ProviderLocker.hpp>

#include "RelativeURI.hpp"
#include "EmptyURIError.hpp"
#include "URISchemeRegistry.hpp"
#include "IllegalURIHeadError.hpp"
#include "HierarchicalURIScheme.hpp"
#include "UnhandeledURISchemeError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::Encoder16;
using redengine::text::Decoder16;
using redengine::text::Encoder32;
using redengine::text::Decoder32;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF8Decoder16;
using redengine::text::Transcoder1632;
using redengine::text::Transcoder3216;
using redengine::text::UTF16Encoder16;
using redengine::text::UTF16Decoder16;
using redengine::platform::ProviderLocker;

namespace redengine {
namespace vfs {

#define doMap(bits, func) \
	SchemeIterator ## bits begin ## bits(schemes ## bits.begin()), end ## bits(schemes ## bits.end()); \
	for(; begin ## bits != end ## bits; ++begin ## bits) \
		begin ## bits->second->func();

	URISchemeRegistry::URISchemeRegistry(URISchemeRegistry* parent) : parent(parent), lockProvider(NULL) {}

	URISchemeRegistry::URISchemeRegistry(const URISchemeRegistry& registry)
			: parent(registry.parent), lockProvider(NULL) {
		ProviderLocker<URISchemeRegistry> locker(registry.getLockProvider(), registry);
		schemes8 = registry.schemes8;
		schemes16 = registry.schemes16;
		schemes32 = registry.schemes32;
		locker.release();
		doMap(8, ref)
		doMap(16, ref)
		doMap(32, ref)
	}

	URISchemeRegistry::~URISchemeRegistry() {
		doMap(8, unref)
		doMap(16, unref)
		doMap(32, unref)
	}

	void URISchemeRegistry::getSchemes8(SchemeIterator8& begin, SchemeIterator8& end) const {
		begin = schemes8.begin();
		end = schemes8.end();
	}

	void URISchemeRegistry::getSchemes16(SchemeIterator16& begin, SchemeIterator16& end) const {
		begin = schemes16.begin();
		end = schemes16.end();
	}

	void URISchemeRegistry::getSchemes32(SchemeIterator32& begin, SchemeIterator32& end) const {
		begin = schemes32.begin();
		end = schemes32.end();
	}

#define makeSetScheme(bits, stype) \
	void URISchemeRegistry::setScheme ## bits(const stype& name, URIScheme* scheme) { \
		ProviderLocker<URISchemeRegistry> locker(lockProvider, this); \
		Schemes ## bits::iterator it(schemes ## bits.find(name)); \
		if(it != schemes ## bits.end()) { \
			it->second->unref(); \
			schemes ## bits.erase(it); \
		} \
		if(scheme) { \
			schemes ## bits[name] = scheme; \
			scheme->ref(); \
		} \
		locker.release(); \
	}

	makeSetScheme(8, string)
	makeSetScheme(16, String16)
	makeSetScheme(32, String32)

	void URISchemeRegistry::setScheme(const string& name8, URIScheme* scheme,
			Decoder16* decoder16, Decoder32* decoder32) {
		String16 name16;
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(name8, name16, decoder16 ? *decoder16 : utf8);
		String32 name32;
		if(decoder32)
			Transcode::transcodeString2<char, Char32>(name8, name32, *decoder32);
		else {
			utf8.reset();
			UTF16Decoder16 utf16;
			Transcode::transcodeString3<char, Char16, Char32>(name8, name32, utf8, utf16);
		}
		setScheme8(name8, scheme);
		setScheme16(name16, scheme);
		setScheme32(name32, scheme);
	}

	void URISchemeRegistry::setScheme(const String16& name16, URIScheme* scheme,
			Encoder16* encoder16, Transcoder1632* transcoder1632) {
		string name8;
		UTF8Encoder16 utf8;
		Transcode::transcodeString2<Char16, char>(name16, name8, encoder16 ? *encoder16 : utf8);
		String32 name32;
		UTF16Decoder16 utf16;
		Transcode::transcodeString2<Char16, Char32>(name16, name32, transcoder1632 ? *transcoder1632 : utf16);
		setScheme8(name8, scheme);
		setScheme16(name16, scheme);
		setScheme32(name32, scheme);
	}

	void URISchemeRegistry::setScheme(const String32& name32, URIScheme* scheme,
			Encoder32* encoder32, Transcoder3216* transcoder3216) {
		String16 name16;
		UTF16Encoder16 utf16;
		Transcode::transcodeString2<Char32, Char16>(name32, name16, transcoder3216 ? *transcoder3216 : utf16);
		string name8;
		if(encoder32)
			Transcode::transcodeString2<Char32, char>(name32, name8, *encoder32);
		else {
			utf16.reset();
			UTF8Encoder16 utf8;
			Transcode::transcodeString3<Char32, Char16, char>(name32, name8, utf16, utf8);
		}
		setScheme8(name8, scheme);
		setScheme16(name16, scheme);
		setScheme32(name32, scheme);
	}

#define makeClearSchemes(bits) \
	void URISchemeRegistry::clearSchemes ## bits() { \
		ProviderLocker<URISchemeRegistry> locker(lockProvider, this); \
		doMap(bits, unref); \
		schemes ## bits.clear(); \
		locker.release(); \
	}

	makeClearSchemes(8)
	makeClearSchemes(16)
	makeClearSchemes(32)

	void URISchemeRegistry::clearSchemes() {
		ProviderLocker<URISchemeRegistry> locker(lockProvider, this);
		doMap(8, unref)
		schemes8.clear();
		doMap(16, unref)
		schemes16.clear();
		doMap(32, unref);
		schemes32.clear();
		locker.release();
	}

#define makeGetScheme(bits, stype) \
	URIScheme* URISchemeRegistry::getScheme(const stype& name) const { \
		ProviderLocker<URISchemeRegistry> locker(lockProvider, this); \
		SchemeIterator ## bits it = schemes ## bits.find(name); \
		URIScheme* scheme = it == schemes ## bits.end() ? NULL : it->second; \
		locker.release(); \
		return scheme; \
	}

	makeGetScheme(8, string)
	makeGetScheme(16, String16)
	makeGetScheme(32, String32)

#define makeResolveScheme(bits, stype) \
	URIScheme* URISchemeRegistry::resolveScheme(const stype& name) const { \
		const URISchemeRegistry* registry = this; \
		do { \
			ProviderLocker<URISchemeRegistry> locker(registry->lockProvider, registry); \
			Schemes ## bits::const_iterator it(registry->schemes ## bits.find(name)); \
			if(it != registry->schemes ## bits.end()) { \
				URIScheme* scheme = it->second; \
				locker.release(); \
				return scheme; \
			} \
			locker.release(); \
			registry = registry->parent; \
		} while(registry); \
		return NULL; \
	}

	makeResolveScheme(8, string)
	makeResolveScheme(16, String16)
	makeResolveScheme(32, String32)

#define makeNewURI(bits, stype, ctype) \
	URI* URISchemeRegistry::newURI(const stype& specifier) const { \
		return parseURIHead<ctype>(specifier); \
	}

	makeNewURI(8, string, char)
	makeNewURI(16, String16, Char16)
	makeNewURI(32, String32, Char32)

	URI* URISchemeRegistry::makeRelativeURI(const string& specifier) {
		return URISchemeRegistry::makeRelativeURI(Transcode::utf8ToBMP(specifier));
	}

	URI* URISchemeRegistry::makeRelativeURI(const String16& specifier) {
		if(specifier.empty())
			throw EmptyURIError();
		HierarchicalURI::Decomposition decomposition;
		HierarchicalURIScheme::parseHierarchicalURI<Char16>(specifier, static_cast<String16::size_type>(0u),
				decomposition, true);
		return new RelativeURI(specifier, decomposition);
	}

	URI* URISchemeRegistry::makeRelativeURI(const String32& specifier) {
		UTF16Encoder16 utf16;
		return URISchemeRegistry::makeRelativeURI(Transcode::transcodeShortString2<Char32, Char16>(specifier,
				utf16));
	}

#define makeMakeAbsoluteURI(bits, stype) \
	URI* URISchemeRegistry::makeAbsoluteURI(const stype& specifier, stype::size_type colonPosition) const { \
		stype ss(specifier.substr(static_cast<stype::size_type>(0u), colonPosition)); \
		URIScheme* parser = resolveScheme(ss); \
		if(!parser) \
			throw UnhandeledURISchemeError(ss); \
		return parser->newURI(specifier, colonPosition + static_cast<stype::size_type>(1u)); \
	}

	makeMakeAbsoluteURI(8, string)
	makeMakeAbsoluteURI(16, String16)
	makeMakeAbsoluteURI(32, String32)

#define makeIllegalURIHead(stype) \
	void URISchemeRegistry::illegalURIHead(const stype& specifier, stype::size_type offset) { \
		throw IllegalURIHeadError(specifier, offset); \
	}

	makeIllegalURIHead(string)
	makeIllegalURIHead(String16)
	makeIllegalURIHead(String32)

}}

#endif /* REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_CPP */
