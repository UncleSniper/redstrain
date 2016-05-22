#ifndef REDSTRAIN_MOD_QU6NTUM_URIDIRECTORYPROVIDERSOURCE_HPP
#define REDSTRAIN_MOD_QU6NTUM_URIDIRECTORYPROVIDERSOURCE_HPP

#include <redstrain/util/Appender.hpp>

#include "PollingProviderSource.hpp"

namespace redengine {
namespace qu6ntum {

	class REDSTRAIN_QU6NTUM_API URIDirectoryProviderSource : public PollingProviderSource {

	  private:
		class REDSTRAIN_QU6NTUM_API EntrySink : public util::Appender<text::String16> {

		  private:
			URIDirectoryProviderSource& providerSource;

		  public:
			EntrySink(URIDirectoryProviderSource&);
			EntrySink(const EntrySink&);

			virtual void append(const text::String16&);

		};

	  private:
		vfs::URI *directoryURI, *resolvedURI;
		vfs::URIAcquisition<vfs::VFS>* acquisition;
		text::String16 providerSuffix;

	  protected:
		virtual void pollProviders();
		virtual vfs::URI* getProviderURIByName(const text::String16&);

	  public:
		URIDirectoryProviderSource(const vfs::URI&);
		URIDirectoryProviderSource(const URIDirectoryProviderSource&);
		virtual ~URIDirectoryProviderSource();

		virtual void startProviderSource(Q6Engine&);
		virtual void stopProviderSource(Q6Engine&);

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_URIDIRECTORYPROVIDERSOURCE_HPP */
