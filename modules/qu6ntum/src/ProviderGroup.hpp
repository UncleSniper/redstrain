#ifndef REDSTRAIN_MOD_QU6NTUM_PROVIDERGROUP_HPP
#define REDSTRAIN_MOD_QU6NTUM_PROVIDERGROUP_HPP

#include <set>
#include <map>
#include <redstrain/vfs/URI.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace qu6ntum {

	class Q6Engine;
	class ProviderInstance;

	class REDSTRAIN_QU6NTUM_API ProviderGroup {

	  public:
		typedef std::set<ProviderInstanceID> InstanceIDs;

	  private:
		typedef std::map<ProviderInstanceID, ProviderInstance*> Instances;

	  private:
		Q6Engine& engine;
		vfs::URI& codebaseURI;
		ProviderInstanceID highestInstance;
		Instances instances;

	  public:
		ProviderGroup(Q6Engine&, const vfs::URI&);
		ProviderGroup(const ProviderGroup&);
		~ProviderGroup();

		inline Q6Engine& getEngine() {
			return engine;
		}

		inline const Q6Engine& getEngine() const {
			return engine;
		}

		inline const vfs::URI& getCodebaseURI() const {
			return codebaseURI;
		}

		void getInstanceIDs(InstanceIDs&) const;
		void addInstance(ProviderInstance&);

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_PROVIDERGROUP_HPP */
