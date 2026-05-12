#include "render/IRenderTarget.hpp"

namespace render {

IRenderTarget::IRenderTarget(vtkRenderWindow* window)
    : m_window(window) {}

vtkRenderWindow* IRenderTarget::GetRenderWindow() const {
    return m_window;
}

}  // namespace render
