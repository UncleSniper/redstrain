#include "CipherFactory.hpp"

namespace redengine {
namespace crypto {

	CipherFactory::CipherFactory() {}

	CipherFactory::CipherFactory(const CipherFactory& factory) : ReferenceCounted(factory) {}

}}
