#ifndef REDSTRAIN_MOD_VFS_HIERARCHICALURI_HPP
#define REDSTRAIN_MOD_VFS_HIERARCHICALURI_HPP

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

		virtual URI* makeNormalizedURI(const text::String16&) const;
		virtual bool isSchemeAndAuthorityEqual(const URI&) const;
		virtual URI* makeRelativizedURI(const URI&, const text::String16&) const;
		virtual URI* withFragment(const text::String16&) const;
		virtual URI* makeResolvedURI(const URI&, bool, const text::String16&) const;
		virtual URI* newURIOfThisType(int, const text::String16&, const text::String16&,
				const text::String16&, const text::String16&, uint16_t,
				const text::String16&, const text::String16&, const text::String16&) const = 0;

	  public:
		HierarchicalURI(int);
		HierarchicalURI(const text::String16&, const Decomposition&);
		HierarchicalURI(const HierarchicalURI&);

		virtual bool isOpaque() const;
		virtual bool isRemote() const;
		virtual URI* clone() const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_HIERARCHICALURI_HPP */
