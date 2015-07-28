#ifndef REDSTRAIN_MOD_BUILD_FILEGENERATINGACTION_HPP
#define REDSTRAIN_MOD_BUILD_FILEGENERATINGACTION_HPP

#include <set>

#include "Action.hpp"

namespace redengine {
namespace build {

	class FileArtifact;

	class REDSTRAIN_BUILD_API FileGeneratingAction : public virtual Action {

	  private:
		typedef std::set<FileArtifact*> FileArtifacts;

	  public:
		typedef FileArtifacts::const_iterator FileArtifactIterator;

	  private:
		FileArtifacts intermediates;

	  protected:
		void addIntermediateDirectoriesFor(const std::string&, const std::string&, BuildContext&);
		void addIntermediateDirectoriesFor(const FileArtifact&, BuildContext&);
		void createIntermediateDirectories() const;
		void wouldCreateIntermediateDirectories() const;

		void dumpFileGeneratingActionAspects(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	  public:
		FileGeneratingAction();
		FileGeneratingAction(const FileGeneratingAction&);
		virtual ~FileGeneratingAction();

		bool addIntermediateDirectory(FileArtifact*);
		bool removeIntermediateDirectory(FileArtifact*);
		void clearIntermediateDirectories();
		void getIntermediateDirectories(FileArtifactIterator&, FileArtifactIterator&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILEGENERATINGACTION_HPP */
