#include "Color.hpp"

using std::string;

namespace redengine {
namespace damnation {
namespace tk {

	// ======== ColorSource ========

	Color::ColorSource::ColorSource() {}

	Color::ColorSource::ColorSource(const ColorSource&) {}

	Color::ColorSource::~ColorSource() {}

	// ======== Color ========

	Color::Color(const string& type, const string& use, const string& placement, unsigned fallback)
			: type(type), use(use), placement(placement), fallback(fallback), local(Color::NO_COLOR),
			cached(Color::NO_COLOR) {}

	Color::Color(const Color& color) : type(color.type), use(color.use), placement(color.placement),
			container(color.container), fallback(color.fallback), local(color.local), cached(color.cached) {}

	void Color::setType(const string& type) {
		this->type = type;
		cached = Color::NO_COLOR;
	}

	void Color::setUse(const string& use) {
		this->use = use;
		cached = Color::NO_COLOR;
	}

	void Color::setPlacement(const string& placement) {
		this->placement = placement;
		cached = Color::NO_COLOR;
	}

	void Color::setContainer(const string& container) {
		this->container = container;
		cached = Color::NO_COLOR;
	}

	unsigned Color::get(ColorSource& source) {
		if(local != Color::NO_COLOR)
			return local;
		if(cached == Color::NO_COLOR) {
			if(!container.empty() && !placement.empty())
				tryCache(source, container + '.' + placement);
			if(!use.empty())
				tryCache(source, use);
			if(!type.empty())
				tryCache(source, type);
			if(cached == Color::NO_COLOR)
				cached = fallback;
		}
		return cached;
	}

	void Color::tryCache(ColorSource& source, const string& key) {
		if(cached != Color::NO_COLOR)
			return;
		unsigned color = source.getColor(key);
		if(color != Color::NO_COLOR)
			cached = color;
	}

	void Color::clearCache() {
		cached = Color::NO_COLOR;
	}

}}}
