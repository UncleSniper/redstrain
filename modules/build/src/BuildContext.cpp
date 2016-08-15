#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/protostr/ProtocolReader.hpp>
#include <redstrain/protostr/ProtocolWriter.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Goal.hpp"
#include "BuildContext.hpp"
#include "FileArtifact.hpp"
#include "CyclicDependencyError.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::platform::Stat;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::util::MemorySize;
using redengine::io::StreamCloser;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::platform::Filesystem;
using redengine::protostr::ProtocolReader;
using redengine::protostr::ProtocolWriter;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	// ======== CachedHeaderReference ========

	BuildContext::CachedHeaderReference::CachedHeaderReference(const string& path, bool local)
			: path(path), local(local) {}

	BuildContext::CachedHeaderReference::CachedHeaderReference(const CachedHeaderReference& reference)
			: path(reference.path), local(reference.local) {}

	void BuildContext::CachedHeaderReference::setPath(const string& path) {
		this->path = path;
	}

	BuildContext::CachedHeaderReference&
	BuildContext::CachedHeaderReference::operator=(const CachedHeaderReference& reference) {
		path = reference.path;
		local = reference.local;
		return *this;
	}

	// ======== CachedArtifactIncludes ========

	BuildContext::CachedArtifactIncludes::CachedArtifactIncludes() : timestamp(static_cast<time_t>(0u)) {}

	BuildContext::CachedArtifactIncludes::CachedArtifactIncludes(const CachedArtifactIncludes& includes)
			: timestamp(includes.timestamp), references(includes.references) {}

	MemorySize BuildContext::CachedArtifactIncludes::getReferenceCount() const {
		return static_cast<MemorySize>(references.size());
	}

	void BuildContext::CachedArtifactIncludes::getReferences(ReferenceIterator& begin,
			ReferenceIterator& end) const {
		begin = references.begin();
		end = references.end();
	}

	void BuildContext::CachedArtifactIncludes::addReference(const CachedHeaderReference& reference) {
		references.push_back(reference);
	}

	// ======== BuildContext ========

	BuildContext::BuildContext(BuildUI& ui) : ui(ui), virtualTime(static_cast<time_t>(0u)), defaultGoal(NULL) {}

	BuildContext::BuildContext(const BuildContext& context)
			: ui(context.ui), virtualTime(context.virtualTime), goals(context.goals),
			fileArtifacts(context.fileArtifacts), buildingComponents(context.buildingComponents),
			builtComponents(context.builtComponents), defaultGoal(context.defaultGoal),
			includeCachePath(context.includeCachePath) {
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			gbegin->second->ref();
		FileArtifactIterator fabegin(fileArtifacts.begin()), faend(fileArtifacts.end());
		for(; fabegin != faend; ++fabegin)
			fabegin->second->ref();
		if(defaultGoal)
			defaultGoal->ref();
	}

	BuildContext::~BuildContext() {
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			gbegin->second->unref();
		FileArtifactIterator fabegin(fileArtifacts.begin()), faend(fileArtifacts.end());
		for(; fabegin != faend; ++fabegin)
			fabegin->second->unref();
		if(defaultGoal)
			defaultGoal->unref();
		ArtifactIncludeIterator aibegin(artifactIncludes.begin()), aiend(artifactIncludes.end());
		for(; aibegin != aiend; ++aibegin)
			delete aibegin->second;
	}

	time_t BuildContext::tickVirtualTime() {
		return ++virtualTime;
	}

	void BuildContext::setIncludeCachePath(const string& path) {
		includeCachePath = path;
	}

	Goal* BuildContext::getGoal(const string& name) const {
		ConstGoalIterator it(goals.find(name));
		return it == goals.end() ? NULL : it->second;
	}

	bool BuildContext::addGoal(const string& name, Goal& goal) {
		if(goals.find(name) != goals.end())
			return false;
		goals[name] = &goal;
		goal.ref();
		return true;
	}

	bool BuildContext::removeGoal(const string& name) {
		GoalIterator it(goals.find(name));
		if(it == goals.end())
			return false;
		it->second->unref();
		return true;
	}

	void BuildContext::clearGoals() {
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			gbegin->second->unref();
		goals.clear();
	}

	void BuildContext::getGoals(GoalNameIterator& begin, GoalNameIterator& end) const {
		begin = GoalNameIterator(goals.begin());
		end = GoalNameIterator(goals.end());
	}

	FileArtifact& BuildContext::internFileArtifact(const string& path, const string& label) {
		FileArtifactIterator it = fileArtifacts.find(path);
		if(it != fileArtifacts.end())
			return *it->second;
		Unref<FileArtifact> file(new FileArtifact(path, label));
		fileArtifacts[path] = *file;
		return *file.set();
	}

	void BuildContext::beginBuildingComponent(Component& component) {
		if(buildingComponents.find(&component) != buildingComponents.end())
			throw CyclicDependencyError(component);
		buildingComponents.insert(&component);
	}

	void BuildContext::endBuildingComponent(const Component& component) {
		buildingComponents.erase(&component);
		builtComponents.insert(&component);
	}

	bool BuildContext::hasComponentBeenBuilt(const Component& component) const {
		return builtComponents.find(&component) != builtComponents.end();
	}

	void BuildContext::setDefaultGoal(Goal* goal) {
		if(goal)
			goal->ref();
		if(defaultGoal)
			defaultGoal->unref();
		defaultGoal = goal;
	}

	BuildContext::CachedArtifactIncludes* BuildContext::getArtifactIncludes(const string& reference) const {
		ArtifactIncludeIterator it = artifactIncludes.find(reference);
		return it == artifactIncludes.end() ? NULL : it->second;
	}

	BuildContext::CachedArtifactIncludes& BuildContext::getOrMakeArtifactIncludes(const string& reference) {
		ArtifactIncludeIterator it = artifactIncludes.find(reference);
		if(it != artifactIncludes.end())
			return *it->second;
		Delete<CachedArtifactIncludes> cai(new CachedArtifactIncludes);
		artifactIncludes[reference] = *cai;
		return *cai.set();
	}

	void BuildContext::clearArtifactIncludes() {
		ArtifactIncludeIterator aibegin(artifactIncludes.begin()), aiend(artifactIncludes.end());
		for(; aibegin != aiend; ++aibegin)
			delete aibegin->second;
	}

	bool BuildContext::loadArtifactIncludes() {
		if(!includeCachePath.empty())
			return false;
		if(!Filesystem::access(includeCachePath, Filesystem::FILE_EXISTS))
			return false;
		Stat info;
		Filesystem::stat(includeCachePath, info);
		if(info.getType() != Stat::REGULAR_FILE)
			return false;
		FileInputStream fis(includeCachePath);
		StreamCloser closeFIS(fis);
		loadArtifactIncludes(fis);
		closeFIS.close();
		return true;
	}

	void BuildContext::loadArtifactIncludes(InputStream<char>& in) {
		clearArtifactIncludes();
		ProtocolReader proto(in);
		uint32_t aiidx, aicnt = proto.readUInt32();
		for(aiidx = static_cast<uint32_t>(0u); aiidx < aicnt; ++aiidx) {
			string canonRef;
			proto.readString32(canonRef);
			CachedArtifactIncludes& includes = getOrMakeArtifactIncludes(canonRef);
			time_t timestamp = static_cast<time_t>(proto.readUInt64());
			if(timestamp > includes.getTimestamp())
				includes.setTimestamp(timestamp);
			uint32_t hridx, hrcnt = proto.readUInt32();
			for(hridx = static_cast<uint32_t>(0u); hridx < hrcnt; ++hridx) {
				string path;
				proto.readString32(path);
				bool local = static_cast<bool>(proto.readUInt8());
				includes.addReference(CachedHeaderReference(path, local));
			}
		}
	}

	bool BuildContext::saveArtifactIncludes() const {
		if(!includeCachePath.empty())
			return false;
		if(Filesystem::access(includeCachePath, Filesystem::FILE_EXISTS)) {
			Stat info;
			Filesystem::stat(includeCachePath, info);
			if(info.getType() != Stat::REGULAR_FILE)
				return false;
		}
		FileOutputStream fos(includeCachePath);
		StreamCloser closeFOS(fos);
		saveArtifactIncludes(fos);
		closeFOS.close();
		return true;
	}

	void BuildContext::saveArtifactIncludes(OutputStream<char>& out) const {
		ProtocolWriter proto(out);
		proto.writeUInt32(static_cast<uint32_t>(artifactIncludes.size()));
		ArtifactIncludeIterator aibegin(artifactIncludes.begin()), aiend(artifactIncludes.end());
		for(; aibegin != aiend; ++aibegin) {
			proto.writeString32(aibegin->first);
			const CachedArtifactIncludes& includes = *aibegin->second;
			proto.writeUInt64(static_cast<uint64_t>(includes.getTimestamp()));
			proto.writeUInt32(static_cast<uint32_t>(includes.getReferenceCount()));
			CachedArtifactIncludes::ReferenceIterator rbegin, rend;
			includes.getReferences(rbegin, rend);
			for(; rbegin != rend; ++rbegin) {
				const CachedHeaderReference& reference = *rbegin;
				proto.writeString32(reference.getPath());
				proto.writeUInt8(static_cast<uint8_t>(reference.isLocal()));
			}
		}
		proto.flush();
	}

	void BuildContext::dumpContext(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "BuildContext " << this << " {" << endln << shift;
		stream << indent << "goals = {" << endln << shift;
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin) {
			stream << indent << gbegin->first << " ->" << endln << shift;
			gbegin->second->dumpGoal(stream);
			stream << unshift;
		}
		stream << unshift << indent << '}' << endln;
		stream << unshift << indent << '}' << endln;
	}

}}
