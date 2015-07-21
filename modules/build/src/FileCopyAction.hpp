#ifndef REDSTRAIN_MOD_BUILD_FILECOPYACTION_HPP
#define REDSTRAIN_MOD_BUILD_FILECOPYACTION_HPP

#include "Action.hpp"

namespace redengine {
namespace build {

	class FileArtifact;

	class REDSTRAIN_BUILD_API FileCopyAction : public Action {

	  private:
		FileArtifact& source;
		FileArtifact& destination;
		bool preservePermissions;

	  public:
		FileCopyAction(FileArtifact&, FileArtifact&, bool = false);
		FileCopyAction(const FileCopyAction&);
		virtual ~FileCopyAction();

		inline FileArtifact& getSource() {
			return source;
		}

		inline const FileArtifact& getSource() const {
			return source;
		}

		inline FileArtifact& getDestination() {
			return destination;
		}

		inline const FileArtifact& getDestination() const {
			return destination;
		}

		inline bool isPreservePermissions() const {
			return preservePermissions;
		}

		inline void setPreservePermissions(bool preservePermissions) {
			this->preservePermissions = preservePermissions;
		}

		virtual void perform(BuildContext&);
		virtual void wouldPerform(BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILECOPYACTION_HPP */
