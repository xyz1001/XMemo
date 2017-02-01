#include "MemoInfo.h"
#include "DbOperator.h"
#include <QDateTime>
#include <QDebug>

MemoInfo::MemoInfo()
{
    this->id = QDateTime::currentMSecsSinceEpoch()/1000;
    this->color = Qt::yellow;
    this->memoWidget = nullptr;
    this->visibility = true;
}

MemoInfo::MemoInfo(unsigned int id, QString content, Qt::GlobalColor color, bool visibility, QPoint pos)
    :id(id), content(content), color(color), visibility(visibility), pos(pos)
{
    this->memoWidget = nullptr;
}

MemoInfo::~MemoInfo()
{
    delete memoWidget;
    memoWidget = nullptr;
}

QString MemoInfo::getDate() const
{
    return QDateTime::fromTime_t(id).toString("yyyy/MM/dd");
}

unsigned int MemoInfo::getId() const
{
    return id;
}

QString MemoInfo::getContent() const
{
    return content;
}

void MemoInfo::setContent(const QString &content)
{
    this->content = content;
}

Qt::GlobalColor MemoInfo::getColor() const
{
    return color;
}

void MemoInfo::setColor(Qt::GlobalColor color)
{
    this->color = color;
}

QPoint MemoInfo::getPosition() const
{
    return this->pos;
}

void MemoInfo::setPosition(QPoint pos)
{
    this->pos = pos;
}

MemoWidget *MemoInfo::getMemoWidget() const
{
    return memoWidget;
}

bool MemoInfo::isVisible() const
{
    return visibility;
}

void MemoInfo::createWidget(bool isEditMode)
{
    this->memoWidget = new MemoWidget(this, isEditMode);
    this->visibility = true;
    connect(memoWidget, &MemoWidget::memoChanged, this, &MemoInfo::onMemoChanged);
    connect(memoWidget, &MemoWidget::memoMoved, this, &MemoInfo::onMemoMoved);
}

void MemoInfo::removeWidget()
{
    this->visibility = false;
    delete memoWidget;
    memoWidget = nullptr;
}


void MemoInfo::onMemoChanged()
{
    this->content = this->memoWidget->getContent();
    this->color = this->memoWidget->getColor();

    DbOperator::getInstance().modifyContent(*this);
    DbOperator::getInstance().modifyColor(*this);

    emit memoChanged(this->id);
}

void MemoInfo::onMemoMoved(QPoint point)
{
    this->pos = point;
    DbOperator::getInstance().modifyPosition(*this);
}
