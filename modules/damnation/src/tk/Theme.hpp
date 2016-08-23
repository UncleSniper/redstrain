#ifndef REDSTRAIN_MOD_DAMNATION_TK_THEME_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_THEME_HPP

#include <map>
#include <redstrain/io/InputStream.hpp>
#include <redstrain/io/OutputStream.hpp>
#include <redstrain/util/ReferenceCounted.hpp>

#include "Color.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API Theme : public Color::ColorSource, public util::ReferenceCounted {

	  private:
		typedef std::map<std::string, unsigned> Colors;

	  public:
		typedef Colors::const_iterator ColorIterator;

	  private:
		Colors colors;

	  public:
		Theme();
		Theme(const Theme&);
		virtual ~Theme();

		void getColors(ColorIterator&, ColorIterator&) const;
		void putColor(const std::string&, unsigned);
		bool removeColor(const std::string&);

		void save(io::OutputStream<char>&) const;
		void load(io::InputStream<char>&);

		virtual unsigned getColor(const std::string&);

		static Theme& getDefaultBuiltinTheme();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_THEME_HPP */
