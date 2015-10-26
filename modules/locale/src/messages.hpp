#ifndef REDSTRAIN_MOD_LOCALE_MESSAGES_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGES_HPP

#include <map>
#include <set>
#include <list>
#include <redstrain/text/StringUtils.hpp>
#include <redstrain/text/TranscodeForError.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/MissingInputSeparatorError.hpp>

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
	void getMessageKeyOrder(const MessageCache<CharT>& cache, std::map<std::basic_string<CharT>, size_t>& mapOrder) {
		mapOrder.clear();
		typename MessageCache<CharT>::ItemIterator begin, end;
		cache.getMessages(begin, end);
		for(; begin != end; ++begin) {
			size_t index = static_cast<size_t>(mapOrder.size());
			mapOrder[begin->getKey()] = index;
		}
	}

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGES_HPP */
