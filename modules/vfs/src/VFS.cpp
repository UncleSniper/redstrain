#include <redstrain/util/Ref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/text/UTF8Encoder16.hpp>
#include <redstrain/text/UTF8Decoder16.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/text/DefaultCodecFactory.hpp>

#include "VFS.hpp"
#include "Unlinker.hpp"
#include "GenericVFile.hpp"

using std::string;
using redengine::util::Ref;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::util::Appender;
using redengine::text::String16;
using redengine::text::Encoder16;
using redengine::text::Decoder16;
using redengine::text::Transcode;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::text::CodecFactory;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF8Decoder16;
using redengine::io::BidirectionalStream;
using redengine::error::ProgrammingError;
using redengine::text::DefaultCodecFactory;

namespace redengine {
namespace vfs {

	static const Char16 parentLinkChars[] = {
		static_cast<Char16>('.'),
		static_cast<Char16>('.')
	};

	const String16 VFS::SELF_LINK(parentLinkChars + 1);
	const String16 VFS::PARENT_LINK(parentLinkChars);

	VFS::FSInfo::FSInfo() : type(UNKNOWNFS), totalBlocks(static_cast<size_t>(0u)),
			freeBlocks(static_cast<size_t>(0u)), totalINodes(static_cast<size_t>(0u)),
			freeINodes(static_cast<size_t>(0u)), nameLength(static_cast<size_t>(0u)) {}

	VFS::FSInfo::FSInfo(const FSInfo& info) : type(info.type), totalBlocks(info.totalBlocks),
			freeBlocks(info.freeBlocks), totalINodes(info.totalINodes), freeINodes(info.freeINodes),
			nameLength(info.nameLength) {}

	VFS::FSInfo& VFS::FSInfo::operator=(const FSInfo& info) {
		type = info.type;
		totalBlocks = info.totalBlocks;
		freeBlocks = info.freeBlocks;
		totalINodes = info.totalINodes;
		freeINodes = info.freeINodes;
		nameLength = info.nameLength;
		return *this;
	}

	VFS::EncodingAppender::EncodingAppender(Appender<string>& sink, VFS& encoder)
			: sink(sink), encoder(encoder) {}

	void VFS::EncodingAppender::append(const String16& element) {
		sink.append(encoder.encodePathname(element));
	}

	void VFS::EncodingAppender::doneAppending() {
		sink.doneAppending();
	}

	VFS::VFS() : encoderFactory(NULL), decoderFactory(NULL) {}

	VFS::VFS(const VFS& vfs) : encoderFactory(vfs.encoderFactory), decoderFactory(vfs.decoderFactory) {
		if(encoderFactory)
			encoderFactory->ref();
		if(decoderFactory)
			decoderFactory->ref();
	}

	VFS::~VFS() {
		if(encoderFactory)
			encoderFactory->unref();
		if(decoderFactory)
			decoderFactory->unref();
	}

	void VFS::setPathnameEncoderFactory(CodecFactory<Encoder16>* encoderFactory) {
		Ref<CodecFactory<Encoder16> > factory(encoderFactory, true);
		if(this->encoderFactory)
			this->encoderFactory->unref();
		this->encoderFactory = encoderFactory;
		factory.set();
	}

	void VFS::setPathnameDecoderFactory(CodecFactory<Decoder16>* decoderFactory) {
		Ref<CodecFactory<Decoder16> > factory(decoderFactory, true);
		if(this->decoderFactory)
			this->decoderFactory->unref();
		this->decoderFactory = decoderFactory;
		factory.set();
	}

	static DefaultCodecFactory<Decoder16, UTF8Decoder16> defaultDecoderFactory;

	String16 VFS::decodePathname(const string& pathname) const {
		Delete<Decoder16> decoder((decoderFactory ? decoderFactory : &defaultDecoderFactory)->newCodec());
		String16 result;
		Transcode::decode(pathname, result, **decoder);
		return result;
	}

	static DefaultCodecFactory<Encoder16, UTF8Encoder16> defaultEncoderFactory;

	string VFS::encodePathname(const String16& pathname) const {
		Delete<Encoder16> encoder((encoderFactory ? encoderFactory : &defaultEncoderFactory)->newCodec());
		string result;
		Transcode::encode(pathname, result, **encoder);
		return result;
	}

	void VFS::deconstructPathname(const string& whole, Pathname& parts) const {
		VFS::deconstructPathname(decodePathname(whole), parts);
	}

	void VFS::deconstructPathname(const String16& whole, Pathname& parts) {
		String16::size_type old = static_cast<String16::size_type>(0u), pos, length = whole.length();
		for(pos = static_cast<String16::size_type>(0u); pos < length; ++pos) {
			switch(whole[pos]) {
				case static_cast<Char16>('/'):
				case static_cast<Char16>('\\'):
					if(!pos)
						parts.clear();
					if(pos > old)
						parts.push_back(whole.substr(old, pos - old));
					old = pos + static_cast<String16::size_type>(1u);
					break;
			}
		}
		if(pos > old)
			parts.push_back(whole.substr(old));
	}

	string VFS::constructBytePathname(const Pathname& path, bool absolute) const {
		return encodePathname(VFS::constructPathname(path.begin(), path.end(), absolute));
	}

	string VFS::constructBytePathname(PathIterator pathBegin, PathIterator pathEnd, bool absolute) const {
		return encodePathname(VFS::constructPathname(pathBegin, pathEnd, absolute));
	}

	String16 VFS::constructPathname(const Pathname& path, bool absolute) {
		return VFS::constructPathname(path.begin(), path.end(), absolute);
	}

	String16 VFS::constructPathname(PathIterator pathBegin, PathIterator pathEnd, bool absolute) {
		String16 result;
		if(absolute)
			result += static_cast<Char16>('/');
		bool first = true;
		for(; pathBegin != pathEnd; ++pathBegin) {
			if(first)
				first = false;
			else
				result += static_cast<Char16>('/');
			result += *pathBegin;
		}
		return result;
	}

	void VFS::stat(const string& path, Stat& info, bool ofLink) {
		Pathname pl;
		deconstructPathname(path, pl);
		stat(pl.begin(), pl.end(), info, ofLink);
	}

	void VFS::stat(const String16& path, Stat& info, bool ofLink) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		stat(pl.begin(), pl.end(), info, ofLink);
	}

	void VFS::stat(const Pathname& path, Stat& info, bool ofLink) {
		stat(path.begin(), path.end(), info, ofLink);
	}

	void VFS::chmod(const string& path, int permissions) {
		Pathname pl;
		deconstructPathname(path, pl);
		chmod(pl.begin(), pl.end(), permissions);
	}

	void VFS::chmod(const String16& path, int permissions) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		chmod(pl.begin(), pl.end(), permissions);
	}

	void VFS::chmod(const Pathname& path, int permissions) {
		chmod(path.begin(), path.end(), permissions);
	}

	void VFS::chown(const string& path, Stat::UserID owner, bool ofLink) {
		Pathname pl;
		deconstructPathname(path, pl);
		chown(pl.begin(), pl.end(), owner, ofLink);
	}

	void VFS::chown(const String16& path, Stat::UserID owner, bool ofLink) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		chown(pl.begin(), pl.end(), owner, ofLink);
	}

	void VFS::chown(const Pathname& path, Stat::UserID owner, bool ofLink) {
		chown(path.begin(), path.end(), owner, ofLink);
	}

	void VFS::chgrp(const string& path, Stat::GroupID group, bool ofLink) {
		Pathname pl;
		deconstructPathname(path, pl);
		chgrp(pl.begin(), pl.end(), group, ofLink);
	}

	void VFS::chgrp(const String16& path, Stat::GroupID group, bool ofLink) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		chgrp(pl.begin(), pl.end(), group, ofLink);
	}

	void VFS::chgrp(const Pathname& path, Stat::GroupID group, bool ofLink) {
		chgrp(path.begin(), path.end(), group, ofLink);
	}

	void VFS::link(const string& oldPath, const string& newPath) {
		Pathname oldpl, newpl;
		deconstructPathname(oldPath, oldpl);
		deconstructPathname(newPath, newpl);
		link(oldpl.begin(), oldpl.end(), newpl.begin(), newpl.end());
	}

	void VFS::link(const String16& oldPath, const String16& newPath) {
		Pathname oldpl, newpl;
		VFS::deconstructPathname(oldPath, oldpl);
		VFS::deconstructPathname(newPath, newpl);
		link(oldpl.begin(), oldpl.end(), newpl.begin(), newpl.end());
	}

	void VFS::link(const Pathname& oldPath, const Pathname& newPath) {
		link(oldPath.begin(), oldPath.end(), newPath.begin(), newPath.end());
	}

	void VFS::unlink(const string& path) {
		Pathname pl;
		deconstructPathname(path, pl);
		unlink(pl.begin(), pl.end());
	}

	void VFS::unlink(const String16& path) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		unlink(pl.begin(), pl.end());
	}

	void VFS::unlink(const Pathname& path) {
		unlink(path.begin(), path.end());
	}

	void VFS::utime(const string& path) {
		Pathname pl;
		deconstructPathname(path, pl);
		utime(pl.begin(), pl.end());
	}

	void VFS::utime(const String16& path) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		utime(pl.begin(), pl.end());
	}

	void VFS::utime(const Pathname& path) {
		utime(path.begin(), path.end());
	}

	void VFS::utime(const string& path, time_t accessTimestamp, time_t modificationTimestamp) {
		Pathname pl;
		deconstructPathname(path, pl);
		utime(pl.begin(), pl.end(), accessTimestamp, modificationTimestamp);
	}

	void VFS::utime(const String16& path, time_t accessTimestamp, time_t modificationTimestamp) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		utime(pl.begin(), pl.end(), accessTimestamp, modificationTimestamp);
	}

	void VFS::utime(const Pathname& path, time_t accessTimestamp, time_t modificationTimestamp) {
		utime(path.begin(), path.end(), accessTimestamp, modificationTimestamp);
	}

	bool VFS::access(const string& path, int permissions) {
		Pathname pl;
		deconstructPathname(path, pl);
		return access(pl.begin(), pl.end(), permissions);
	}

	bool VFS::access(const String16& path, int permissions) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		return access(pl.begin(), pl.end(), permissions);
	}

	bool VFS::access(const Pathname& path, int permissions) {
		return access(path.begin(), path.end(), permissions);
	}

	void VFS::rename(const string& oldPath, const string& newPath) {
		Pathname oldpl, newpl;
		deconstructPathname(oldPath, oldpl);
		deconstructPathname(newPath, newpl);
		rename(oldpl.begin(), oldpl.end(), newpl.begin(), newpl.end());
	}

	void VFS::rename(const String16& oldPath, const String16& newPath) {
		Pathname oldpl, newpl;
		VFS::deconstructPathname(oldPath, oldpl);
		VFS::deconstructPathname(newPath, newpl);
		rename(oldpl.begin(), oldpl.end(), newpl.begin(), newpl.end());
	}

	void VFS::rename(const Pathname& oldPath, const Pathname& newPath) {
		rename(oldPath.begin(), oldPath.end(), newPath.begin(), newPath.end());
	}

	void VFS::mkdir(const string& path, int permissions) {
		Pathname pl;
		deconstructPathname(path, pl);
		mkdir(pl.begin(), pl.end(), permissions);
	}

	void VFS::mkdir(const String16& path, int permissions) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		mkdir(pl.begin(), pl.end(), permissions);
	}

	void VFS::mkdir(const Pathname& path, int permissions) {
		mkdir(path.begin(), path.end(), permissions);
	}

	void VFS::rmdir(const string& path) {
		Pathname pl;
		deconstructPathname(path, pl);
		rmdir(pl.begin(), pl.end());
	}

	void VFS::rmdir(const String16& path) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		rmdir(pl.begin(), pl.end());
	}

	void VFS::rmdir(const Pathname& path) {
		rmdir(path.begin(), path.end());
	}

	void VFS::symlink(const string& oldPath, const string& newPath) {
		Pathname oldpl;
		deconstructPathname(oldPath, oldpl);
		symlink(oldpl.begin(), oldpl.end(), decodePathname(newPath));
	}

	void VFS::symlink(const String16& oldPath, const String16& newPath) {
		Pathname oldpl;
		VFS::deconstructPathname(oldPath, oldpl);
		symlink(oldpl.begin(), oldpl.end(), newPath);
	}

	void VFS::symlink(const Pathname& oldPath, const String16& newPath) {
		symlink(oldPath.begin(), oldPath.end(), newPath);
	}

	void VFS::readlink(const string& path, string& result) {
		Pathname pl;
		deconstructPathname(path, pl);
		String16 result16;
		readlink(pl.begin(), pl.end(), result16);
		result = encodePathname(result16);
	}

	void VFS::readlink(const String16& path, String16& result) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		readlink(pl.begin(), pl.end(), result);
	}

	void VFS::readlink(const Pathname& path, String16& result) {
		readlink(path.begin(), path.end(), result);
	}

	void VFS::readdir(const string& path, Appender<string>& sink) {
		Pathname pl;
		deconstructPathname(path, pl);
		EncodingAppender encApp(sink, *this);
		readdir(pl.begin(), pl.end(), encApp);
	}

	void VFS::readdir(const String16& path, Appender<String16>& sink) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		readdir(pl.begin(), pl.end(), sink);
	}

	void VFS::readdir(const Pathname& path, Appender<String16>& sink) {
		readdir(path.begin(), path.end(), sink);
	}

	void VFS::truncate(const string& path, size_t size) {
		Pathname pl;
		deconstructPathname(path, pl);
		truncate(pl.begin(), pl.end(), size);
	}

	void VFS::truncate(const String16& path, size_t size) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		truncate(pl.begin(), pl.end(), size);
	}

	void VFS::truncate(const Pathname& path, size_t size) {
		truncate(path.begin(), path.end(), size);
	}

	void VFS::statfs(const string& path, FSInfo& info) {
		Pathname pl;
		deconstructPathname(path, pl);
		statfs(pl.begin(), pl.end(), info);
	}

	void VFS::statfs(const String16& path, FSInfo& info) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		statfs(pl.begin(), pl.end(), info);
	}

	void VFS::statfs(const Pathname& path, FSInfo& info) {
		statfs(path.begin(), path.end(), info);
	}

	void VFS::mknod(const string& path, Stat::Type type, int permissions, Stat::DeviceID device) {
		Pathname pl;
		deconstructPathname(path, pl);
		mknod(pl.begin(), pl.end(), type, permissions, device);
	}

	void VFS::mknod(const String16& path, Stat::Type type, int permissions, Stat::DeviceID device) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		mknod(pl.begin(), pl.end(), type, permissions, device);
	}

	void VFS::mknod(const Pathname& path, Stat::Type type, int permissions, Stat::DeviceID device) {
		mknod(path.begin(), path.end(), type, permissions, device);
	}

	InputStream<char>* VFS::getInputStream(const string& path) {
		Pathname pl;
		deconstructPathname(path, pl);
		return getInputStream(pl.begin(), pl.end());
	}

	InputStream<char>* VFS::getInputStream(const String16& path) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		return getInputStream(pl.begin(), pl.end());
	}

	InputStream<char>* VFS::getInputStream(const Pathname& path) {
		return getInputStream(path.begin(), path.end());
	}

	OutputStream<char>* VFS::getOutputStream(const string& path) {
		Pathname pl;
		deconstructPathname(path, pl);
		return getOutputStream(pl.begin(), pl.end());
	}

	OutputStream<char>* VFS::getOutputStream(const String16& path) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		return getOutputStream(pl.begin(), pl.end());
	}

	OutputStream<char>* VFS::getOutputStream(const Pathname& path) {
		return getOutputStream(path.begin(), path.end());
	}

	BidirectionalStream<char>* VFS::getStream(const string& path, bool truncate) {
		Pathname pl;
		deconstructPathname(path, pl);
		return getStream(pl.begin(), pl.end(), truncate);
	}

	BidirectionalStream<char>* VFS::getStream(const String16& path, bool truncate) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		return getStream(pl.begin(), pl.end(), truncate);
	}

	BidirectionalStream<char>* VFS::getStream(const Pathname& path, bool truncate) {
		return getStream(path.begin(), path.end(), truncate);
	}

	VFile* VFS::getFileReference(const string& path) {
		Pathname pl;
		deconstructPathname(path, pl);
		return getFileReference(pl.begin(), pl.end());
	}

	VFile* VFS::getFileReference(const String16& path) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		return getFileReference(pl.begin(), pl.end());
	}

	VFile* VFS::getFileReference(const Pathname& path) {
		return getFileReference(path.begin(), path.end());
	}

	VFile* VFS::getFileReference(PathIterator pathBegin, PathIterator pathEnd) {
		return new GenericVFile(*this, pathBegin, pathEnd);
	}

	void VFS::removeRecursively(const string& path) {
		Pathname pl;
		deconstructPathname(path, pl);
		removeRecursively(pl.begin(), pl.end());
	}

	void VFS::removeRecursively(const String16& path) {
		Pathname pl;
		VFS::deconstructPathname(path, pl);
		removeRecursively(pl.begin(), pl.end());
	}

	void VFS::removeRecursively(const Pathname& path) {
		removeRecursively(path.begin(), path.end());
	}

	static void removeRecursively(VFS&, VFS::Pathname&);

	struct RecursiveRemover : Appender<String16> {

		VFS& vfs;
		VFS::Pathname& stack;

		RecursiveRemover(VFS& vfs, VFS::Pathname& stack) : vfs(vfs), stack(stack) {}

		virtual void append(const String16& child) {
			stack.push_back(child);
			removeRecursively(vfs, stack);
			stack.pop_back();
		}

	};

	static void removeRecursively(VFS& vfs, VFS::Pathname& path) {
		Stat info;
		vfs.stat(path.begin(), path.end(), info, true);
		if(info.getType() != Stat::DIRECTORY) {
			vfs.unlink(path.begin(), path.end());
			return;
		}
		// Good thing we chose std::list for this... Even if
		// the recursive call to removeRecursively() modifies
		// the stack, it will be popped down to its original
		// size before the call returns (and only popped down
		// below that size after *this* call returns), so any
		// and all iterators we give to readdir() will be
		// valid at all times at which readdir() is in control.
		// Since readdir() has no access to the stack, they
		// surely will be valid in any code readdir() might
		// call, as well.
		RecursiveRemover handler(vfs, path);
		vfs.readdir(path.begin(), path.end(), handler);
	}

	void VFS::removeRecursively(PathIterator pathBegin, PathIterator pathEnd) {
		Pathname path;
		path.insert(path.end(), pathBegin, pathEnd);
		::redengine::vfs::removeRecursively(*this, path);
	}

	void VFS::copyTree(const string& oldPath, VFS& fs, const string& newPath, int flags) {
		Pathname oldpl, newpl;
		deconstructPathname(oldPath, oldpl);
		fs.deconstructPathname(newPath, newpl);
		copyTree(oldpl.begin(), oldpl.end(), fs, newpl.begin(), newpl.end(), flags);
	}

	void VFS::copyTree(const String16& oldPath, VFS& fs, const String16& newPath, int flags) {
		Pathname oldpl, newpl;
		VFS::deconstructPathname(oldPath, oldpl);
		VFS::deconstructPathname(newPath, newpl);
		copyTree(oldpl.begin(), oldpl.end(), fs, newpl.begin(), newpl.end(), flags);
	}

	void VFS::copyTree(const Pathname& oldPath, VFS& fs, const Pathname& newPath, int flags) {
		copyTree(oldPath.begin(), oldPath.end(), fs, newPath.begin(), newPath.end(), flags);
	}

	struct TreeCopier : Appender<String16> {

		VFS& sourceFS;
		VFS& destinationFS;
		VFS::Pathname& sourceStack;
		VFS::Pathname& destinationStack;
		int flags;

		TreeCopier(VFS& sourceFS, VFS& destinationFS, VFS::Pathname& sourceStack, VFS::Pathname& destinationStack,
				int flags) : sourceFS(sourceFS), destinationFS(destinationFS), sourceStack(sourceStack),
				destinationStack(destinationStack), flags(flags) {}

		void clearMoveDestination();
		void copyRegularFile();
		void copyDirectory();

		void copy() {
			Stat info;
			sourceFS.stat(sourceStack.begin(), sourceStack.end(), info, true);
			switch(info.getType()) {
				case Stat::REGULAR_FILE:
					copyRegularFile();
					break;
				case Stat::DIRECTORY:
					copyDirectory();
					break;
				case Stat::CHARACTER_DEVICE:
				case Stat::BLOCK_DEVICE:
				case Stat::NAMED_PIPE:
				case Stat::LOCAL_SOCKET:
					clearMoveDestination();
					destinationFS.mknod(destinationStack.begin(), destinationStack.end(), info.getType(),
							info.getPermissions(), info.getDevice());
					break;
				case Stat::SYMBOLIC_LINK:
					{
						String16 linkdest;
						sourceFS.readlink(sourceStack.begin(), sourceStack.end(), linkdest);
						clearMoveDestination();
						destinationFS.symlink(destinationStack.begin(), destinationStack.end(), linkdest);
					}
					break;
				default:
					throw ProgrammingError("Unrecognized file type: "
							+ StringUtils::toString(static_cast<int>(info.getType())));
			}
			if(flags & VFS::PRESERVE_OWNER)
				destinationFS.chown(destinationStack.begin(), destinationStack.end(), info.getOwner(), true);
			if(flags & VFS::PRESERVE_GROUP)
				destinationFS.chgrp(destinationStack.begin(), destinationStack.end(), info.getGroup(), true);
			if(info.getType() != Stat::SYMBOLIC_LINK) {
				if(flags & VFS::PRESERVE_PERMISSIONS)
					destinationFS.chmod(destinationStack.begin(), destinationStack.end(), info.getPermissions());
				if(flags & VFS::PRESERVE_TIMESTAMPS)
					destinationFS.utime(destinationStack.begin(), destinationStack.end(),
							info.getAccessTimestamp(), info.getModificationTimestamp());
			}
			if(flags & VFS::REMOVE_SOURCE) {
				if(info.getType() == Stat::DIRECTORY)
					sourceFS.rmdir(sourceStack.begin(), sourceStack.end());
				else
					sourceFS.unlink(sourceStack.begin(), sourceStack.end());
			}
		}

		virtual void append(const String16& child) {
			sourceStack.push_back(child);
			destinationStack.push_back(child);
			copy();
			destinationStack.pop_back();
			sourceStack.pop_back();
		}

	};

	void TreeCopier::clearMoveDestination() {
		if(!destinationFS.access(destinationStack.begin(), destinationStack.end(), VFS::FILE_EXISTS))
			return;
		Stat info;
		destinationFS.stat(destinationStack.begin(), destinationStack.end(), info, true);
		if(info.getType() == Stat::DIRECTORY)
			destinationFS.rmdir(destinationStack.begin(), destinationStack.end());
		else
			destinationFS.unlink(destinationStack.begin(), destinationStack.end());
	}

	void TreeCopier::copyRegularFile() {
		clearMoveDestination();
		Delete<InputStream<char> > in(sourceFS.getInputStream(sourceStack.begin(), sourceStack.end()));
		StreamCloser closeIn(*in);
		Delete<OutputStream<char> > out(destinationFS.getOutputStream(destinationStack.begin(),
				destinationStack.end()));
		Unlinker unlinkOut(destinationFS, destinationStack.begin(), destinationStack.end(), false);
		StreamCloser closeOut(*out);
		in->copyTo(**out);
		in->close();
		closeIn.release();
		out->close();
		closeOut.release();
		unlinkOut.release();
	}

	void TreeCopier::copyDirectory() {
		Unlinker uncreate(destinationFS, true);
		if(destinationFS.access(destinationStack.begin(), destinationStack.end(), VFS::FILE_EXISTS)) {
			Stat info;
			destinationFS.stat(destinationStack.begin(), destinationStack.end(), info, false);
			if(info.getType() != Stat::DIRECTORY) {
				destinationFS.unlink(destinationStack.begin(), destinationStack.end());
				destinationFS.mkdir(destinationStack.begin(), destinationStack.end(),
						Stat::DEFAULT_DIRECORY_PERMISSIONS);
				uncreate = destinationStack;
			}
		}
		else {
			destinationFS.mkdir(destinationStack.begin(), destinationStack.end(),
					Stat::DEFAULT_DIRECORY_PERMISSIONS);
			uncreate = destinationStack;
		}
		sourceFS.readdir(sourceStack.begin(), sourceStack.end(), *this);
		uncreate.release();
	}

	void VFS::copyTree(PathIterator oldPathBegin, PathIterator oldPathEnd, VFS& fs,
			PathIterator newPathBegin, PathIterator newPathEnd, int flags) {
		if(&fs == this && flags & VFS::REMOVE_SOURCE) {
			rename(oldPathBegin, oldPathEnd, newPathBegin, newPathEnd);
			return;
		}
		Pathname oldPath, newPath;
		oldPath.insert(oldPath.end(), oldPathBegin, oldPathEnd);
		newPath.insert(newPath.end(), newPathBegin, newPathEnd);
		TreeCopier copier(*this, fs, oldPath, newPath, flags);
		copier.copy();
	}

	void VFS::copyTree(const string& oldPath, const string& newPath, int flags) {
		Pathname oldpl, newpl;
		deconstructPathname(oldPath, oldpl);
		deconstructPathname(newPath, newpl);
		copyTree(oldpl.begin(), oldpl.end(), *this, newpl.begin(), newpl.end(), flags);
	}

	void VFS::copyTree(const String16& oldPath, const String16& newPath, int flags) {
		Pathname oldpl, newpl;
		VFS::deconstructPathname(oldPath, oldpl);
		VFS::deconstructPathname(newPath, newpl);
		copyTree(oldpl.begin(), oldpl.end(), *this, newpl.begin(), newpl.end(), flags);
	}

	void VFS::copyTree(const Pathname& oldPath, const Pathname& newPath, int flags) {
		copyTree(oldPath.begin(), oldPath.end(), *this, newPath.begin(), newPath.end(), flags);
	}

	void VFS::copyTree(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd, int flags) {
		copyTree(oldPathBegin, oldPathEnd, *this, newPathBegin, newPathEnd, flags);
	}

}}
