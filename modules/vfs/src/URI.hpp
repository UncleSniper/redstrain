#ifndef REDSTRAIN_MOD_VFS_URI_HPP
#define REDSTRAIN_MOD_VFS_URI_HPP

#include <stdint.h>
#include <redstrain/io/InputStream.hpp>
#include <redstrain/text/types.hpp>

#include "URIAcquisition.hpp"
#include "api.hpp"

namespace redengine {
namespace vfs {

	class VFS;

	class REDSTRAIN_VFS_API URI {

	  private:
		static const char HEX_DIGITS[16];

	  public:
		URI();
		URI(const URI&);
		virtual ~URI();

		virtual std::string getScheme8() const = 0;
		virtual text::String16 getScheme16() const = 0;
		virtual text::String32 getScheme32() const = 0;

		virtual std::string getRawSchemeSpecificPart8() const = 0;
		virtual text::String16 getRawSchemeSpecificPart16() const = 0;
		virtual text::String32 getRawSchemeSpecificPart32() const = 0;
		virtual std::string getSchemeSpecificPart() const = 0;
		virtual std::string getSchemeSpecificPart8() const = 0;
		virtual text::String16 getSchemeSpecificPart16() const = 0;
		virtual text::String32 getSchemeSpecificPart32() const = 0;

		virtual std::string getRawAuthority8() const = 0;
		virtual text::String16 getRawAuthority16() const = 0;
		virtual text::String32 getRawAuthority32() const = 0;
		virtual std::string getAuthority() const = 0;
		virtual std::string getAuthority8() const = 0;
		virtual text::String16 getAuthority16() const = 0;
		virtual text::String32 getAuthority32() const = 0;

		virtual std::string getRawAuthentication8() const = 0;
		virtual text::String16 getRawAuthentication16() const = 0;
		virtual text::String32 getRawAuthentication32() const = 0;
		virtual std::string getAuthentication() const = 0;
		virtual std::string getAuthentication8() const = 0;
		virtual text::String16 getAuthentication16() const = 0;
		virtual text::String32 getAuthentication32() const = 0;

		virtual std::string getRawUserName8() const = 0;
		virtual text::String16 getRawUserName16() const = 0;
		virtual text::String32 getRawUserName32() const = 0;
		virtual std::string getUserName() const = 0;
		virtual std::string getUserName8() const = 0;
		virtual text::String16 getUserName16() const = 0;
		virtual text::String32 getUserName32() const = 0;

		virtual std::string getRawPassword8() const = 0;
		virtual text::String16 getRawPassword16() const = 0;
		virtual text::String32 getRawPassword32() const = 0;
		virtual std::string getPassword() const = 0;
		virtual std::string getPassword8() const = 0;
		virtual text::String16 getPassword16() const = 0;
		virtual text::String32 getPassword32() const = 0;

		virtual std::string getRawHost8() const = 0;
		virtual text::String16 getRawHost16() const = 0;
		virtual text::String32 getRawHost32() const = 0;
		virtual std::string getHost() const = 0;
		virtual std::string getHost8() const = 0;
		virtual text::String16 getHost16() const = 0;
		virtual text::String32 getHost32() const = 0;

		virtual uint16_t getPort() const = 0;

		virtual std::string getRawPath8() const = 0;
		virtual text::String16 getRawPath16() const = 0;
		virtual text::String32 getRawPath32() const = 0;
		virtual std::string getPath() const = 0;
		virtual std::string getPath8() const = 0;
		virtual text::String16 getPath16() const = 0;
		virtual text::String32 getPath32() const = 0;

		virtual std::string getRawQuery8() const = 0;
		virtual text::String16 getRawQuery16() const = 0;
		virtual text::String32 getRawQuery32() const = 0;
		virtual std::string getQuery() const = 0;
		virtual std::string getQuery8() const = 0;
		virtual text::String16 getQuery16() const = 0;
		virtual text::String32 getQuery32() const = 0;

		virtual std::string getRawFragment8() const = 0;
		virtual text::String16 getRawFragment16() const = 0;
		virtual text::String32 getRawFragment32() const = 0;
		virtual std::string getFragment() const = 0;
		virtual std::string getFragment8() const = 0;
		virtual text::String16 getFragment16() const = 0;
		virtual text::String32 getFragment32() const = 0;

		virtual std::string toString() const = 0;
		virtual text::String16 toString16() const = 0;
		virtual text::String32 toString32() const = 0;

		virtual URI* normalize() const = 0;
		virtual URI* relativize(const URI&) const = 0;
		virtual URI* resolve(const URI&) const = 0;

		virtual URIAcquisition<io::InputStream<char> >* acquireResource() const = 0;
		virtual URIAcquisition<VFS>* acquireVFS() const = 0;

		template<typename OriginalCharT, typename RenditionCharT>
		static void escape(
			const std::basic_string<OriginalCharT>& original,
			std::basic_string<RenditionCharT>& rendition,
			void (*byteize)(const std::basic_string<OriginalCharT>&, std::string&),
			void (*render)(const std::string&, std::basic_string<RenditionCharT>&)
		) {
			std::string octets;
			byteize(original, octets);
			render(octets, rendition);
		}

		template<typename OriginalCharT, typename RenditionCharT>
		static void escape(
			const std::basic_string<OriginalCharT>& original,
			std::basic_string<RenditionCharT>& rendition,
			void (*byteize)(const std::basic_string<OriginalCharT>&, std::string&),
			util::MemorySize (*renderChar)(const char*, util::MemorySize, RenditionCharT&),
			bool (*escapePredicate)(RenditionCharT)
		) {
			std::string octets;
			byteize(original, octets);
			util::MemorySize ocount = static_cast<util::MemorySize>(octets.length());
			util::MemorySize consumed = static_cast<util::MemorySize>(0u), c, u;
			const char* obuffer = octets.data();
			RenditionCharT rc, ebuffer[3];
			*ebuffer = static_cast<RenditionCharT>('%');
			while(consumed < ocount) {
				c = renderChar(obuffer, ocount - consumed, rc);
				if(escapePredicate(rc)) {
					for(u = static_cast<util::MemorySize>(0u); u < c; ++u) {
						unsigned ocode = static_cast<unsigned>(static_cast<unsigned char>(obuffer[consumed + u]));
						ebuffer[u + static_cast<util::MemorySize>(1u)] = static_cast<RenditionCharT>(
								static_cast<unsigned char>(URI::HEX_DIGITS[ocode >> 4]));
						ebuffer[u + static_cast<util::MemorySize>(2u)] = static_cast<RenditionCharT>(
								static_cast<unsigned char>(URI::HEX_DIGITS[ocode & 0x0Fu]));
						rendition.append(ebuffer,
								static_cast<typename std::basic_string<RenditionCharT>::size_type>(3u));
					}
				}
				else
					rendition += rc;
				consumed += c;
			}
		}

		/*TODO
		template<typename RenditionCharT, typename OriginalCharT>
		static void unescape(
			const std::basic_string<RenditionCharT>& rendition,
			std::basic_string<OriginalCharT>& original
		)
		*/

		static void byteizeID(const std::string&, std::string&);
		static void byteizeUTF8(const text::String16&, std::string&);
		static void byteizeUTF16BE(const text::String32&, std::string&);
		static void byteizeUTF16LE(const text::String32&, std::string&);
		static void byteizeUTF16UTF8(const text::String32&, std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URI_HPP */
