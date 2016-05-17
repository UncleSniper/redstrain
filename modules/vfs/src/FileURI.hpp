#ifndef REDSTRAIN_MOD_VFS_FILEURI_HPP
#define REDSTRAIN_MOD_VFS_FILEURI_HPP

#include "URIAcquirer.hpp"
#include "HierarchicalURI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API FileURI : public HierarchicalURI {

	  private:
		class REDSTRAIN_VFS_API AcquisitionBase {

		  protected:
			const FileURI& uri;
			VFS& vfs;

		  protected:
			AcquisitionBase(const FileURI&, VFS&);
			AcquisitionBase(const AcquisitionBase&);
			virtual ~AcquisitionBase();

		};

		class REDSTRAIN_VFS_API StreamAcquisition
				: public AcquisitionBase, public URIAcquisition<io::InputStream<char> > {

		  private:
			io::InputStream<char>* stream;

		  public:
			StreamAcquisition(const FileURI&, VFS&);
			StreamAcquisition(const StreamAcquisition&);
			virtual ~StreamAcquisition();

			virtual void requestResource();
			virtual io::OutputStream<char>* getRequestStream();
			virtual io::InputStream<char>* getAcquiredResource();
			virtual void releaseAcquiredResource();

		};

		class REDSTRAIN_VFS_API VFSAcquisition
				: public AcquisitionBase, public URIAcquisition<VFS> {

		  private:
			VFS* rootedVFS;

		  public:
			VFSAcquisition(const FileURI&, VFS&);
			VFSAcquisition(const VFSAcquisition&);
			virtual ~VFSAcquisition();

			virtual void requestResource();
			virtual io::OutputStream<char>* getRequestStream();
			virtual VFS* getAcquiredResource();
			virtual void releaseAcquiredResource();

		};

	  private:
		VFS* vfs;
		const URIAcquirer<FileURI>* remoteAcquirer;

	  protected:
		virtual URI* newURIOfThisType(int, const text::String16&, const text::String16&,
				const text::String16&, const text::String16&, uint16_t,
				const text::String16&, const text::String16&, const text::String16&) const;

	  public:
		FileURI(const text::String16&, const Decomposition&);
		FileURI(const Components<text::Char16, CL_RENDITION>&);
		FileURI(const Components<char, CL_ORIGINAL>&);
		FileURI(const Components<text::Char16, CL_ORIGINAL>&);
		FileURI(const Components<text::Char32, CL_ORIGINAL>&);
		FileURI(const std::string&);
		FileURI(const text::String16&);
		FileURI(const text::String32&);
		FileURI(const std::string&, const std::string&);
		FileURI(const text::String16&, const text::String16&);
		FileURI(const text::String32&, const text::String32&);
		FileURI(const FileURI&);

		inline VFS* getVFS() const {
			return vfs;
		}

		inline void setVFS(VFS* vfs) {
			this->vfs = vfs;
		}

		inline const URIAcquirer<FileURI>* getRemoteAcquirer() const {
			return remoteAcquirer;
		}

		inline void setRemoteAcquirer(const URIAcquirer<FileURI>* remoteAcquirer) {
			this->remoteAcquirer = remoteAcquirer;
		}

		virtual bool isLocator() const;
		virtual URIAcquisition<io::InputStream<char> >* acquireResource() const;
		virtual URIAcquisition<VFS>* acquireVFS() const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_FILEURI_HPP */
