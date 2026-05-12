#ifndef RENDER_RENDERSCHEDULER_HPP
#define RENDER_RENDERSCHEDULER_HPP

#include <cstddef>
#include <memory>
#include <unordered_set>
#include <vector>

#include "render/RenderTypes.hpp"

class vtkRenderWindow;
class vtkResliceImageViewer;

namespace render {

class IRenderTarget;

/**
 * @brief Centralized render request and scheduling system.
 *
 * # Ownership model
 *   - IViewController subclasses own one RenderScheduler each (unique_ptr).
 *   - The scheduler owns its registered IRenderTarget objects (unique_ptr).
 *   - No global singleton — controllers pass a raw pointer down to
 *     SliceController / SphereController / overlays as needed.
 *
 * # Lifecycle
 *   1. Controller creates the scheduler in _Initialize().
 *   2. Controller registers every render window (RegisterWindow / RegisterRiv).
 *   3. During interactions, subsystems call RequestRender(window).
 *   4. After all state mutations for one logical event are done, the
 *      controller (or the highest-level caller) calls Flush().
 *      Flush() calls ExecuteRender() exactly once per dirty window and
 *      clears the dirty set.
 *
 * # Deduplication guarantee
 *   Multiple RequestRender() calls for the same vtkRenderWindow within one
 *   flush cycle collapse into a single ExecuteRender() call.
 *
 * # Two render modes
 *   - Multi-window (3 independent vtkRenderWindows):
 *       RegisterWindow(win) × 3 — one WindowRenderTarget per window.
 *   - Shared viewport (1 vtkRenderWindow, 3 vtkRenderers):
 *       RegisterWindow(win) × 1 — one WindowRenderTarget for the shared window.
 *       RequestRender(win) from any of the three renderers' owners collapses
 *       into one window->Render().
 *
 * # vtkResliceImageViewer (RIV) targets
 *   RegisterRiv(riv) creates a RivRenderTarget that calls riv->Render()
 *   (pipeline sync) then window->Render(). Requesting render on a RIV target
 *   marks the underlying window dirty AND ensures the RIV Render() runs first.
 */
class RenderScheduler {
  public:
    RenderScheduler();
    ~RenderScheduler();

    RenderScheduler(const RenderScheduler&) = delete;
    RenderScheduler& operator=(const RenderScheduler&) = delete;

    // ── Registration ────────────────────────────────────────────────────────

    /**
     * @brief Register a plain vtkRenderWindow as a render target.
     * @return Handle that can be passed to Unregister().
     */
    TargetHandle RegisterWindow(vtkRenderWindow* window);

    /**
     * @brief Register a vtkResliceImageViewer as a render target.
     *
     * The scheduler will call riv->Render() before window->Render() so that
     * the reslice pipeline (UpdateDisplayExtent / NeedToRenderOn) runs first.
     */
    TargetHandle RegisterRiv(vtkResliceImageViewer* riv);

    /** @brief Remove a previously registered target. */
    void Unregister(TargetHandle handle);

    // ── Request / Flush ─────────────────────────────────────────────────────

    /**
     * @brief Mark the target associated with @p window as needing a render.
     *
     * Safe to call many times per frame — deduplication happens at Flush().
     * No-op if @p window is not registered.
     */
    void RequestRender(vtkRenderWindow* window);

    /**
     * @brief Mark ALL registered targets as needing a render.
     */
    void RequestRenderAll();

    /**
     * @brief Execute one render per dirty window and clear the dirty set.
     *
     * Should be called once at the end of each logical interaction event
     * (e.g., at the end of a controller method that may have triggered
     * multiple RequestRender() calls internally).
     */
    void Flush();

    // ── Introspection ────────────────────────────────────────────────────────

    /** @brief True if at least one target is marked dirty. */
    bool HasPendingRenders() const;

    /** @brief Number of registered targets. */
    std::size_t TargetCount() const;

  private:
    TargetHandle _NextHandle();
    void _FireListeners(vtkRenderWindow* window, double renderTimeMs, double fps);

    struct Entry {
        TargetHandle handle;
        vtkRenderWindow* window{nullptr};  // weak — owned by VTK/Qt
        std::unique_ptr<IRenderTarget> target;
    };

    std::vector<Entry> m_entries;
    std::unordered_set<vtkRenderWindow*> m_dirty;
    TargetHandle m_nextHandle{1};
};

}  // namespace render

#endif  // RENDER_RENDERSCHEDULER_HPP
