#ifndef REDSTRAIN_MOD_QU6NTUM_Q6ENGINE_HPP
#define REDSTRAIN_MOD_QU6NTUM_Q6ENGINE_HPP

#include <set>
#include <deque>
#include <redstrain/vfs/URI.hpp>

#include "api.hpp"

namespace redengine {
namespace qu6ntum {

	class ProviderSource;
	class ManifestSource;

	class REDSTRAIN_QU6NTUM_API Q6Engine {

	  public:
		enum EngineState {
			ES_SHUT_DOWN,
			ES_STARTING_UP,
			ES_RUNNING,
			ES_SHUTTING_DOWN
		};

		enum FileStateChange {
			FSC_CREATED,
			FSC_MODIFIED,
			FSC_REMOVED
		};

	  public:
		typedef std::set<ProviderSource*> ProviderSources;

	  private:
		vfs::URI* instanceHome;
		EngineState engineState;
		ProviderSources providerSources;

	  public:
		Q6Engine();
		Q6Engine(const Q6Engine&);

		inline EngineState getEngineState() const {
			return engineState;
		}

		inline const vfs::URI& getInstanceHome() const {
			return *instanceHome;
		}

		void setInstanceHome(const vfs::URI&);
		void setInstanceHome(const text::String16&);
		void setInstanceHome(const std::string&);

		bool startup();
		bool shutdown();

		void getProviderSources(ProviderSources&);
		bool addProviderSource(ProviderSource&);
		bool removeProviderSource(ProviderSource&);
		void clearProviderSources();

		void notifyProviderCodeChange(const vfs::URI&, const text::String16&, ManifestSource*, FileStateChange);
		void notifyProviderManifestChange(const vfs::URI&, ManifestSource&, FileStateChange);

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_Q6ENGINE_HPP */
