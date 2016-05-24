#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/platform/ObjectLocker.hpp>

#include "ProviderGroup.hpp"
#include "ProviderInstance.hpp"

using redengine::vfs::URI;
using redengine::util::IntegerBounds;
using redengine::platform::ObjectLocker;

namespace redengine {
namespace qu6ntum {

	static const ProviderInstanceID NO_IID = IntegerBounds<ProviderInstanceID>::MAX;

	ProviderGroup::ProviderGroup(Q6Engine& engine, const URI& codebaseURI)
			: engine(engine), codebaseURI(*codebaseURI.clone()), highestInstance(NO_IID) {}

	ProviderGroup::ProviderGroup(const ProviderGroup& group)
			: engine(group.engine), codebaseURI(*group.codebaseURI.clone()), highestInstance(NO_IID) {}

	ProviderGroup::~ProviderGroup() {
		delete &codebaseURI;
		Instances::const_iterator ibegin(instances.begin()), iend(instances.end());
		for(; ibegin != iend; ++ibegin)
			delete ibegin->second;
	}

	void ProviderGroup::getInstanceIDs(InstanceIDs& ids) const {
		ids.clear();
		ObjectLocker<Instances> lock(&instances);
		Instances::const_iterator ibegin(instances.begin()), iend(instances.end());
		for(; ibegin != iend; ++ibegin)
			ids.insert(ibegin->first);
		lock.release();
	}

	void ProviderGroup::addInstance(ProviderInstance&) {
		//TODO
	}

}}
