#ifndef REDSTRAIN_MOD_DAMNATION_TK_COLOR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_COLOR_HPP

#include <string>

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API Color {

	  public:
		class REDSTRAIN_DAMNATION_API ColorSource {

		  public:
			ColorSource();
			ColorSource(const ColorSource&);
			virtual ~ColorSource();

			virtual unsigned getColor(const std::string&) = 0;

		};

	  public:
		static const unsigned NO_COLOR = 256u;
		static const unsigned TERMINAL_DEFAULT = 257u;
		static const unsigned SIMPLE_BLACK = 0u;
		static const unsigned SIMPLE_WHITE = 7u;

	  private:
		std::string type, use, placement, container;
		unsigned fallback, local, cached;

	  private:
		void tryCache(ColorSource&, const std::string&);

	  public:
		Color(const std::string&, const std::string&, const std::string&, unsigned);
		Color(const Color&);

		inline const std::string& getType() const {
			return type;
		}

		void setType(const std::string&);

		inline const std::string& getUse() const {
			return use;
		}

		void setUse(const std::string&);

		inline const std::string& getPlacement() const {
			return placement;
		}

		void setPlacement(const std::string&);

		inline const std::string& getContainer() const {
			return container;
		}

		void setContainer(const std::string&);

		inline unsigned getFallback() const {
			return fallback;
		}

		inline void setFallback(unsigned fallback) {
			this->fallback = fallback;
			cached = Color::NO_COLOR;
		}

		inline unsigned getLocal() const {
			return local;
		}

		inline void setLocal(unsigned local) {
			this->local = local;
		}

		unsigned get(ColorSource&);
		void clearCache();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_COLOR_HPP */
