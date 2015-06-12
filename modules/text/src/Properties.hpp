#ifndef REDSTRAIN_MOD_TEXT_PROPERTIES_HPP
#define REDSTRAIN_MOD_TEXT_PROPERTIES_HPP

#include <map>
#include <string>
#include <redstrain/io/InputStream.hpp>
#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API Properties {

	  public:
		typedef std::map<String16, String16> Map;
		typedef Map::const_iterator Iterator;

	  private:
		Map map;

	  private:
		void sinkLogicalLine(const String16&);

	  public:
		Properties();
		Properties(const Properties&);

		bool hasProperty(const std::string&) const;
		bool hasProperty(const String16&) const;

		std::string getProperty(const std::string&) const;
		std::string getProperty(const String16&) const;
		String16 getProperty16(const std::string&) const;
		String16 getProperty16(const String16&) const;

		void setProperty(const std::string&, const std::string&);
		void setProperty(const std::string&, const String16&);
		void setProperty(const String16&, const std::string&);
		void setProperty(const String16&, const String16&);

		void getProperties(Iterator&, Iterator&) const;

		void load(io::InputStream<char>&);
		void load(io::InputStream<Char16>&);
		void save(io::OutputStream<char>&) const;
		void save(io::OutputStream<Char16>&) const;

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_PROPERTIES_HPP */
