#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H
#include "qxmlstream.h"
#include <QGraphicsTextItem>  // 用于显示和编辑文本
#include <QPen>               //用于定义绘制时使用的笔样式
QT_BEGIN_NAMESPACE
class QFocusEvent;             //用于处理焦点事件
class QGraphicsItem;           //是所有图形项的基类
class QGraphicsScene;          //表示一个图形场景
class QGraphicsSceneMouseEvent;//用于处理图形场景中的鼠标事件
QT_END_NAMESPACE
class DiagramTextItem : public QGraphicsTextItem//用于创建可编辑的文本项
{
    Q_OBJECT
public:
    DiagramTextItem(QGraphicsItem *parent = nullptr);
    enum { Type = UserType + 3 };
    int type() const override { return Type; }
    bool contentIsUpdated() { return contentHasChanged; }
    bool positionIsUpdated() { return isMoved; }
    void setUpdated() { isMoved = false; }// 提供设置方法，重置更新标志，表示项的状态已被同步
    DiagramTextItem* clone();
    void writeToXml(QXmlStreamWriter &xmlWriter) const ;
signals:
    void lostFocus(DiagramTextItem *item);
    void selectedChange(QGraphicsItem *item);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString contentLastTime;   // 记录上次保存的内容，用于检测内容是否发生变化
    QPointF positionLastTime;  // 记录上次保存的位置，用于检测位置是否发生变化
    bool isMoved = false;      // 标记项是否被移动过，初始值为 false
    bool contentHasChanged = false; // 标记内容是否已更改，初始值为 false
};
#endif // DIAGRAMTEXTITEM_H
