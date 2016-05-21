#ifndef REDSTRAIN_MOD_QU6NTUM_MANIFESTSOURCE_HPP
#define REDSTRAIN_MOD_QU6NTUM_MANIFESTSOURCE_HPP

#include "api.hpp"

namespace redengine {
namespace qu6ntum {

	class Manifest;

	class REDSTRAIN_QU6NTUM_API ManifestSource {

	  public:
		ManifestSource();
		ManifestSource(const ManifestSource&);
		virtual ~ManifestSource();

		virtual void readManifest(Manifest&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_MANIFESTSOURCE_HPP */
