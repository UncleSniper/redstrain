#ifndef REDSTRAIN_MOD_BUILD_VALVEGROUP_HPP
#define REDSTRAIN_MOD_BUILD_VALVEGROUP_HPP

#include <set>
#include <redstrain/util/ReferenceCounted.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamtypes.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "api.hpp"

namespace redengine {
namespace build {

	class Valve;
	class StaticValve;
	class BuildContext;

	class REDSTRAIN_BUILD_API ValveGroup : public util::ReferenceCounted {

	  private:
		typedef std::set<Valve*> Valves;
		typedef std::set<StaticValve*> StaticValves;

	  public:
		typedef Valves::const_iterator ValveIterator;
		typedef StaticValves::const_iterator StaticValveIterator;

	  private:
		Valves valves;
		StaticValves defaults;

	  public:
		ValveGroup();
		ValveGroup(const ValveGroup&);
		~ValveGroup();

		bool addMember(Valve*);
		bool removeMember(Valve*);
		void clearMembers();
		void getMembers(ValveIterator&, ValveIterator&) const;

		bool addDefault(StaticValve*);
		bool removeDefault(StaticValve*);
		void clearDefaults();
		void getDefaults(StaticValveIterator&, StaticValveIterator&) const;

		bool forceDefaults(BuildContext&);

#ifdef TESTING_REDSTRAIN_BUILD_API
		void dumpValveGroup(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_VALVEGROUP_HPP */
