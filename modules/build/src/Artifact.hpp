#ifndef REDSTRAIN_MOD_BUILD_ARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_ARTIFACT_HPP

#include <ctime>
#include <string>
#include <redstrain/util/ReferenceCounted.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamtypes.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

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
		virtual std::string getHumanReadableReference(bool) const = 0;

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpArtifact(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ARTIFACT_HPP */
