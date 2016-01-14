#include <redstrain/platform/ObjectLocker.hpp>

#include "BlobMessageMapping.hpp"

using std::map;
using std::string;
using redengine::util::MemorySize;
using redengine::platform::ObjectLocker;

namespace redengine {
namespace locale {

	// ======== Blob ========

	BlobMessageMapping::Blob::Blob(const string& language, const string& country, const char* data, MemorySize size)
			: language(language), country(country), data(data), size(size) {}

	BlobMessageMapping::Blob::Blob(const Blob& blob)
			: language(blob.language), country(blob.country), data(blob.data), size(blob.size) {}

	// ======== BlobMessageMapping ========

	BlobMessageMapping::BlobMessageMapping() : blobMap(NULL) {}

	BlobMessageMapping::BlobMessageMapping(const BlobMessageMapping& mapping) : blobs(mapping.blobs), blobMap(NULL) {}

	BlobMessageMapping::~BlobMessageMapping() {
		if(blobMap)
			delete blobMap;
	}

	void BlobMessageMapping::buildMap() {
		blobMap = new BlobMap;
		BlobIterator begin(blobs.begin()), end(blobs.end());
		for(; begin != end; ++begin) {
			BlobMap::iterator it0(blobMap->find(begin->getLanguage()));
			if(it0 == blobMap->end())
				((*blobMap)[begin->getLanguage()] = map<string, const Blob*>())[begin->getCountry()] = &*begin;
			else
				it0->second[begin->getCountry()] = &*begin;
		}
	}

	void BlobMessageMapping::addBlob(const Blob& blob) {
		blobs.push_back(blob);
		if(blobMap) {
			delete blobMap;
			blobMap = NULL;
		}
	}

	void BlobMessageMapping::addBlob(const string& language, const string& country,
			const char* data, MemorySize size) {
		addBlob(Blob(language, country, data, size));
	}

	bool BlobMessageMapping::linkBlob(const string& oldLanguage, const string& oldCountry,
			const string& newLanguage, const string& newCountry) {
		BlobIterator begin(blobs.begin()), end(blobs.end());
		for(; begin != end; ++begin) {
			if(begin->getLanguage() == oldLanguage && begin->getCountry() == oldCountry) {
				addBlob(newLanguage, newCountry, begin->getData(), begin->getSize());
				return true;
			}
		}
		return false;
	}

	void BlobMessageMapping::clearBlobs() {
		blobs.clear();
		if(blobMap) {
			delete blobMap;
			blobMap = NULL;
		}
	}

	void BlobMessageMapping::getBlobs(BlobIterator& begin, BlobIterator& end) const {
		begin = blobs.begin();
		end = blobs.end();
	}

	const BlobMessageMapping::Blob* BlobMessageMapping::getBlob(const string& language, const string& country) {
		ObjectLocker<BlobMessageMapping> lock(this);
		if(!blobMap)
			buildMap();
		lock.release();
		BlobMap::const_iterator it0(blobMap->find(language));
		map<string, const Blob*>::const_iterator it1;
		if(it0 == blobMap->end()) {
			it0 = blobMap->find("");
			if(it0 == blobMap->end())
				return NULL;
		}
		it1 = it0->second.find(country);
		if(it1 != it0->second.end())
			return it1->second;
		it1 = it0->second.find("");
		if(it1 != it0->second.end())
			return it1->second;
		return it0->second.empty() ? NULL : it0->second.begin()->second;
	}

}}
