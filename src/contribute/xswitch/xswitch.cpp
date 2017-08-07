#include "xswitch.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

namespace xwidget{
namespace xswitch{

class XSwitch::XSwitchImpl {
public:
    XSwitchImpl(const XSwitchOption &option)
        : option_(option) {

        size_hint_ = SizeHint();
        box_rect_ = QRect(option_.slider_margin < 0 ? -option_.slider_margin : 0,
                                          (size_hint_.height() - option_.box_height) / 2,
                                          option_.box_width, option_.box_height);
        slider_rect_ = QRectF(option_.slider_margin > 0 ? option_.slider_margin : 0,
                                       (size_hint_.height() - option_.slider_diameter) / 2,
                                       option_.slider_diameter, option_.slider_diameter);
        slide_end_ = option_.box_width - option_.slider_margin * 2 - option_.slider_diameter;
    }

    QSize SizeHint() {
        int w = std::max(static_cast<int>(option_.box_width),
                         option_.box_width - option_.slider_margin * 2);
        int h = std::max(option_.slider_diameter, option_.box_height);
        return QSize(w, h);
    }

    XSwitchOption option_;
    int slider_offset_= 0;
    QSize size_hint_;
    QRect box_rect_;
    QRectF slider_rect_;
    uint16_t animation_duration_ = 100;
    int slide_start_ = 0;
    int slide_end_ = 0;
};

XSwitch::XSwitch(const XSwitchOption &option, QWidget *parent)
    : QAbstractButton(parent),
      pimpl_(new XSwitchImpl(option)),
      animation_(new QPropertyAnimation(this, "slider_offset", this))
{
    setCheckable(true);
    animation_->setDuration(pimpl_->animation_duration_);
    connect(this, &XSwitch::toggled, this, [=](bool ){
            Animation();
            });
}

XSwitch::~XSwitch() = default;

int XSwitch::GetOffset() const {
    return pimpl_->slider_offset_;
}

void XSwitch::SetOffset(int offset) {
    pimpl_->slider_offset_ = offset;
    update();
}

void XSwitch::paintEvent(QPaintEvent */*e*/) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QBrush box_brush;
    QBrush slider_brush;
    if (isEnabled()) {
        box_brush = isChecked() ? pimpl_->option_.turn_on_box_brush : pimpl_->option_.turn_off_box_brush;
        slider_brush = pimpl_->option_.slider_brush;
    } else {
        box_brush = pimpl_->option_.disable_box_brush;
        slider_brush = pimpl_->option_.disable_slider_brush;
        pimpl_->slider_offset_ = isChecked() ?
            pimpl_->slide_end_ : pimpl_->slide_start_;
    }
    painter.setBrush(box_brush);
    painter.drawRoundedRect(pimpl_->box_rect_, pimpl_->option_.box_height/2, pimpl_->option_.box_height/2);
    painter.setBrush(slider_brush);
    painter.drawEllipse(QRectF(pimpl_->slider_rect_.x() + pimpl_->slider_offset_,
                               pimpl_->slider_rect_.y(),
                               pimpl_->slider_rect_.width(),
                               pimpl_->slider_rect_.height()));
}

void XSwitch::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() & Qt::LeftButton) {
        QAbstractButton::mouseReleaseEvent(e);
    }

}

void XSwitch::Animation() {
        animation_->stop();
        animation_->setStartValue(GetOffset());

        if (isChecked()) {
            animation_->setEndValue(pimpl_->slide_end_);
        } else {
            animation_->setEndValue(pimpl_->slide_start_);
        }

        animation_->start();
}

void XSwitch::enterEvent(QEvent *e) {
    setCursor(Qt::PointingHandCursor);
    QAbstractButton::enterEvent(e);
}

QSize XSwitch::sizeHint() const {
    return pimpl_->SizeHint();
}

XSwitchOption SimpleSwitchOption() {
    return XSwitchOption{20,
                        0,
                        40,
                        20,
                        QBrush(QColor(32, 147, 238, 255)),
                        QBrush(QColor(100, 100, 100, 255)),
                        QBrush(QColor(221, 221, 221, 255)),
                        QBrush(QColor(32, 147, 238, 100)),
                        QBrush(QColor(200, 200, 200, 255))};
}

XSwitchOption MaterialDesignSmallSwitchOption() {
    return XSwitchOption{20,
                        0,
                        40,
                        20,
                        QBrush(QColor("#000000")),
                        QBrush(QColor("#0000ff")),
                        QBrush(QColor(0, 255, 255, 50)),
                        QBrush(QColor("#ff00ff")),
                        QBrush(QColor("#ffff00"))};
}

XSwitchOption MaterialDesignSwitchOption() {
    return XSwitchOption{20,
                        0,
                        40,
                        20,
                        QBrush(QColor("#000000")),
                        QBrush(QColor("#0000ff")),
                        QBrush(QColor(0, 255, 255, 50)),
                        QBrush(QColor("#ff00ff")),
                        QBrush(QColor("#ffff00"))};
}

XSwitchOption InsetSwitchOption() {
    return XSwitchOption{20,
                        0,
                        40,
                        20,
                        QBrush(QColor("#000000")),
                        QBrush(QColor("#0000ff")),
                        QBrush(QColor(0, 255, 255, 50)),
                        QBrush(QColor("#ff00ff")),
                        QBrush(QColor("#ffff00"))};
}

} //namespace xswitch
} //namespace xwidget
