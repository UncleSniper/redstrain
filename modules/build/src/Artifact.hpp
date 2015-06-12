#ifndef REDSTRAIN_MOD_BUILD_ARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_ARTIFACT_HPP

#include <time.h>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Artifact : public util::ReferenceCounted {

	  private:
		bool predictedPresent;
		time_t predictedTimestamp;

	  protected:
		void setPredictedModificationTimestamp(time_t);

	  public:
		Artifact();
		Artifact(const Artifact&);

		bool wouldBePresent();
		time_t getPredictedModificationTimestamp();
		void wouldModify();
		void wouldRemove();

		virtual bool isPresent() = 0;
		virtual time_t getModificationTimestamp() = 0;
		virtual void remove() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ARTIFACT_HPP */
