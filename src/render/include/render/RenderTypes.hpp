#ifndef RENDER_RENDERTYPES_HPP
#define RENDER_RENDERTYPES_HPP

#include <cstddef>

namespace render {

// Opaque handle identifying a registered render target.
using TargetHandle = std::size_t;

static constexpr TargetHandle kInvalidHandle = 0;

}  // namespace render

#endif  // RENDER_RENDERTYPES_HPP
