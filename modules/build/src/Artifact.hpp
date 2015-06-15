#ifndef REDSTRAIN_MOD_BUILD_ARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_ARTIFACT_HPP

#include <time.h>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Artifact : public util::ReferenceCounted {

	  public:
		class REDSTRAIN_BUILD_API Mood {

		  public:
			Mood();
			Mood(const Mood&);
			virtual ~Mood();

			virtual bool present(Artifact&) const = 0;
			virtual time_t modificationTimestamp(Artifact&) const = 0;

		};

		class REDSTRAIN_BUILD_API DefinitiveMood : public Mood {

		  public:
			DefinitiveMood();
			DefinitiveMood(const DefinitiveMood&);

			virtual bool present(Artifact&) const;
			virtual time_t modificationTimestamp(Artifact&) const;

		};

		class REDSTRAIN_BUILD_API PredictiveMood : public Mood {

		  public:
			PredictiveMood();
			PredictiveMood(const PredictiveMood&);

			virtual bool present(Artifact&) const;
			virtual time_t modificationTimestamp(Artifact&) const;

		};

	  private:
		bool predictedPresent;
		time_t predictedTimestamp;

	  protected:
		void setPredictedModificationTimestamp(time_t);

	  public:
		static DefinitiveMood DEFINITIVE_MOOD;
		static PredictiveMood PREDICTIVE_MOOD;

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
