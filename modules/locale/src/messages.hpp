#ifndef REDSTRAIN_MOD_LOCALE_MESSAGES_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGES_HPP

#include <map>
#include <set>
#include <list>
#include <redstrain/text/StringUtils.hpp>
#include <redstrain/text/TranscodeForError.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>
#include <redstrain/io/MissingInputSeparatorError.hpp>
#include <redstrain/io/streamtypes.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "MessageCache.hpp"
#include "MissingMessageKeyError.hpp"
#include "DuplicateMessageKeyError.hpp"
#include "UndefinedMessageKeyError.hpp"

namespace redengine {
namespace locale {

	template<typename CharT>
	void readMessages(const std::string& inputStreamName, io::InputStream<CharT>& inputStream,
			MessageCache<CharT>& cache, const std::list<std::basic_string<CharT> >* order) {
		typedef std::basic_string<CharT> String;
		typedef typename String::size_type StringLength;
		typedef std::pair<String, unsigned> PrecachePair;
		typedef std::map<String, PrecachePair> Precache;
		io::FormattedInputStream<CharT> formattedInput(inputStream);
		String line;
		unsigned lno = 0u;
		Precache precache;
		cache.clear();
		while(formattedInput.readLine(line)) {
			++lno;
			line = text::StringUtils<CharT>::trim(line, text::StringUtils<CharT>::TRIM_FRONT);
			if(line.empty())
				continue;
			if(line[0] == static_cast<CharT>('#')) {
				line.clear();
				continue;
			}
			StringLength pos = line.find(static_cast<CharT>('='));
			if(pos == String::npos)
				throw io::MissingInputSeparatorError("=", inputStreamName, lno);
			String key(line.substr(static_cast<StringLength>(0u), pos));
			String value(line.substr(pos + static_cast<StringLength>(1u)));
			if(precache.find(key) != precache.end())
				throw DuplicateMessageKeyError(text::TranscodeForError<CharT>::toCharString(key),
						inputStreamName, lno);
			precache[key] = PrecachePair(value, lno);
			if(!order)
				cache.addMessage(key, value);
			line.clear();
		}
		if(order) {
			typename std::list<std::basic_string<CharT> >::const_iterator
					obegin(order->begin()), oend(order->end());
			for(; obegin != oend; ++obegin) {
				typename Precache::iterator it(precache.find(*obegin));
				if(it == precache.end())
					throw MissingMessageKeyError(text::TranscodeForError<CharT>::toCharString(*obegin),
							inputStreamName);
				cache.addMessage(it->first, it->second.first);
				precache.erase(it);
			}
			if(!precache.empty()) {
				typename Precache::iterator front = precache.begin();
				throw UndefinedMessageKeyError(text::TranscodeForError<CharT>::toCharString(front->first),
						inputStreamName, front->second.second);
			}
		}
	}

	template<typename CharT>
	void getMessageKeyOrder(const MessageCache<CharT>& cache,
			std::map<std::basic_string<CharT>, util::MemorySize>* mapOrder,
			std::list<std::basic_string<CharT> >* listOrder) {
		if(mapOrder)
			mapOrder->clear();
		if(listOrder)
			listOrder->clear();
		typename MessageCache<CharT>::ItemIterator begin, end;
		cache.getMessages(begin, end);
		for(; begin != end; ++begin) {
			if(mapOrder) {
				util::MemorySize index = static_cast<util::MemorySize>(mapOrder->size());
				(*mapOrder)[begin->getKey()] = index;
			}
			if(listOrder)
				listOrder->push_back(begin->getKey());
		}
	}

	template<typename CharT>
	void generateMessageKeyHeader(const MessageCache<CharT>& cache, io::OutputStream<char>& outputStream,
			const std::string typeName, const std::string& constantPrefix = "MSG_",
			const std::string& guardMacro = "") {
		using redengine::io::operator<<;
		using redengine::io::endln;
		using redengine::io::shift;
		using redengine::io::indent;
		using redengine::io::unshift;
		io::DefaultConfiguredOutputStream<char>::Stream formattedOutput(outputStream);
		typename MessageCache<CharT>::ItemIterator begin, end;
		if(!guardMacro.empty()) {
			formattedOutput << "#ifndef " << guardMacro << endln;
			formattedOutput << "#define " << guardMacro << endln << endln;
		}
		class NamespaceOpener : public util::Appender<std::string> {
		  private:
			io::DefaultConfiguredOutputStream<char>::Stream& output;
			std::string lastSegment;
			unsigned level;
		  public:
			NamespaceOpener(io::DefaultConfiguredOutputStream<char>::Stream& output)
					: output(output), level(0u) {}
			virtual void append(const std::string& segment) {
				std::string seg(text::StringUtils<char>::trim(segment));
				if(seg.empty())
					return;
				if(!lastSegment.empty()) {
					output << indent << "namespace " << lastSegment << " {" << endln;
					++level;
				}
				lastSegment = seg;
			}
			virtual void doneAppending() {
				if(level)
					output << shift << endln;
				output << indent << "enum " << lastSegment << " {" << endln << shift;
			}
			void closeAll() {
				output << unshift << indent << "};" << endln;
				if(level) {
					output << endln << unshift << indent;
					for(; level; --level)
						output << '}';
					output << endln;
				}
			}
		} nsOpener(formattedOutput);
		text::StringUtils<char>::split(typeName, "::", nsOpener);
		cache.getMessages(begin, end);
		for(; begin != end; ++begin)
			formattedOutput << indent << constantPrefix
					<< text::TranscodeForError<CharT>::toCharString(begin->getKey()) << ',' << endln;
		formattedOutput << indent << constantPrefix << "_LAST" << endln;
		nsOpener.closeAll();
		if(!guardMacro.empty())
			formattedOutput << endln << "#endif /* " << guardMacro << " */" << endln;
	}

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGES_HPP */
