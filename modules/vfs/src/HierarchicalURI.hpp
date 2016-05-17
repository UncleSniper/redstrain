#ifndef REDSTRAIN_MOD_VFS_HIERARCHICALURI_HPP
#define REDSTRAIN_MOD_VFS_HIERARCHICALURI_HPP

#include <redstrain/text/Formatter.hpp>
#include <redstrain/text/DefaultFormattingOptionStringEmitter.hpp>

#include "QueriedURI.hpp"
#include "AbstractURI.hpp"
#include "FragmentedURI.hpp"
#include "AuthoritativeURI.hpp"
#include "GenericPathedURI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API HierarchicalURI : public AbstractURI, public AuthoritativeURI,
			public GenericPathedURI, public QueriedURI, public FragmentedURI {

	  public:
		class REDSTRAIN_VFS_API Decomposition {

		  public:
			typedef text::String16::size_type Index;

		  public:
			static const int HAS_AUTHORITY = 001;
			static const int HAS_USERNAME  = 002;
			static const int HAS_PASSWORD  = 004;
			static const int HAS_PORT      = 010;
			static const int HAS_QUERY     = 020;
			static const int HAS_FRAGMENT  = 040;

		  public:
			int definedComponents;
			Index schemeEnd;
			Index authorityStart, authorityEnd;
			Index usernameStart, usernameEnd;
			Index passwordStart, passwordEnd;
			Index hostnameStart, hostnameEnd;
			Index portStart, portEnd;
			Index pathStart, pathEnd;
			Index queryStart, queryEnd;
			Index fragmentStart, fragmentEnd;

		  public:
			Decomposition();
			Decomposition(const Decomposition&);

		};

	  private:
		template<typename CharT, ComponentLevel>
		struct PortComponentType;

		template<typename CharT>
		struct PortComponentType<CharT, CL_RENDITION> {

			typedef std::basic_string<CharT> String;
			typedef typename String::size_type Length;
			typedef String Port;

			static Length getPortLength(const Port& port) {
				return port.length();
			}

			static void renderPort(const Port& port, String& rendition) {
				rendition += port;
			}

		};

		template<typename CharT>
		struct PortComponentType<CharT, CL_ORIGINAL> {

			typedef std::basic_string<CharT> String;
			typedef typename String::size_type Length;
			typedef uint16_t Port;

			static Length getPortLength(Port port) {
				Length length = static_cast<Length>(0u);
				for(; port; port /= static_cast<Port>(10u))
					++length;
				return length;
			}

			static void renderPort(Port port, String& rendition) {
				if(!port)
					return;
				rendition += text::DefaultIntegerFormatter<CharT>::template formatInteger<
					Port,
					text::DefaultFormattingRendition<CharT>
				>(port, text::FormattingOptions<CharT>(text::DefaultFormattingOptionStringEmitter<CharT>::instance));
			}

		};

	  public:
		template<typename CharT, ComponentLevel Level>
		class Components {

		  public:
			typedef std::basic_string<CharT> String;

		  public:
			int definedComponents;
			std::string scheme;
			String username, password, hostname;
			typename PortComponentType<CharT, Level>::Port port;
			String path, query, fragment;

		  public:
			Components(int definedComponents) : definedComponents(definedComponents) {}
			Components(const Components& components) : definedComponents(components.definedComponents),
					scheme(components.scheme), username(components.username), password(components.password),
					hostname(components.hostname), port(components.port), path(components.path),
					query(components.query), fragment(components.fragment) {}

			void toURIRendition(String& uri, void (*decodeScheme)(const std::string&, String&),
					Decomposition& decomposition) const {
				typedef typename String::size_type Index;
				Index length = scheme.empty() ? static_cast<Index>(0u)
						: static_cast<Index>(scheme.length()) + static_cast<Index>(1u);
				Index plength = static_cast<Index>(0u);
				if(definedComponents & Decomposition::HAS_AUTHORITY) {
					length += static_cast<Index>(2u) + hostname.length();
					if(definedComponents & Decomposition::HAS_USERNAME) {
						length += username.length() + static_cast<Index>(1u);
						if(definedComponents & Decomposition::HAS_PASSWORD)
							length += static_cast<Index>(1u) + password.length();
					}
					if(definedComponents & Decomposition::HAS_PORT) {
						plength = PortComponentType<CharT, Level>::getPortLength(port);
						if(plength)
							length += static_cast<Index>(1u) + plength;
					}
				}
				length += path.length();
				if(definedComponents & Decomposition::HAS_QUERY)
					length += static_cast<Index>(1u) + query.length();
				if(definedComponents & Decomposition::HAS_FRAGMENT)
					length += static_cast<Index>(1u) + fragment.length();
				uri.clear();
				uri.reserve(length);
				if(!scheme.empty()) {
					String dscheme;
					decodeScheme(scheme, dscheme);
					uri += dscheme;
					uri += static_cast<CharT>(':');
				}
				decomposition.schemeEnd = uri.length();
				if(definedComponents & Decomposition::HAS_AUTHORITY) {
					uri += static_cast<CharT>('/');
					uri += static_cast<CharT>('/');
					decomposition.authorityStart = uri.length();
					if(definedComponents & Decomposition::HAS_USERNAME) {
						decomposition.usernameStart = uri.length();
						uri += username;
						decomposition.usernameEnd = uri.length();
						if(definedComponents & Decomposition::HAS_PASSWORD) {
							uri += static_cast<CharT>(':');
							decomposition.passwordStart = uri.length();
							uri += password;
							decomposition.passwordEnd = uri.length();
						}
						uri += static_cast<CharT>('@');
					}
					decomposition.hostnameStart = uri.length();
					uri += hostname;
					decomposition.hostnameStart = uri.length();
					if(plength) {
						uri += static_cast<CharT>(':');
						decomposition.portStart = uri.length();
						PortComponentType<CharT, Level>::renderPort(port, uri);
						decomposition.portEnd = uri.length();
					}
					decomposition.authorityEnd = uri.length();
				}
				decomposition.pathStart = uri.length();
				uri += path;
				decomposition.pathEnd = uri.length();
				if(definedComponents & Decomposition::HAS_QUERY) {
					uri += static_cast<CharT>('?');
					decomposition.queryStart = uri.length();
					uri += query;
					decomposition.queryEnd = uri.length();
				}
				if(definedComponents & Decomposition::HAS_FRAGMENT) {
					uri += static_cast<CharT>('#');
					decomposition.fragmentStart = uri.length();
					uri += fragment;
					decomposition.fragmentEnd = uri.length();
				}
			}

		};

	  protected:
		virtual void authoritativeDecodeOriginal(const std::string&, std::string&) const;
		virtual void authoritativeDecodeOriginal(const std::string&, text::String16&) const;
		virtual void authoritativeDecodeOriginal(const std::string&, text::String32&) const;
		virtual void pathedDecodeOriginal(const std::string&, std::string&) const;
		virtual void pathedDecodeOriginal(const std::string&, text::String16&) const;
		virtual void pathedDecodeOriginal(const std::string&, text::String32&) const;
		virtual void queriedDecodeOriginal(const std::string&, std::string&) const;
		virtual void queriedDecodeOriginal(const std::string&, text::String16&) const;
		virtual void queriedDecodeOriginal(const std::string&, text::String32&) const;
		virtual void fragmentedDecodeOriginal(const std::string&, std::string&) const;
		virtual void fragmentedDecodeOriginal(const std::string&, text::String16&) const;
		virtual void fragmentedDecodeOriginal(const std::string&, text::String32&) const;
		virtual void hierarchicalDecodeOriginal(const std::string&, std::string&) const;
		virtual void hierarchicalDecodeOriginal(const std::string&, text::String16&) const;
		virtual void hierarchicalDecodeOriginal(const std::string&, text::String32&) const;
		virtual void hierarchicalEncodeOriginal(const std::string&, std::string&) const;
		virtual void hierarchicalEncodeOriginal(const text::String16&, std::string&) const;
		virtual void hierarchicalEncodeOriginal(const text::String32&, std::string&) const;
		virtual void hierarchicalDecodeRendition(const std::string&, text::String16&,
				bool (*escapePredicate)(text::Char16)) const;

		virtual URI* makeNormalizedURI(const text::String16&) const;
		virtual bool isSchemeAndAuthorityEqual(const URI&) const;
		virtual URI* makeRelativizedURI(const URI&, const text::String16&) const;
		virtual URI* withFragment(const text::String16&) const;
		virtual URI* makeResolvedURI(const URI&, bool, const text::String16&) const;
		virtual URI* newURIOfThisType(int, const text::String16&, const text::String16&,
				const text::String16&, const text::String16&, uint16_t,
				const text::String16&, const text::String16&, const text::String16&) const = 0;

		void setByRendition(const Components<text::Char16, CL_RENDITION>&);
		void setByOriginal(const Components<char, CL_ORIGINAL>&);
		void setByOriginal(const Components<text::Char16, CL_ORIGINAL>&);
		void setByOriginal(const Components<text::Char32, CL_ORIGINAL>&);

		void renderRendition(text::String16&, bool) const;
		void renderOctets(std::string&, bool) const;

	  public:
		HierarchicalURI(int);
		HierarchicalURI(const text::String16&, const Decomposition&);
		HierarchicalURI(const Components<text::Char16, CL_RENDITION>&);
		HierarchicalURI(const Components<char, CL_ORIGINAL>&);
		HierarchicalURI(const Components<text::Char16, CL_ORIGINAL>&);
		HierarchicalURI(const Components<text::Char32, CL_ORIGINAL>&);
		HierarchicalURI(const HierarchicalURI&);

		virtual std::string getRawSchemeSpecificPart8() const;
		virtual text::String16 getRawSchemeSpecificPart16() const;
		virtual text::String32 getRawSchemeSpecificPart32() const;
		virtual std::string getSchemeSpecificPart() const;
		virtual std::string getSchemeSpecificPart8() const;
		virtual text::String16 getSchemeSpecificPart16() const;
		virtual text::String32 getSchemeSpecificPart32() const;

		virtual std::string toString8() const;
		virtual text::String16 toString16() const;
		virtual text::String32 toString32() const;

		virtual bool isOpaque() const;
		virtual bool isRemote() const;
		virtual URI* clone() const;

		template<typename CharT>
		static bool userInfoEscapePredicate(CharT) {
			//TODO
			return false;
		}

		template<typename CharT>
		static bool hostnameEscapePredicate(CharT) {
			return false;
		}

		template<typename CharT>
		static bool pathEscapePredicate(CharT) {
			//TODO
			return false;
		}

		template<typename CharT>
		static bool queryEscapePredicate(CharT) {
			//TODO
			return false;
		}

		template<typename CharT>
		static bool fragmentEscapePredicate(CharT) {
			//TODO
			return false;
		}

	};

}}

#endif /* REDSTRAIN_MOD_VFS_HIERARCHICALURI_HPP */
