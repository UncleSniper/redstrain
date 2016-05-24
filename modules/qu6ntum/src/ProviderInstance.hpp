#ifndef REDSTRAIN_MOD_QU6NTUM_PROVIDERINSTANCE_HPP
#define REDSTRAIN_MOD_QU6NTUM_PROVIDERINSTANCE_HPP

#include <redstrain/platform/Library.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace qu6ntum {

	class ProviderGroup;

	class REDSTRAIN_QU6NTUM_API ProviderInstance {

	  public:
		enum InstanceState {
			IS_MAPPED,
			IS_INITIALIZING,
			IS_AVAILABLE,
			IS_UNINITIALIZING,
			IS_UNMAPPED
		};

	  private:
		ProviderGroup& group;
		const ProviderInstanceID instanceID;
		const std::string libraryPath;
		platform::Library library;
		volatile InstanceState instanceState;
		int instanceStateLock;

	  public:
		ProviderInstance(ProviderGroup&, ProviderInstanceID, const std::string&);
		ProviderInstance(const ProviderInstance&);
		~ProviderInstance();

		inline ProviderGroup& getProviderGroup() {
			return group;
		}

		inline const ProviderGroup& getProviderGroup() const {
			return group;
		}

		inline ProviderInstanceID getInstanceID() const {
			return instanceID;
		}

		inline const std::string& getLibraryPath() const {
			return libraryPath;
		}

		inline InstanceState getInstanceState() const {
			return instanceState;
		}

		void initialize();
		void decommission();

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_PROVIDERINSTANCE_HPP */
