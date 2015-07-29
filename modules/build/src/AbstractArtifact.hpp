#ifndef REDSTRAIN_MOD_BUILD_ABSTRACTARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_ABSTRACTARTIFACT_HPP

#include "Artifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API AbstractArtifact : public Artifact {

	  private:
		bool predictedPresent;
		time_t predictedTimestamp, predictedVirtualClock, virtualClock;

	  protected:
		void setPredictedModificationTimestamp(time_t);

	  public:
		AbstractArtifact();
		AbstractArtifact(const AbstractArtifact&);

		virtual bool wouldBePresent();
		virtual void getPredictedModificationTimestamp(util::Appender<time_t>&);
		virtual void getPredictedVirtualClock(util::Appender<time_t>&);
		virtual void wouldModify(BuildContext&);
		virtual void wouldRemove();

		virtual void getVirtualClock(util::Appender<time_t>&);
		virtual void notifyModified(BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ABSTRACTARTIFACT_HPP */
