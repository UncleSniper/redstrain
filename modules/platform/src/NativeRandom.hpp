#include <redstrain/util/Random.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API NativeRandom : public util::Random {

	  public:
		NativeRandom();
		NativeRandom(const NativeRandom&);

		virtual util::MemorySize getSeedSize() const;
		virtual void seed(const char*);
		virtual void randomBytes(char*, util::MemorySize);

		static NativeRandom instance;

	};

}}
