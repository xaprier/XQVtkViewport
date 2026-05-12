#include "ui/ControllerPanel.hpp"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

#include "ui/ControllerPanelDicomItem.hpp"
#include "ui/ControllerPanelFPSOverlayItem.hpp"
#include "ui/ControllerPanelSphereItem.hpp"

namespace ui {

ControllerPanel::ControllerPanel(QWidget* parent)
    : QWidget(parent) {
    _setupUi();
}

ControllerPanel::~ControllerPanel() = default;

void ControllerPanel::SetSeries(const QStringList& seriesNames) {
    m_dicomItem->SetSeries(seriesNames);
}

ControllerPanelDicomItem* ControllerPanel::GetDicomItem() const {
    return m_dicomItem;
}

ControllerPanelSphereItem* ControllerPanel::GetSphereItem() const {
    return m_sphereItem;
}

ControllerPanelFPSOverlayItem* ControllerPanel::GetFPSOverlayItem() const {
    return m_fpsOverlayItem;
}

void ControllerPanel::_setupUi() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(8);

    m_dicomItem = new ControllerPanelDicomItem(this);
    m_sphereItem = new ControllerPanelSphereItem(this);
    m_fpsOverlayItem = new ControllerPanelFPSOverlayItem(this);

    auto makeSep = [this]() -> QFrame* {
        auto* sep = new QFrame(this);
        sep->setFrameShape(QFrame::HLine);
        sep->setFrameShadow(QFrame::Sunken);
        return sep;
    };

    layout->addWidget(m_dicomItem);
    layout->addWidget(makeSep());
    layout->addWidget(m_sphereItem);
    layout->addWidget(makeSep());
    layout->addWidget(m_fpsOverlayItem);
    layout->addStretch(1);

    connect(m_dicomItem, &ControllerPanelDicomItem::DirectorySelected,
            this, &ControllerPanel::DirectorySelected);
    connect(m_dicomItem, &ControllerPanelDicomItem::SeriesLoadRequested,
            this, &ControllerPanel::SeriesLoadRequested);
    connect(m_sphereItem, &ControllerPanelSphereItem::SphereAddRemoveClicked,
            this, &ControllerPanel::SphereAddRemoveClicked);
    connect(m_sphereItem, &ControllerPanelSphereItem::SphereRadiusChanged,
            this, &ControllerPanel::SphereRadiusChanged);
    connect(m_sphereItem, &ControllerPanelSphereItem::SphereColorChanged,
            this, &ControllerPanel::SphereColorChanged);
    connect(m_fpsOverlayItem, &ControllerPanelFPSOverlayItem::FPSOverlayEnableChanged,
            this, &ControllerPanel::FPSOverlayEnableChanged);
    connect(m_fpsOverlayItem, &ControllerPanelFPSOverlayItem::FPSOverlayColorChanged,
            this, &ControllerPanel::FPSOverlayColorChanged);
    connect(m_fpsOverlayItem, &ControllerPanelFPSOverlayItem::FPSOverlayPositionChanged,
            this, &ControllerPanel::FPSOverlayPositionChanged);
    connect(m_fpsOverlayItem, &ControllerPanelFPSOverlayItem::FPSOverlayMarginChanged,
            this, &ControllerPanel::FPSOverlayMarginChanged);
    connect(m_fpsOverlayItem, &ControllerPanelFPSOverlayItem::FPSOverlayFontSizeChanged,
            this, &ControllerPanel::FPSOverlayFontSizeChanged);
}

}  // namespace ui
