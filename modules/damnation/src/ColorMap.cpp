#include <cmath>
#include <cstring>
#include <stdint.h>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "ColorMap.hpp"
#include "IllegalColorIndexError.hpp"

using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace damnation {

#define rgbl(literal) static_cast<uint32_t>(literal ## u)

	// renditions of high colors as per xterm default
	static const uint32_t SPEC256[] = {
		// 0..7
		rgbl(0x000000), rgbl(0xcd0000), rgbl(0x00cd00), rgbl(0xcdcd00),
		rgbl(0x0000ff), rgbl(0xcd00cd), rgbl(0xcd00cd), rgbl(0xe5e5e5),
		// 8..15
		rgbl(0x7f7f7f), rgbl(0xff0000), rgbl(0x00ff00), rgbl(0xffff00),
		rgbl(0x5c5cff), rgbl(0xff00ff), rgbl(0x00ffff), rgbl(0xffffff),
		// 16..255, V..ygtpqq..239@q'd from 256colres.h
		rgbl(0x000000), rgbl(0x00005f), rgbl(0x000087), rgbl(0x0000af),
		rgbl(0x0000d7), rgbl(0x0000ff), rgbl(0x005f00), rgbl(0x005f5f),
		rgbl(0x005f87), rgbl(0x005faf), rgbl(0x005fd7), rgbl(0x005fff),
		rgbl(0x008700), rgbl(0x00875f), rgbl(0x008787), rgbl(0x0087af),
		rgbl(0x0087d7), rgbl(0x0087ff), rgbl(0x00af00), rgbl(0x00af5f),
		rgbl(0x00af87), rgbl(0x00afaf), rgbl(0x00afd7), rgbl(0x00afff),
		rgbl(0x00d700), rgbl(0x00d75f), rgbl(0x00d787), rgbl(0x00d7af),
		rgbl(0x00d7d7), rgbl(0x00d7ff), rgbl(0x00ff00), rgbl(0x00ff5f),
		rgbl(0x00ff87), rgbl(0x00ffaf), rgbl(0x00ffd7), rgbl(0x00ffff),
		rgbl(0x5f0000), rgbl(0x5f005f), rgbl(0x5f0087), rgbl(0x5f00af),
		rgbl(0x5f00d7), rgbl(0x5f00ff), rgbl(0x5f5f00), rgbl(0x5f5f5f),
		rgbl(0x5f5f87), rgbl(0x5f5faf), rgbl(0x5f5fd7), rgbl(0x5f5fff),
		rgbl(0x5f8700), rgbl(0x5f875f), rgbl(0x5f8787), rgbl(0x5f87af),
		rgbl(0x5f87d7), rgbl(0x5f87ff), rgbl(0x5faf00), rgbl(0x5faf5f),
		rgbl(0x5faf87), rgbl(0x5fafaf), rgbl(0x5fafd7), rgbl(0x5fafff),
		rgbl(0x5fd700), rgbl(0x5fd75f), rgbl(0x5fd787), rgbl(0x5fd7af),
		rgbl(0x5fd7d7), rgbl(0x5fd7ff), rgbl(0x5fff00), rgbl(0x5fff5f),
		rgbl(0x5fff87), rgbl(0x5fffaf), rgbl(0x5fffd7), rgbl(0x5fffff),
		rgbl(0x870000), rgbl(0x87005f), rgbl(0x870087), rgbl(0x8700af),
		rgbl(0x8700d7), rgbl(0x8700ff), rgbl(0x875f00), rgbl(0x875f5f),
		rgbl(0x875f87), rgbl(0x875faf), rgbl(0x875fd7), rgbl(0x875fff),
		rgbl(0x878700), rgbl(0x87875f), rgbl(0x878787), rgbl(0x8787af),
		rgbl(0x8787d7), rgbl(0x8787ff), rgbl(0x87af00), rgbl(0x87af5f),
		rgbl(0x87af87), rgbl(0x87afaf), rgbl(0x87afd7), rgbl(0x87afff),
		rgbl(0x87d700), rgbl(0x87d75f), rgbl(0x87d787), rgbl(0x87d7af),
		rgbl(0x87d7d7), rgbl(0x87d7ff), rgbl(0x87ff00), rgbl(0x87ff5f),
		rgbl(0x87ff87), rgbl(0x87ffaf), rgbl(0x87ffd7), rgbl(0x87ffff),
		rgbl(0xaf0000), rgbl(0xaf005f), rgbl(0xaf0087), rgbl(0xaf00af),
		rgbl(0xaf00d7), rgbl(0xaf00ff), rgbl(0xaf5f00), rgbl(0xaf5f5f),
		rgbl(0xaf5f87), rgbl(0xaf5faf), rgbl(0xaf5fd7), rgbl(0xaf5fff),
		rgbl(0xaf8700), rgbl(0xaf875f), rgbl(0xaf8787), rgbl(0xaf87af),
		rgbl(0xaf87d7), rgbl(0xaf87ff), rgbl(0xafaf00), rgbl(0xafaf5f),
		rgbl(0xafaf87), rgbl(0xafafaf), rgbl(0xafafd7), rgbl(0xafafff),
		rgbl(0xafd700), rgbl(0xafd75f), rgbl(0xafd787), rgbl(0xafd7af),
		rgbl(0xafd7d7), rgbl(0xafd7ff), rgbl(0xafff00), rgbl(0xafff5f),
		rgbl(0xafff87), rgbl(0xafffaf), rgbl(0xafffd7), rgbl(0xafffff),
		rgbl(0xd70000), rgbl(0xd7005f), rgbl(0xd70087), rgbl(0xd700af),
		rgbl(0xd700d7), rgbl(0xd700ff), rgbl(0xd75f00), rgbl(0xd75f5f),
		rgbl(0xd75f87), rgbl(0xd75faf), rgbl(0xd75fd7), rgbl(0xd75fff),
		rgbl(0xd78700), rgbl(0xd7875f), rgbl(0xd78787), rgbl(0xd787af),
		rgbl(0xd787d7), rgbl(0xd787ff), rgbl(0xd7af00), rgbl(0xd7af5f),
		rgbl(0xd7af87), rgbl(0xd7afaf), rgbl(0xd7afd7), rgbl(0xd7afff),
		rgbl(0xd7d700), rgbl(0xd7d75f), rgbl(0xd7d787), rgbl(0xd7d7af),
		rgbl(0xd7d7d7), rgbl(0xd7d7ff), rgbl(0xd7ff00), rgbl(0xd7ff5f),
		rgbl(0xd7ff87), rgbl(0xd7ffaf), rgbl(0xd7ffd7), rgbl(0xd7ffff),
		rgbl(0xff0000), rgbl(0xff005f), rgbl(0xff0087), rgbl(0xff00af),
		rgbl(0xff00d7), rgbl(0xff00ff), rgbl(0xff5f00), rgbl(0xff5f5f),
		rgbl(0xff5f87), rgbl(0xff5faf), rgbl(0xff5fd7), rgbl(0xff5fff),
		rgbl(0xff8700), rgbl(0xff875f), rgbl(0xff8787), rgbl(0xff87af),
		rgbl(0xff87d7), rgbl(0xff87ff), rgbl(0xffaf00), rgbl(0xffaf5f),
		rgbl(0xffaf87), rgbl(0xffafaf), rgbl(0xffafd7), rgbl(0xffafff),
		rgbl(0xffd700), rgbl(0xffd75f), rgbl(0xffd787), rgbl(0xffd7af),
		rgbl(0xffd7d7), rgbl(0xffd7ff), rgbl(0xffff00), rgbl(0xffff5f),
		rgbl(0xffff87), rgbl(0xffffaf), rgbl(0xffffd7), rgbl(0xffffff),
		rgbl(0x080808), rgbl(0x121212), rgbl(0x1c1c1c), rgbl(0x262626),
		rgbl(0x303030), rgbl(0x3a3a3a), rgbl(0x444444), rgbl(0x4e4e4e),
		rgbl(0x585858), rgbl(0x626262), rgbl(0x6c6c6c), rgbl(0x767676),
		rgbl(0x808080), rgbl(0x8a8a8a), rgbl(0x949494), rgbl(0x9e9e9e),
		rgbl(0xa8a8a8), rgbl(0xb2b2b2), rgbl(0xbcbcbc), rgbl(0xc6c6c6),
		rgbl(0xd0d0d0), rgbl(0xdadada), rgbl(0xe4e4e4), rgbl(0xeeeeee),
	};

	ColorMap::ColorMap() {
		resetColors();
		resetContrast();
	}

	ColorMap::ColorMap(const ColorMap& map) {
		memcpy(map256, map.map256, static_cast<size_t>(256u));
		memcpy(contrast256, map.contrast256, static_cast<size_t>(256u));
	}

	unsigned ColorMap::highToLowColor(unsigned color) const {
		if(color >= 256u)
			throw IllegalColorIndexError(color);
		return static_cast<unsigned>(map256[color]);
	}

	unsigned ColorMap::contrastTo(unsigned color) const {
		if(color >= 256u)
			throw IllegalColorIndexError(color);
		return static_cast<unsigned>(contrast256[color]);
	}

	void ColorMap::putHighToLow(unsigned highColor, unsigned lowColor) {
		if(highColor >= 256u)
			throw IllegalColorIndexError(highColor);
		if(lowColor >= 8u)
			throw IllegalColorIndexError(lowColor);
		map256[highColor] = static_cast<unsigned char>(lowColor);
	}

	void ColorMap::putContrast(unsigned color, unsigned contrast) {
		if(color >= 256u)
			throw IllegalColorIndexError(color);
		if(contrast)
			throw IllegalColorIndexError(contrast);
		contrast256[color] = contrast;
	}

	// Thank you, http://www.easyrgb.com/index.php?X=MATH&H=02#text2
	// for the math involved!

	static double rgb2xyz_sanitize(double rgb) {
		if(rgb > 0.04045)
			return pow((rgb + 0.055) / 1.055, 2.4);
		else
			return rgb / 12.92;
	}

	static void rgb2xyz(double r, double g, double b, double& x, double& y, double& z) {
		r = rgb2xyz_sanitize(r) * 100.0;
		g = rgb2xyz_sanitize(g) * 100.0;
		b = rgb2xyz_sanitize(b) * 100.0;
		x = r * 0.4124 + g * 0.3576 + b * 0.1805;
		y = r * 0.2126 + g * 0.7152 + b * 0.0722;
		z = r * 0.0193 + g * 0.1192 + b * 0.9505;
	}

	static double xyz2lab_sanitize(double xyz) {
		if(xyz > 0.008856)
			return pow(xyz, 1.0 / 3.0);
		else
			return 7.787 * xyz + 16.0 / 116.0;
	}

	static void xyz2lab(double x, double y, double z, double& l, double& a, double& b) {
		x = xyz2lab_sanitize(x / 95.047);
		y = xyz2lab_sanitize(y / 100.0);
		z = xyz2lab_sanitize(z / 108.883);
		l = 116.0 * y - 16.0;
		a = 500.0 * (x - y);
		b = 200.0 * (y - z);
	}

	static void rgb2lab(uint32_t rgb, double& l, double& a, double& b) {
#define rgb_c(index) (static_cast<double>((rgb >> (16 - index * 8)) & static_cast<uint32_t>(0xFFu)) / 255.0)
		double rgb_r = rgb_c(0), rgb_g = rgb_c(1), rgb_b = rgb_c(2);
#undef rgb_c
		double x, y, z;
		rgb2xyz(rgb_r, rgb_g, rgb_b, x, y, z);
		xyz2lab(x, y, z, l, a, b);
	}

	static double labDistance(double la, double aa, double ba, double lb, double ab, double bb) {
		double dL = la - lb, da = aa - ab, db = ba - bb;
		return sqrt(dL * dL + da * da + db * db);
	}

	void ColorMap::resetColors() {
		double base[8][3];
		unsigned u;
		for(u = 0u; u < 8u; ++u) {
			map256[u] = static_cast<unsigned char>(u);
			rgb2lab(SPEC256[u], base[u][0], base[u][1], base[u][2]);
		}
		for(u = 8u; u < 256u; ++u) {
			double l, a, b;
			rgb2lab(SPEC256[u], l, a, b);
			unsigned bestBase = 0u;
			double shortestDistance = 0.0;
			unsigned bi;
			for(bi = 0u; bi < 8u; ++bi) {
				double delta = labDistance(l, a, b, base[u][0], base[u][1], base[u][2]);
				if(!bi || delta < shortestDistance) {
					bestBase = bi;
					shortestDistance = delta;
				}
			}
			map256[u] = static_cast<unsigned char>(bestBase);
		}
	}

	void ColorMap::resetContrast() {
		double wL, wa, wb, bL, ba, bb;
		rgb2lab(SPEC256[0], bL, ba, bb);
		rgb2lab(SPEC256[7], wL, wa, wb);
		unsigned u;
		for(u = 0u; u < 256u; ++u) {
			double l, a, b;
			rgb2lab(SPEC256[u], l, a, b);
			double dBlack = labDistance(l, a, b, bL, ba, bb);
			double dWhite = labDistance(l, a, b, wL, wa, wb);
			contrast256[u] = static_cast<unsigned char>(dBlack < dWhite ? 0u : 7u);
		}
	}

	void ColorMap::fixKnownIssues() {}

	class DefaultColorMapSynchronizedSingleton : public SynchronizedSingleton<ColorMap> {

	  protected:
		virtual ColorMap* newInstance();

	  public:
		DefaultColorMapSynchronizedSingleton();
		DefaultColorMapSynchronizedSingleton(const DefaultColorMapSynchronizedSingleton&);

	};

	DefaultColorMapSynchronizedSingleton::DefaultColorMapSynchronizedSingleton() {}

	DefaultColorMapSynchronizedSingleton::DefaultColorMapSynchronizedSingleton(const
			DefaultColorMapSynchronizedSingleton& singleton) : SynchronizedSingleton<ColorMap>(singleton) {}

	ColorMap* DefaultColorMapSynchronizedSingleton::newInstance() {
		ColorMap* map = new ColorMap;
		map->fixKnownIssues();
		return map;
	}

	static DefaultColorMapSynchronizedSingleton defaultColorMapSynchronizedSingleton;

	const ColorMap& ColorMap::getDefaultColorMap() {
		return defaultColorMapSynchronizedSingleton.get();
	}

}}
