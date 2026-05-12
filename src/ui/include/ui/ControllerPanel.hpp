#ifndef CONTROLLERPANEL_HPP
#define CONTROLLERPANEL_HPP

#include <QWidget>

class QColor;

namespace overlays {
enum class OverlayPosition;
}

namespace ui {
class ControllerPanelDicomItem;
class ControllerPanelFPSOverlayItem;
class ControllerPanelSphereItem;

/**
 * @brief Shared control panel used by both Viewport and Multi-Window modes.
 *
 * Stacks a ControllerPanelDicomItem (folder/series selection),
 * a ControllerPanelSphereItem (sphere toggle, radius, colour), and a
 * ControllerPanelFPSOverlayItem (FPS overlay configuration) with separators.
 */
class ControllerPanel : public QWidget {
    Q_OBJECT
  public:
    explicit ControllerPanel(QWidget* parent = nullptr);
    ~ControllerPanel() override;

    void SetSeries(const QStringList& seriesNames);

    [[nodiscard]] ControllerPanelDicomItem* GetDicomItem() const;
    [[nodiscard]] ControllerPanelSphereItem* GetSphereItem() const;
    [[nodiscard]] ControllerPanelFPSOverlayItem* GetFPSOverlayItem() const;

  Q_SIGNALS:
    void DirectorySelected(const QString& directory);
    void SeriesLoadRequested(int seriesIndex);
    void SphereAddRemoveClicked();
    void SphereRadiusChanged(double radius);
    void SphereColorChanged(const QColor& color);
    void FPSOverlayEnableChanged(bool enabled);
    void FPSOverlayColorChanged(const QColor& color);
    void FPSOverlayPositionChanged(const overlays::OverlayPosition& position);
    void FPSOverlayMarginChanged(int margin);
    void FPSOverlayFontSizeChanged(int fontSize);

  private:
    void _setupUi();

    ControllerPanelDicomItem* m_dicomItem{nullptr};
    ControllerPanelSphereItem* m_sphereItem{nullptr};
    ControllerPanelFPSOverlayItem* m_fpsOverlayItem{nullptr};
};
}  // namespace ui

#endif  // CONTROLLERPANEL_HPP
