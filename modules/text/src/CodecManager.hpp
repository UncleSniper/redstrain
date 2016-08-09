#ifndef REDSTRAIN_MOD_TEXT_CODECMANAGER_HPP
#define REDSTRAIN_MOD_TEXT_CODECMANAGER_HPP

#include <map>
#include <redstrain/platform/LockProvider.hpp>

#include "TextCodec.hpp"
#include "CodecResolver.hpp"
#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API CodecManager {

	  public:
		typedef CodecFactory<Encoder16> Encoder16Factory;
		typedef CodecFactory<Decoder16> Decoder16Factory;
		typedef CodecFactory<Encoder32> Encoder32Factory;
		typedef CodecFactory<Decoder32> Decoder32Factory;
		typedef CodecFactory<Transcoder1632> Transcoder1632Factory;
		typedef CodecFactory<Transcoder3216> Transcoder3216Factory;

		typedef std::map<std::string, Encoder16Factory*> Encoder16Registry;
		typedef std::map<std::string, Decoder16Factory*> Decoder16Registry;
		typedef std::map<std::string, Encoder32Factory*> Encoder32Registry;
		typedef std::map<std::string, Decoder32Factory*> Decoder32Registry;
		typedef std::map<std::string, Transcoder1632Factory*> Transcoder1632Registry;
		typedef std::map<std::string, Transcoder3216Factory*> Transcoder3216Registry;

		typedef Encoder16Registry::const_iterator Encoder16Iterator;
		typedef Decoder16Registry::const_iterator Decoder16Iterator;
		typedef Encoder32Registry::const_iterator Encoder32Iterator;
		typedef Decoder32Registry::const_iterator Decoder32Iterator;
		typedef Transcoder1632Registry::const_iterator Transcoder1632Iterator;
		typedef Transcoder3216Registry::const_iterator Transcoder3216Iterator;

		typedef CodecResolver<Encoder16> Encoder16Resolver;
		typedef CodecResolver<Decoder16> Decoder16Resolver;
		typedef CodecResolver<Encoder32> Encoder32Resolver;
		typedef CodecResolver<Decoder32> Decoder32Resolver;
		typedef CodecResolver<Transcoder1632> Transcoder1632Resolver;
		typedef CodecResolver<Transcoder3216> Transcoder3216Resolver;

		typedef std::set<Encoder16Resolver*> Encoder16Resolvers;
		typedef std::set<Decoder16Resolver*> Decoder16Resolvers;
		typedef std::set<Encoder32Resolver*> Encoder32Resolvers;
		typedef std::set<Decoder32Resolver*> Decoder32Resolvers;
		typedef std::set<Transcoder1632Resolver*> Transcoder1632Resolvers;
		typedef std::set<Transcoder3216Resolver*> Transcoder3216Resolvers;

		typedef Encoder16Resolvers::const_iterator Encoder16ResolverIterator;
		typedef Decoder16Resolvers::const_iterator Decoder16ResolverIterator;
		typedef Encoder32Resolvers::const_iterator Encoder32ResolverIterator;
		typedef Decoder32Resolvers::const_iterator Decoder32ResolverIterator;
		typedef Transcoder1632Resolvers::const_iterator Transcoder1632ResolverIterator;
		typedef Transcoder3216Resolvers::const_iterator Transcoder3216ResolverIterator;

		typedef std::map<std::string, std::string> CanonicalNames;
		typedef CanonicalNames::const_iterator CanonicalNameIterator;

	  private:
		Encoder16Registry enc16reg;
		Decoder16Registry dec16reg;
		Encoder32Registry enc32reg;
		Decoder32Registry dec32reg;
		Transcoder1632Registry trc1632reg;
		Transcoder3216Registry trc3216reg;
		Encoder16Resolvers enc16resolv;
		Decoder16Resolvers dec16resolv;
		Encoder32Resolvers enc32resolv;
		Decoder32Resolvers dec32resolv;
		Transcoder1632Resolvers trc1632resolv;
		Transcoder3216Resolvers trc3216resolv;
		CanonicalNames enc16cnames, dec16cnames;
		CanonicalNames enc32cnames, dec32cnames;
		CanonicalNames trc1632cnames, trc3216cnames;
		platform::LockProvider<CodecManager>* lockProvider;

	  public:
		CodecManager();
		CodecManager(const CodecManager&);
		~CodecManager();

		inline platform::LockProvider<CodecManager>* getLockProvider() const {
			return lockProvider;
		}

		inline void setLockProvider(platform::LockProvider<CodecManager>* lockProvider) {
			this->lockProvider = lockProvider;
		}

		Encoder16Factory* getEncoder16Factory(const std::string&) const;
		Decoder16Factory* getDecoder16Factory(const std::string&) const;
		Encoder32Factory* getEncoder32Factory(const std::string&) const;
		Decoder32Factory* getDecoder32Factory(const std::string&) const;
		Transcoder1632Factory* getTranscoder1632Factory(const std::string&) const;
		Transcoder3216Factory* getTranscoder3216Factory(const std::string&) const;

		Encoder16Factory* loadEncoder16Factory(const std::string&);
		Decoder16Factory* loadDecoder16Factory(const std::string&);
		Encoder32Factory* loadEncoder32Factory(const std::string&);
		Decoder32Factory* loadDecoder32Factory(const std::string&);
		Transcoder1632Factory* loadTranscoder1632Factory(const std::string&);
		Transcoder3216Factory* loadTranscoder3216Factory(const std::string&);

		Encoder16* getEncoder16(const std::string&) const;
		Decoder16* getDecoder16(const std::string&) const;
		Encoder32* getEncoder32(const std::string&, bool) const;
		Decoder32* getDecoder32(const std::string&, bool) const;
		Transcoder1632* getTranscoder1632(const std::string&) const;
		Transcoder3216* getTranscoder3216(const std::string&) const;

		Encoder16* newEncoder16(const std::string&);
		Decoder16* newDecoder16(const std::string&);
		Encoder32* newEncoder32(const std::string&, bool);
		Decoder32* newDecoder32(const std::string&, bool);
		Transcoder1632* newTranscoder1632(const std::string&);
		Transcoder3216* newTranscoder3216(const std::string&);

		void setEncoder16Factory(const std::string&, Encoder16Factory*);
		void setDecoder16Factory(const std::string&, Decoder16Factory*);
		void setEncoder32Factory(const std::string&, Encoder32Factory*);
		void setDecoder32Factory(const std::string&, Decoder32Factory*);
		void setTranscoder1632Factory(const std::string&, Transcoder1632Factory*);
		void setTranscoder3216Factory(const std::string&, Transcoder3216Factory*);

		void getEncoder16Factories(Encoder16Iterator&, Encoder16Iterator&) const;
		void getDecoder16Factories(Decoder16Iterator&, Decoder16Iterator&) const;
		void getEncoder32Factories(Encoder32Iterator&, Encoder32Iterator&) const;
		void getDecoder32Factories(Decoder32Iterator&, Decoder32Iterator&) const;
		void getTranscoder1632Factories(Transcoder1632Iterator&, Transcoder1632Iterator&) const;
		void getTranscoder3216Factories(Transcoder3216Iterator&, Transcoder3216Iterator&) const;

		void registerBuiltins();
		void registerBlobs();
		void purge();

		void getEncoder16Resolvers(Encoder16ResolverIterator&, Encoder16ResolverIterator&) const;
		void getDecoder16Resolvers(Decoder16ResolverIterator&, Decoder16ResolverIterator&) const;
		void getEncoder32Resolvers(Encoder32ResolverIterator&, Encoder32ResolverIterator&) const;
		void getDecoder32Resolvers(Decoder32ResolverIterator&, Decoder32ResolverIterator&) const;
		void getTranscoder1632Resolvers(Transcoder1632ResolverIterator&, Transcoder1632ResolverIterator&) const;
		void getTranscoder3216Resolvers(Transcoder3216ResolverIterator&, Transcoder3216ResolverIterator&) const;

		bool addEncoder16Resolver(Encoder16Resolver*);
		bool addDecoder16Resolver(Decoder16Resolver*);
		bool addEncoder32Resolver(Encoder32Resolver*);
		bool addDecoder32Resolver(Decoder32Resolver*);
		bool addTranscoder1632Resolver(Transcoder1632Resolver*);
		bool addTranscoder3216Resolver(Transcoder3216Resolver*);

		bool removeEncoder16Resolver(Encoder16Resolver*);
		bool removeDecoder16Resolver(Decoder16Resolver*);
		bool removeEncoder32Resolver(Encoder32Resolver*);
		bool removeDecoder32Resolver(Decoder32Resolver*);
		bool removeTranscoder1632Resolver(Transcoder1632Resolver*);
		bool removeTranscoder3216Resolver(Transcoder3216Resolver*);

		void listKnownEncoders16(std::set<std::string>&) const;
		void listKnownDecoders16(std::set<std::string>&) const;
		void listKnownEncoders32(std::set<std::string>&) const;
		void listKnownDecoders32(std::set<std::string>&) const;
		void listKnownTranscoders1632(std::set<std::string>&) const;
		void listKnownTranscoders3216(std::set<std::string>&) const;

		void setEncoder16CanonicalName(const std::string&, const std::string&);
		void setDecoder16CanonicalName(const std::string&, const std::string&);
		void setEncoder32CanonicalName(const std::string&, const std::string&);
		void setDecoder32CanonicalName(const std::string&, const std::string&);
		void setTranscoder1632CanonicalName(const std::string&, const std::string&);
		void setTranscoder3216CanonicalName(const std::string&, const std::string&);

		std::string getEncoder16CanonicalName(const std::string&) const;
		std::string getDecoder16CanonicalName(const std::string&) const;
		std::string getEncoder32CanonicalName(const std::string&) const;
		std::string getDecoder32CanonicalName(const std::string&) const;
		std::string getTranscoder1632CanonicalName(const std::string&) const;
		std::string getTranscoder3216CanonicalName(const std::string&) const;

		static CodecManager& getDefaultCodecManager();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CODECMANAGER_HPP */
