#ifndef REDSTRAIN_MOD_TEXT_CODECMANAGER_HPP
#define REDSTRAIN_MOD_TEXT_CODECMANAGER_HPP

#include <map>

#include "TextCodec.hpp"
#include "CodecResolver.hpp"
#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API CodecManager {

	  public:
		typedef CodecFactory<Encoder16> Encoder16Factory;
		typedef CodecFactory<Decoder16> Decoder16Factory;
		typedef std::map<std::string, Encoder16Factory*> Encoder16Registry;
		typedef Encoder16Registry::const_iterator Encoder16Iterator;
		typedef std::map<std::string, Decoder16Factory*> Decoder16Registry;
		typedef Decoder16Registry::const_iterator Decoder16Iterator;
		typedef CodecResolver<Encoder16> Encoder16Resolver;
		typedef CodecResolver<Decoder16> Decoder16Resolver;
		typedef std::set<Encoder16Resolver*> Encoder16Resolvers;
		typedef Encoder16Resolvers::const_iterator Encoder16ResolverIterator;
		typedef std::set<Decoder16Resolver*> Decoder16Resolvers;
		typedef Decoder16Resolvers::const_iterator Decoder16ResolverIterator;
		typedef std::map<std::string, std::string> CanonicalNames;
		typedef CanonicalNames::const_iterator CanonicalNameIterator;

	  private:
		Encoder16Registry enc16reg;
		Decoder16Registry dec16reg;
		Encoder16Resolvers enc16resolv;
		Decoder16Resolvers dec16resolv;
		CanonicalNames enc16cnames, dec16cnames;

	  public:
		CodecManager();
		CodecManager(const CodecManager&);
		~CodecManager();

		Encoder16Factory* getEncoder16Factory(const std::string&) const;
		Decoder16Factory* getDecoder16Factory(const std::string&) const;

		Encoder16* getEncoder16(const std::string&) const;
		Decoder16* getDecoder16(const std::string&) const;
		Encoder16* newEncoder16(const std::string&);
		Decoder16* newDecoder16(const std::string&);

		void setEncoder16Factory(const std::string&, Encoder16Factory*);
		void setDecoder16Factory(const std::string&, Decoder16Factory*);
		void getEncoder16Factories(Encoder16Iterator&, Encoder16Iterator&) const;
		void getDecoder16Factories(Decoder16Iterator&, Decoder16Iterator&) const;

		void registerBuiltins();
		void registerBlobs();
		void purge();

		void getEncoder16Resolvers(Encoder16ResolverIterator&, Encoder16ResolverIterator&) const;
		void getDecoder16Resolvers(Decoder16ResolverIterator&, Decoder16ResolverIterator&) const;
		bool addEncoder16Resolver(Encoder16Resolver*);
		bool addDecoder16Resolver(Decoder16Resolver*);
		bool removeEncoder16Resolver(Encoder16Resolver*);
		bool removeDecoder16Resolver(Decoder16Resolver*);

		void listKnownEncoders16(std::set<std::string>&) const;
		void listKnownDecoders16(std::set<std::string>&) const;

		void setEncoder16CanonicalName(const std::string&, const std::string&);
		void setDecoder16CanonicalName(const std::string&, const std::string&);
		std::string getEncoder16CanonicalName(const std::string&) const;
		std::string getDecoder16CanonicalName(const std::string&) const;

		static CodecManager& getDefaultCodecManager();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CODECMANAGER_HPP */
