#ifndef REDSTRAIN_MOD_VFS_HIERARCHICALURISCHEME_HPP
#define REDSTRAIN_MOD_VFS_HIERARCHICALURISCHEME_HPP

#include "URIScheme.hpp"
#include "HierarchicalURI.hpp"
#include "URISchemeRegistry.hpp"
#include "HierarchicalURISyntaxError.hpp"
#include "MissingSchemeSpecificURIPartError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API HierarchicalURIScheme : public URIScheme {

	  private:
		template<typename CharT>
		struct ParseState {

			typedef std::basic_string<CharT> String;
			typedef typename String::size_type Index;

			const String& uri;
			Index index;
			const Index length;

			ParseState(const String& uri, Index index) : uri(uri), index(index), length(uri.length()) {}
			ParseState(const ParseState& state) : uri(state.uri), index(state.index), length(state.length) {}

			inline void bad() const {
				throw HierarchicalURISyntaxError(uri, index);
			}

		};

	  private:
		template<typename CharT>
		static bool parseUserInfo(ParseState<CharT>& state, HierarchicalURI::Decomposition& decomposition) {
			typedef typename std::basic_string<CharT>::size_type Index;
			bool inPassword = false;
			Index index = state.index;
			decomposition.usernameStart = index;
			for(; index < state.length; ++index) {
				CharT c = state.uri[index];
				switch(c) {
					case static_cast<CharT>(':'):
						if(!inPassword) {
							inPassword = true;
							decomposition.usernameEnd = index;
							decomposition.passwordStart = index + static_cast<Index>(1u);
						}
					case static_cast<CharT>('-'):
					case static_cast<CharT>('_'):
					case static_cast<CharT>('.'):
					case static_cast<CharT>('!'):
					case static_cast<CharT>('~'):
					case static_cast<CharT>('*'):
					case static_cast<CharT>('\''):
					case static_cast<CharT>('('):
					case static_cast<CharT>(')'):
					case static_cast<CharT>(';'):
					case static_cast<CharT>('&'):
					case static_cast<CharT>('='):
					case static_cast<CharT>('+'):
					case static_cast<CharT>('$'):
					case static_cast<CharT>(','):
					case static_cast<CharT>('%'):
						break;
					case static_cast<CharT>('@'):
						if(inPassword) {
							decomposition.passwordEnd = index;
							decomposition.definedComponents |= HierarchicalURI::Decomposition::HAS_PASSWORD;
						}
						else
							decomposition.usernameEnd = index;
						decomposition.definedComponents |= HierarchicalURI::Decomposition::HAS_USERNAME;
						state.index = index + static_cast<Index>(1u);
						return true;
					default:
						if(c >= static_cast<CharT>('a') && c <= static_cast<CharT>('z'))
							break;
						if(c >= static_cast<CharT>('A') && c <= static_cast<CharT>('Z'))
							break;
						if(c >= static_cast<CharT>('0') && c <= static_cast<CharT>('9'))
							break;
						return false;
				}
			}
			return false;
		}

		template<typename CharT>
		static void parseHostPort(ParseState<CharT>& state, HierarchicalURI::Decomposition& decomposition) {
			typedef typename std::basic_string<CharT>::size_type Index;
			Index index = state.index;
			decomposition.hostnameStart = index;
			enum {
				IS_EMPTY,
				IS_TOP_START,
				IS_TOP_MID,
				IS_TOP_END,
				IS_DOMAIN_START,
				IS_DOMAIN_MID,
				IS_DOMAIN_END,
				IS_IP4,
				IS_ERROR
			} innerState = IS_EMPTY;
			enum {
				OS_NONE,
				OS_TOP,
				OS_DOMAIN,
				OS_IP4_1,
				OS_IP4_2,
				OS_IP4_3,
				OS_IP4_4
			} outerState = OS_NONE;
			Index lastValidEnd = index, errorIndex = static_cast<Index>(0u);
			for(; index < state.length; ++index) {
				CharT c = state.uri[state.index];
				if(c == static_cast<CharT>('.')) {
					switch(innerState) {
						case IS_TOP_START:
						case IS_TOP_END:
							// top/domain
							switch(outerState) {
								case OS_NONE:
								case OS_DOMAIN:
									outerState = OS_TOP;
								case OS_TOP:
									innerState = IS_EMPTY;
									lastValidEnd = index;
									break;
								case OS_IP4_1:
								case OS_IP4_2:
								case OS_IP4_3:
								case OS_IP4_4:
									outerState = OS_TOP;
									innerState = IS_EMPTY;
									lastValidEnd = index;
									break;
								default:
									innerState = IS_ERROR;
									errorIndex = index;
									break;
							}
							break;
						case IS_DOMAIN_START:
						case IS_DOMAIN_END:
							// domain
							switch(outerState) {
								case OS_NONE:
								case OS_DOMAIN:
								case OS_TOP:
								case OS_IP4_1:
								case OS_IP4_2:
								case OS_IP4_3:
								case OS_IP4_4:
									outerState = OS_DOMAIN;
									innerState = IS_EMPTY;
									break;
								default:
									innerState = IS_ERROR;
									errorIndex = index;
									break;
							}
							break;
						case IS_IP4:
							// ip4/domain
							switch(outerState) {
								case OS_NONE:
									outerState = OS_IP4_1;
									innerState = IS_EMPTY;
									break;
								case OS_TOP:
									outerState = OS_DOMAIN;
								case OS_DOMAIN:
									innerState = IS_EMPTY;
									break;
								case OS_IP4_1:
									outerState = OS_IP4_2;
									innerState = IS_EMPTY;
									break;
								case OS_IP4_2:
									outerState = OS_IP4_3;
									innerState = IS_EMPTY;
									break;
								case OS_IP4_3:
									outerState = OS_IP4_4;
									innerState = IS_EMPTY;
									lastValidEnd = index;
									break;
								case OS_IP4_4:
									outerState = OS_DOMAIN;
									innerState = IS_EMPTY;
									break;
								default:
									innerState = IS_ERROR;
									errorIndex = index;
									break;
							}
							break;
						case IS_TOP_MID:
						case IS_DOMAIN_MID:
							innerState = IS_ERROR;
							errorIndex = index;
							break;
						case IS_EMPTY:
						default:
							innerState = IS_ERROR;
							errorIndex = index;
						case IS_ERROR:
							break;
					}
				}
				else if(c == static_cast<CharT>('-')) {
					switch(innerState) {
						case IS_EMPTY:
							innerState = IS_ERROR;
							errorIndex = index;
							break;
						case IS_TOP_START:
						case IS_TOP_END:
							innerState = IS_TOP_MID;
						case IS_TOP_MID:
							break;
						case IS_DOMAIN_START:
						case IS_DOMAIN_END:
						case IS_IP4:
							innerState = IS_DOMAIN_MID;
						case IS_DOMAIN_MID:
							break;
						default:
							innerState = IS_ERROR;
							errorIndex = index;
						case IS_ERROR:
							break;
					}
				}
				else if(
					(c >= static_cast<CharT>('a') && c <= static_cast<CharT>('z'))
					|| (c >= static_cast<CharT>('A') && c <= static_cast<CharT>('Z'))
				) {
					switch(innerState) {
						case IS_EMPTY:
							innerState = IS_TOP_START;
						case IS_TOP_START:
							break;
						case IS_TOP_MID:
							innerState = IS_TOP_END;
						case IS_TOP_END:
							break;
						case IS_DOMAIN_MID:
							innerState = IS_DOMAIN_END;
						case IS_DOMAIN_START:
						case IS_DOMAIN_END:
							break;
						case IS_IP4:
							innerState = IS_DOMAIN_START;
							break;
						default:
							innerState = IS_ERROR;
							errorIndex = index;
						case IS_ERROR:
							break;
					}
				}
				else if(c >= static_cast<CharT>('0') && c <= static_cast<CharT>('9')) {
					switch(innerState) {
						case IS_EMPTY:
							innerState = IS_IP4;
						case IS_TOP_START:
							innerState = IS_DOMAIN_START;
						case IS_TOP_MID:
							innerState = IS_TOP_END;
						case IS_TOP_END:
							break;
						case IS_DOMAIN_MID:
							innerState = IS_DOMAIN_END;
						case IS_DOMAIN_START:
						case IS_DOMAIN_END:
							break;
						case IS_IP4:
							break;
						default:
							innerState = IS_ERROR;
							errorIndex = index;
						case IS_ERROR:
							break;
					}
				}
				else {
					innerState = IS_ERROR;
					errorIndex = index;
				}
				if(innerState == IS_ERROR)
					break;
			}
			switch(innerState) {
				case IS_TOP_START:
				case IS_TOP_END:
					// top/domain
					switch(outerState) {
						case OS_NONE:
						case OS_DOMAIN:
							outerState = OS_TOP;
						case OS_TOP:
							innerState = IS_EMPTY;
							lastValidEnd = index;
							break;
						case OS_IP4_1:
						case OS_IP4_2:
						case OS_IP4_3:
						case OS_IP4_4:
							outerState = OS_TOP;
							innerState = IS_EMPTY;
							lastValidEnd = index;
							break;
						default:
							innerState = IS_ERROR;
							errorIndex = index;
							break;
					}
					break;
				case IS_DOMAIN_START:
				case IS_DOMAIN_END:
					// domain
					switch(outerState) {
						case OS_NONE:
						case OS_DOMAIN:
						case OS_TOP:
						case OS_IP4_1:
						case OS_IP4_2:
						case OS_IP4_3:
						case OS_IP4_4:
							outerState = OS_DOMAIN;
							innerState = IS_EMPTY;
							break;
						default:
							innerState = IS_ERROR;
							errorIndex = index;
							break;
					}
					break;
				case IS_IP4:
					// ip4/domain
					switch(outerState) {
						case OS_NONE:
							outerState = OS_IP4_1;
							innerState = IS_EMPTY;
							break;
						case OS_TOP:
							outerState = OS_DOMAIN;
						case OS_DOMAIN:
							innerState = IS_EMPTY;
							break;
						case OS_IP4_1:
							outerState = OS_IP4_2;
							innerState = IS_EMPTY;
							break;
						case OS_IP4_2:
							outerState = OS_IP4_3;
							innerState = IS_EMPTY;
							break;
						case OS_IP4_3:
							outerState = OS_IP4_4;
							innerState = IS_EMPTY;
							lastValidEnd = index;
							break;
						case OS_IP4_4:
							outerState = OS_DOMAIN;
							innerState = IS_EMPTY;
							break;
						default:
							innerState = IS_ERROR;
							errorIndex = index;
							break;
					}
					break;
				case IS_TOP_MID:
				case IS_DOMAIN_MID:
					innerState = IS_ERROR;
					errorIndex = index;
					break;
				case IS_EMPTY:
				default:
					innerState = IS_ERROR;
					errorIndex = index;
				case IS_ERROR:
					break;
			}
			if(lastValidEnd > state.index)
				decomposition.hostnameEnd = lastValidEnd;
			else {
				state.index = innerState == IS_ERROR ? errorIndex : index;
				state.bad();
			}
			index = lastValidEnd;
			if(index < state.length && state.uri[index] == static_cast<CharT>(':')) {
				decomposition.definedComponents |= HierarchicalURI::Decomposition::HAS_PORT;
				decomposition.portStart = ++index;
				while(index < state.length) {
					CharT c = state.uri[index];
					if(c < static_cast<CharT>('0') || c > static_cast<CharT>('9'))
						break;
					++index;
				}
				decomposition.portEnd = index;
			}
			state.index = index;
		}

		template<typename CharT>
		static void parseAuthority(ParseState<CharT>& state, HierarchicalURI::Decomposition& decomposition) {
			typedef typename std::basic_string<CharT>::size_type Index;
			decomposition.definedComponents |= HierarchicalURI::Decomposition::HAS_AUTHORITY;
			state.index += static_cast<Index>(2u);
			decomposition.authorityStart = state.index;
			if(state.index >= state.length) {
				decomposition.authorityEnd = decomposition.hostnameStart = decomposition.hostnameEnd = state.index;
				return;
			}
			bool hasUserInfo = HierarchicalURIScheme::parseUserInfo<CharT>(state, decomposition);
			if(hasUserInfo)
				HierarchicalURIScheme::parseHostPort<CharT>(state, decomposition);
			else {
				switch(state.uri[state.index]) {
					case static_cast<CharT>('/'):
					case static_cast<CharT>('?'):
					case static_cast<CharT>('#'):
						decomposition.hostnameStart = decomposition.hostnameEnd = state.index;
						break;
					default:
						HierarchicalURIScheme::parseHostPort<CharT>(state, decomposition);
						break;
				}
			}
			decomposition.authorityEnd = state.index;
		}

		template<typename CharT>
		static void parseAbsPath(ParseState<CharT>& state, HierarchicalURI::Decomposition& decomposition) {
			decomposition.pathStart = state.index;
			for(; state.index < state.length; ++state.index) {
				CharT c = state.uri[state.index];
				switch(URISchemeRegistry::getCharClass<CharT>(c)) {
					case URISchemeRegistry::CC_RESERVED:
						if(c == static_cast<CharT>('?')) {
							decomposition.pathEnd = state.index;
							return;
						}
					case URISchemeRegistry::CC_UNRESERVED:
						break;
					case URISchemeRegistry::CC_ILLEGAL:
						if(c != static_cast<CharT>('%')) {
							decomposition.pathEnd = state.index;
							return;
						}
						break;
					default:
						decomposition.pathEnd = state.index;
						return;
				}
			}
			decomposition.pathEnd = state.index;
		}

	  protected:
		virtual URI* newURI(const text::String16&, const HierarchicalURI::Decomposition&) const = 0;

	  public:
		HierarchicalURIScheme();
		HierarchicalURIScheme(const URIScheme&);

		virtual URI* newURI(const std::string&, std::string::size_type);
		virtual URI* newURI(const text::String16&, text::String16::size_type);
		virtual URI* newURI(const text::String32&, text::String32::size_type);

		template<typename CharT>
		static bool isURIC(CharT c) {
			switch(URISchemeRegistry::getCharClass<CharT>(c)) {
				case URISchemeRegistry::CC_RESERVED:
				case URISchemeRegistry::CC_UNRESERVED:
					return true;
				case URISchemeRegistry::CC_ILLEGAL:
				default:
					return c == static_cast<CharT>('%');
			}
		}

		template<typename CharT>
		static void parseHierarchicalURI(const std::basic_string<CharT>& uri,
				typename std::basic_string<CharT>::size_type sspIndex,
				HierarchicalURI::Decomposition& decomposition, bool allowRelativePath) {
			typedef typename std::basic_string<CharT>::size_type Index;
			decomposition.schemeEnd = sspIndex ? sspIndex - static_cast<Index>(1u) : sspIndex;
			ParseState<CharT> state(uri, sspIndex);
			if(state.index >= state.length)
				throw MissingSchemeSpecificURIPartError(uri);
			if(uri[state.index] != static_cast<CharT>('/')) {
				if(!allowRelativePath)
					state.bad();
				HierarchicalURIScheme::parseAbsPath<CharT>(state, decomposition);
			}
			else {
				Index nextIndex = state.index + static_cast<Index>(1u);
				if(nextIndex >= state.length) {
					decomposition.pathStart = state.index;
					decomposition.pathEnd = nextIndex;
					return;
				}
				if(uri[nextIndex] == static_cast<CharT>('/')) {
					HierarchicalURIScheme::parseAuthority<CharT>(state, decomposition);
					if(state.index < state.length && uri[state.index] == static_cast<CharT>('/'))
						HierarchicalURIScheme::parseAbsPath<CharT>(state, decomposition);
					else
						decomposition.pathStart = decomposition.pathEnd = state.index;
				}
				else
					HierarchicalURIScheme::parseAbsPath<CharT>(state, decomposition);
			}
			if(state.index < state.length && uri[state.index] == static_cast<CharT>('?')) {
				decomposition.definedComponents |= HierarchicalURI::Decomposition::HAS_QUERY;
				decomposition.queryStart = ++state.index;
				while(state.index < state.length && HierarchicalURIScheme::isURIC<CharT>(uri[state.index]))
					++state.index;
				decomposition.queryEnd = state.index;
			}
			if(state.index < state.length && uri[state.index] == static_cast<CharT>('#')) {
				decomposition.definedComponents |= HierarchicalURI::Decomposition::HAS_FRAGMENT;
				decomposition.fragmentStart = ++state.index;
				while(state.index < state.length && HierarchicalURIScheme::isURIC<CharT>(uri[state.index]))
					++state.index;
				decomposition.fragmentEnd = state.index;
			}
			if(state.index < state.length)
				state.bad();
		}

	};

}}

#endif /* REDSTRAIN_MOD_VFS_HIERARCHICALURISCHEME_HPP */
