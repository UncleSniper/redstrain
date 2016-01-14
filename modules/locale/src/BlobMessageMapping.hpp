#ifndef REDSTRAIN_MOD_LOCALE_BLOBMESSAGEMAPPING_HPP
#define REDSTRAIN_MOD_LOCALE_BLOBMESSAGEMAPPING_HPP

#include <map>
#include <list>
#include <string>
#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API BlobMessageMapping {

	  public:
		class REDSTRAIN_LOCALE_API Blob {

		  private:
			std::string language, country;
			const char* data;
			util::MemorySize size;

		  public:
			Blob(const std::string&, const std::string&, const char*, util::MemorySize);
			Blob(const Blob&);

			inline const std::string& getCountry() const {
				return country;
			}

			inline const std::string& getLanguage() const {
				return language;
			}

			inline const char* getData() const {
				return data;
			}

			inline util::MemorySize getSize() const {
				return size;
			}

		};

	  private:
		typedef std::list<Blob> Blobs;
		typedef std::map<std::string, std::map<std::string, const Blob*> > BlobMap;

	  public:
		typedef Blobs::const_iterator BlobIterator;

	  private:
		Blobs blobs;
		BlobMap* blobMap;

	  private:
		void buildMap();

	  public:
		BlobMessageMapping();
		BlobMessageMapping(const BlobMessageMapping&);
		~BlobMessageMapping();

		void addBlob(const Blob&);
		void addBlob(const std::string&, const std::string&, const char*, util::MemorySize);
		bool linkBlob(const std::string&, const std::string&, const std::string&, const std::string&);
		void clearBlobs();
		void getBlobs(BlobIterator&, BlobIterator&) const;
		const Blob* getBlob(const std::string&, const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_BLOBMESSAGEMAPPING_HPP */
