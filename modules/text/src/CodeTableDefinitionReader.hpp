#ifndef REDSTRAIN_MOD_TEXT_CODETABLEDEFINITIONREADER_HPP
#define REDSTRAIN_MOD_TEXT_CODETABLEDEFINITIONREADER_HPP

#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/parsenum.hpp>

#include "StringUtils.hpp"
#include "CachedCodeTable.hpp"
#include "TooFewCodeTableEntriesError.hpp"
#include "TooManyCodeTableEntriesError.hpp"

namespace redengine {
namespace text {

	template<
		typename TableCharT = Char16,
		typename DefinitionCharT = char
	>
	class CodeTableDefinitionReader {

	  public:
		class Sink {

		  public:
			Sink() {}
			Sink(const Sink&) {}
			virtual ~Sink() {}

			virtual void mapChar(char, TableCharT) = 0;

		};

		class CachedCodeTableSink : public Sink {

		  private:
			CachedCodeTable<TableCharT>& table;

		  public:
			CachedCodeTableSink(CachedCodeTable<TableCharT>& table) : table(table) {}
			CachedCodeTableSink(const CachedCodeTableSink& sink) : Sink(sink), table(sink.table) {}

			inline CachedCodeTable<TableCharT>& getCodeTable() {
				return table;
			}

			inline const CachedCodeTable<TableCharT>& getCodeTable() const {
				return table;
			}

			virtual void mapChar(char eight, TableCharT value) {
				table.map(eight, value);
			}

		};

	  public:
		static void readDefinition(io::InputStream<DefinitionCharT>& source, Sink& sink) {
			io::FormattedInputStream<DefinitionCharT> formatted(source);
			std::basic_string<DefinitionCharT> line;
			unsigned lno = 0u, eight = 0u;
			while(formatted.readLine(line)) {
				++lno;
				typename std::basic_string<DefinitionCharT>::size_type pos
						= line.find(static_cast<DefinitionCharT>('#'));
				if(pos != std::basic_string<DefinitionCharT>::npos)
					line.erase(pos);
				line = StringUtils<DefinitionCharT>::trim(line);
				if(line.empty())
					continue;
				//TODO: wrap exceptions in something more reasonable, with lno included
				TableCharT value = io::parseInteger<
					TableCharT,
					DefinitionCharT,
					io::DefaultNumberCharSemantics<DefinitionCharT, TableCharT>,
					io::DefaultNumberErrorRenderer<DefinitionCharT>
				>(line, io::HEXADECIMAL);
				sink.mapChar(static_cast<char>(static_cast<unsigned char>(eight)), value);
				if(++eight > 256u)
					throw TooManyCodeTableEntriesError();
				line.clear();
			}
			if(eight < 256u)
				throw TooFewCodeTableEntriesError(eight);
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CODETABLEDEFINITIONREADER_HPP */
