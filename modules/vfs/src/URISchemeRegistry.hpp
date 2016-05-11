#ifndef REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_HPP
#define REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_HPP

#include <map>
#include <redstrain/text/TextCodec.hpp>
#include <redstrain/platform/LockProvider.hpp>

#include "api.hpp"

namespace redengine {
namespace vfs {

	class URI;
	class URIScheme;

	class REDSTRAIN_VFS_API URISchemeRegistry {

	  private:
		typedef std::map<std::string, URIScheme*> Schemes8;
		typedef std::map<text::String16, URIScheme*> Schemes16;
		typedef std::map<text::String32, URIScheme*> Schemes32;

	  public:
		typedef Schemes8::const_iterator SchemeIterator8;
		typedef Schemes16::const_iterator SchemeIterator16;
		typedef Schemes32::const_iterator SchemeIterator32;

	  private:
		URISchemeRegistry* parent;
		Schemes8 schemes8;
		Schemes16 schemes16;
		Schemes32 schemes32;
		platform::LockProvider<URISchemeRegistry>* lockProvider;

	  public:
		URISchemeRegistry(URISchemeRegistry* = NULL);
		URISchemeRegistry(const URISchemeRegistry&);
		~URISchemeRegistry();

		inline URISchemeRegistry* getParent() const {
			return parent;
		}

		inline void setParent(URISchemeRegistry* parent) {
			this->parent = parent;
		}

		inline platform::LockProvider<URISchemeRegistry>* getLockProvider() const {
			return lockProvider;
		}

		inline void setLockProvider(platform::LockProvider<URISchemeRegistry>* lockProvider) {
			this->lockProvider = lockProvider;
		}

		void getSchemes8(SchemeIterator8&, SchemeIterator8&) const;
		void getSchemes16(SchemeIterator16&, SchemeIterator16&) const;
		void getSchemes32(SchemeIterator32&, SchemeIterator32&) const;

		void setScheme8(const std::string&, URIScheme*);
		void setScheme16(const text::String16&, URIScheme*);
		void setScheme32(const text::String32&, URIScheme*);

		void setScheme(const std::string&, URIScheme*, text::Decoder16* = NULL, text::Decoder32* = NULL);
		void setScheme(const text::String16&, URIScheme*, text::Encoder16* = NULL, text::Transcoder1632* = NULL);
		void setScheme(const text::String32&, URIScheme*, text::Encoder32* = NULL, text::Transcoder3216* = NULL);

		void clearSchemes8();
		void clearSchemes16();
		void clearSchemes32();
		void clearSchemes();

		URIScheme* getScheme(const std::string&) const;
		URIScheme* getScheme(const text::String16&) const;
		URIScheme* getScheme(const text::String32&) const;

		URIScheme* resolveScheme(const std::string&) const;
		URIScheme* resolveScheme(const text::String16&) const;
		URIScheme* resolveScheme(const text::String32&) const;

		URI* newURI(const std::string&) const;
		URI* newURI(const text::String16&) const;
		URI* newURI(const text::String32&) const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_HPP */
