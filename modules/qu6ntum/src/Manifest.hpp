#ifndef REDSTRAIN_MOD_QU6NTUM_MANIFEST_HPP
#define REDSTRAIN_MOD_QU6NTUM_MANIFEST_HPP

#include <list>
#include <redstrain/text/types.hpp>

#include "api.hpp"

namespace redengine {
namespace qu6ntum {

	class REDSTRAIN_QU6NTUM_API Manifest {

	  private:
		typedef std::list<text::String16> NameList;

	  public:
		typedef NameList::const_iterator NameIterator;

	  private:
		NameList requiredProviders;

	  public:
		Manifest();
		Manifest(const Manifest&);

		void getRequiredProviders(NameIterator&, NameIterator&) const;
		void addRequiredProvider(const text::String16&);
		void clearRequiredProviders();

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_MANIFEST_HPP */
