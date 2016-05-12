#ifndef REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_HPP
#define REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_HPP

#include <map>
#include <redstrain/text/TextCodec.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/LockProvider.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "api.hpp"

namespace redengine {
namespace vfs {

	class URI;
	class URIScheme;

	class REDSTRAIN_VFS_API URISchemeRegistry {

	  public:
		enum CharClass {
			CC_RESERVED,
			CC_UNRESERVED,
			CC_ILLEGAL
		};

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

		template<typename CharT>
		static CharClass getCharClass(CharT c) {
			switch(c) {
				case static_cast<CharT>(';'):
				case static_cast<CharT>('/'):
				case static_cast<CharT>('?'):
				case static_cast<CharT>(':'):
				case static_cast<CharT>('@'):
				case static_cast<CharT>('&'):
				case static_cast<CharT>('='):
				case static_cast<CharT>('+'):
				case static_cast<CharT>('$'):
				case static_cast<CharT>(','):
					return CC_RESERVED;
				case static_cast<CharT>('-'):
				case static_cast<CharT>('_'):
				case static_cast<CharT>('.'):
				case static_cast<CharT>('!'):
				case static_cast<CharT>('~'):
				case static_cast<CharT>('*'):
				case static_cast<CharT>('\''):
				case static_cast<CharT>('('):
				case static_cast<CharT>(')'):
					return CC_UNRESERVED;
				default:
					if(c >= static_cast<CharT>('0') && c <= static_cast<CharT>('9'))
						return CC_UNRESERVED;
					if(c >= static_cast<CharT>('a') && c <= static_cast<CharT>('z'))
						return CC_UNRESERVED;
					if(c >= static_cast<CharT>('A') && c <= static_cast<CharT>('Z'))
						return CC_UNRESERVED;
					return CC_ILLEGAL;
			}
		}

	  private:
		static URI* makeRelativeURI(const std::string&);
		static URI* makeRelativeURI(const text::String16&);
		static URI* makeRelativeURI(const text::String32&);
		URI* makeAbsoluteURI(const std::string&, std::string::size_type) const;
		URI* makeAbsoluteURI(const text::String16&, text::String16::size_type) const;
		URI* makeAbsoluteURI(const text::String32&, text::String32::size_type) const;
		static void illegalURIHead(const std::string&, std::string::size_type);
		static void illegalURIHead(const text::String16&, text::String16::size_type);
		static void illegalURIHead(const text::String32&, text::String32::size_type);

		template<typename CharT>
		URI* parseURIHead(const std::basic_string<CharT>& specifier) const {
			typedef std::basic_string<CharT> StringT;
			typedef typename StringT::size_type StringSizeT;
			if(specifier.empty())
				return URISchemeRegistry::makeRelativeURI(specifier);
			StringSizeT offset = static_cast<StringSizeT>(0u);
			CharT c = specifier[offset];
			if(c == static_cast<CharT>('/'))
				return URISchemeRegistry::makeRelativeURI(specifier);
			StringSizeT length = specifier.length();
			// 2 = scheme, 1 = rel_segment
			int couldBe = (
				(c >= static_cast<CharT>('a') && c <= static_cast<CharT>('z'))
				|| (c >= static_cast<CharT>('A') && c <= static_cast<CharT>('Z'))
			) ? 3 : 1;
			for(; offset < length; ++offset) {
				c = specifier[offset];
				// scheme = alpha *( alpha | digit | "+" | "-" | "." )
				//                     U       U      R     U     U
				// rel_path = rel_segment [ abs_path ]
				// rel_segment = 1*( unreserved | escaped | ";" | "@" | "&" | "=" | "+" | "$" | ",")
				//                                           R     R     R     R     R     R     R
				switch(URISchemeRegistry::getCharClass<CharT>(c)) {
					case CC_RESERVED:
						if(c != static_cast<CharT>('+')) {
							if(!(couldBe &= ~2))
								URISchemeRegistry::illegalURIHead(specifier, offset);
						}
						switch(c) {
							case static_cast<CharT>('/'):
								if(couldBe & 1)
									return URISchemeRegistry::makeRelativeURI(specifier);
								URISchemeRegistry::illegalURIHead(specifier, offset);
							case static_cast<CharT>(':'):
								if(couldBe & 2)
									return makeAbsoluteURI(specifier, offset);
							case static_cast<CharT>('?'):
								if(!(couldBe &= ~1))
									URISchemeRegistry::illegalURIHead(specifier, offset);
								break;
							default:
								break;
						}
						break;
					case CC_UNRESERVED:
						switch(c) {
							case static_cast<CharT>('_'):
							case static_cast<CharT>('!'):
							case static_cast<CharT>('~'):
							case static_cast<CharT>('*'):
							case static_cast<CharT>('\''):
							case static_cast<CharT>('('):
							case static_cast<CharT>(')'):
								if(!(couldBe &= ~2))
									URISchemeRegistry::illegalURIHead(specifier, offset);
								break;
							default:
								break;
						}
						break;
					case CC_ILLEGAL:
						if(c == static_cast<CharT>('%')) {
							if(!(couldBe &= ~2))
								URISchemeRegistry::illegalURIHead(specifier, offset);
							break;
						}
					default:
						URISchemeRegistry::illegalURIHead(specifier, offset);
				}
			}
			if(!(couldBe & 1))
				URISchemeRegistry::illegalURIHead(specifier, offset);
			return URISchemeRegistry::makeRelativeURI(specifier);
		}

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URISCHEMEREGISTRY_HPP */
