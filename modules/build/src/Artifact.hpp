#ifndef REDSTRAIN_MOD_BUILD_ARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_ARTIFACT_HPP

#include <ctime>
#include <string>
#include <redstrain/util/Appender.hpp>
#include <redstrain/io/InputStream.hpp>
#include <redstrain/io/OutputStream.hpp>
#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Transform;
	class BuildContext;

	class REDSTRAIN_BUILD_API Artifact : public util::ReferenceCounted {

	  public:
		class REDSTRAIN_BUILD_API Mood {

		  public:
			Mood();
			Mood(const Mood&);
			virtual ~Mood();

			virtual bool present(Artifact&) const = 0;
			virtual void modificationTimestamp(Artifact&, util::Appender<time_t>&) const = 0;
			virtual void virtualClock(Artifact&, util::Appender<time_t>&) const = 0;
			virtual void modified(Artifact&, BuildContext&) const = 0;
			virtual void remove(Artifact&) const = 0;
			virtual void require(Artifact&, BuildContext&) const = 0;
			virtual void perform(Transform&, BuildContext&, Artifact&) const = 0;

		};

		class REDSTRAIN_BUILD_API DefinitiveMood : public Mood {

		  public:
			DefinitiveMood();
			DefinitiveMood(const DefinitiveMood&);

			virtual bool present(Artifact&) const;
			virtual void modificationTimestamp(Artifact&, util::Appender<time_t>&) const;
			virtual void virtualClock(Artifact&, util::Appender<time_t>&) const;
			virtual void modified(Artifact&, BuildContext&) const;
			virtual void remove(Artifact&) const;
			virtual void require(Artifact&, BuildContext&) const;
			virtual void perform(Transform&, BuildContext&, Artifact&) const;

		  public:
			static DefinitiveMood instance;

		};

		class REDSTRAIN_BUILD_API PredictiveMood : public Mood {

		  public:
			PredictiveMood();
			PredictiveMood(const PredictiveMood&);

			virtual bool present(Artifact&) const;
			virtual void modificationTimestamp(Artifact&, util::Appender<time_t>&) const;
			virtual void virtualClock(Artifact&, util::Appender<time_t>&) const;
			virtual void modified(Artifact&, BuildContext&) const;
			virtual void remove(Artifact&) const;
			virtual void require(Artifact&, BuildContext&) const;
			virtual void perform(Transform&, BuildContext&, Artifact&) const;

		  public:
			static PredictiveMood instance;

		};

		enum REDSTRAIN_BUILD_API ReferenceDirection {
			FOR_INPUT,
			FOR_OUTPUT
		};

	  private:
		Transform* generatingTransform;

	  private:
		void require(const Mood&, BuildContext&);
		void rebuild(const Mood&, BuildContext&);

	  public:
		Artifact();
		Artifact(const Artifact&);
		virtual ~Artifact();

		inline Transform* getGeneratingTransform() const {
			return generatingTransform;
		}

		void setGeneratingTransform(Transform*);

		void require(BuildContext&);
		void wouldRequire(BuildContext&);

		virtual bool wouldBePresent() = 0;
		virtual void getPredictedModificationTimestamp(util::Appender<time_t>&) = 0;
		virtual void getPredictedVirtualClock(util::Appender<time_t>&) = 0;
		virtual void wouldModify(BuildContext&) = 0;
		virtual void wouldRemove() = 0;

		virtual bool isPresent() = 0;
		virtual void getModificationTimestamp(util::Appender<time_t>&) = 0;
		virtual void getVirtualClock(util::Appender<time_t>&) = 0;
		virtual void notifyModified(BuildContext&) = 0;
		virtual void remove() = 0;

		virtual io::InputStream<char>* getInputStream(BuildContext&) = 0;
		virtual io::OutputStream<char>* getOutputStream(BuildContext&) = 0;
		virtual void getFileReference(const std::string&, util::Appender<std::string>&, ReferenceDirection,
				BuildContext&) = 0;

		virtual bool isTransformable() = 0;
		virtual std::string getName() = 0;
		virtual std::string getLabel() = 0;

		virtual void dumpArtifact(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ARTIFACT_HPP */
