#ifndef WIDGET_H
#define WIDGET_H

#include <cstdint>

#include <memory>
#include <string>

#include <QWidget>
#include <QAbstractButton>
#include <QPropertyAnimation>
#include <QHBoxLayout>
#include <QRect>
#include <QRectF>

namespace xwidget{
namespace xswitch{

struct XSwitchOption{
    uint16_t slider_diameter;
    int16_t slider_margin;
    uint16_t box_width;
    uint16_t box_height;
    QBrush slider_brush;
    QBrush disable_slider_brush;
    QBrush turn_off_box_brush;
    QBrush turn_on_box_brush;
    QBrush disable_box_brush;
};

class XSwitch final : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(int slider_offset READ GetOffset WRITE SetOffset)

public:
    XSwitch(const XSwitchOption &option, QWidget *parent = 0);
    ~XSwitch();

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void enterEvent(QEvent *) override;

private:
    class XSwitchImpl;
    std::unique_ptr<XSwitchImpl> pimpl_;

    void SetOffset(int offset);
    int GetOffset() const;
    QPropertyAnimation *animation_ = nullptr;
};

XSwitchOption SimpleSwitchOption();
XSwitchOption MaterialDesignSmallSwitchOption();
XSwitchOption MaterialDesignSwitchOption();
XSwitchOption InsetSwitchOption();

}  // namespace xswitch 
}  // namespace xwidget
#endif // WIDGET_H
