#include "diagramscene.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
QPen const DiagramScene::penForLines = QPen(QBrush(QColor(Qt::black)), 2, Qt::PenStyle::DashLine);
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent): QGraphicsScene(parent)
{
    myItemMenu = itemMenu; // 设置上下文菜单
    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    textItem = nullptr;
    myItemColor = Qt::red;
    myTextColor = Qt::green;
}

void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    foreach (QGraphicsItem* p, selectedItems()) {
        if (p->type() == DiagramTextItem::Type) {
            DiagramTextItem* item = qgraphicsitem_cast<DiagramTextItem*>(p);
            item->setDefaultTextColor(myTextColor);
        }
    }
}

void DiagramScene::setFont(const QFont &font)
{
    myFont = font;
    foreach (QGraphicsItem* p, selectedItems()) {
        if (p->type() == DiagramTextItem::Type) {
            DiagramTextItem* item = qgraphicsitem_cast<DiagramTextItem*>(p);
            item->setFont(myFont);
        }
    }
}

void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    foreach (QGraphicsItem* p, selectedItems()) {
        if (p->type() == DiagramItem::Type) {
            DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(p);
            item->setBrush(QBrush(myItemColor));
        }
    }
}

void DiagramScene::deleteItems(QList<QGraphicsItem*> const& items)
{
    qDebug() << "delete items" << items;
    QList<QGraphicsItem*> diagramItems;
    foreach (QGraphicsItem *item, items) {
        removeItem(item);
        delete item;
    }
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

//所有逻辑放在 DiagramTextItem::focusOutEvent 中可能会简化代码结构，但会导致代码难以维护和扩展。必须逻辑分离
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    //即使没有选中的文本或光标位置未定义，QGraphicsTextItem::textCursor() 也会返回一个有效的 QTextCursor 对象。
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);
    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    } else {
        if (item->contentIsUpdated()) {
            qDebug() << "content update ---";
            emit textChanged();
        }
    }
}

// 视图接受键盘和鼠标输入转化为场景事件，场景将每个事件又可传播给图形项
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    switch (myMode) {
    case InsertItem:
    {
        Digitem = new DiagramItem(myItemType, myItemMenu);
        Digitem->setBrush(myItemColor);
        addItem(Digitem);
        Digitem->setPos(mouseEvent->scenePos());
        qDebug() << "type: " << myItemType << " color: " << myItemColor<< "insert item at: " << mouseEvent->scenePos();
        emit itemInserted(Digitem);
        hasItemSelected = itemAt(mouseEvent->scenePos(), QTransform()) != nullptr;
        break;
    }
    case InsertText:
    {
        textItem = new DiagramTextItem();
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0); // 设置较高的 Z 值使文本显示在最上层
        textItem->setDefaultTextColor(myTextColor);
        textItem->setPos(mouseEvent->scenePos());
        addItem(textItem);
        //InsertText可能涉及编辑操作，InsertItem 和 InsertLine只需要简单的插入和显示
        connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)), this, SLOT(editorLostFocus(DiagramTextItem*)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
        emit textInserted(textItem);
        qDebug() << "text inserted at" << textItem->scenePos();
        break;
    }
    default: // 默认模式（移动项模式）又重新获得图形项

        hasItemSelected = itemAt(mouseEvent->scenePos(), QTransform()) != nullptr;
        break;
    }

    QGraphicsScene::mousePressEvent(mouseEvent); //没有找到合适的图形项处理或者还有额外的处理需要完成时调用
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == MoveItem && hasItemSelected) {
        mouseDraggingMoveEvent(mouseEvent); // 处理拖动项时的鼠标移动事件，提供粘性模式和正交辅助线功能。
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DiagramScene::clearOrthogonalLines()
{
    foreach(QGraphicsLineItem* p, orthogonalLines) {
        removeItem(p);
        delete p;
    }
    orthogonalLines.clear();
}

void DiagramScene::mouseDraggingMoveEvent(QGraphicsSceneMouseEvent* event)//移动过程中随时在对比
{
    clearOrthogonalLines();
    if ((event->buttons() & Qt::LeftButton) != 0 && selectedItems().size() == 1) {
        QGraphicsItem* itemUnderCursor = selectedItems().first(); // 获取当前选中的项
        QPointF curCenter = itemUnderCursor->scenePos();
        QPointF const& mousePos = event->scenePos();
        foreach(QGraphicsItem* p, items()) {
            if (p->type() != DiagramItem::Type || p == itemUnderCursor) continue;
            DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(p);
            QPointF const& objPoint = item->scenePos();
            LineAttr lineAttr;
            //项的位置 找到的项的位置 鼠标的位置
            tryEnteringStickyMode(itemUnderCursor, objPoint, mousePos);
            if ((lineAttr = getPointsRelationship(objPoint, curCenter)) != Other) {
                if ((lineAttr & Horizontal) != 0) {
                    QGraphicsLineItem* newHLine = new QGraphicsLineItem();
                    newHLine->setLine(QLineF(QPointF(0, objPoint.y()),QPointF(sceneRect().width(), objPoint.y())));
                    newHLine->setPen(penForLines);
                    orthogonalLines.append(newHLine);
                }
                if ((lineAttr & Vertical) != 0) {
                    QGraphicsLineItem* newVLine = new QGraphicsLineItem();
                    newVLine->setLine(QLineF(QPointF(objPoint.x(), 0),QPointF(objPoint.x(), sceneRect().height())));
                    newVLine->setPen(penForLines);
                    orthogonalLines.append(newVLine);
                }
            }
        }
        tryLeavingStickyMode(itemUnderCursor, mousePos);
    }
    foreach(QGraphicsLineItem* p, orthogonalLines) {
        addItem(p);
    }
}

void DiagramScene::tryEnteringStickyMode(QGraphicsItem* item, const QPointF& target,const QPointF& mousePos)
{
    QPointF const& itemPos = item->scenePos();
    if (!verticalStickyMode) {
        if (closeEnough(itemPos.x(), target.x(), stickyDistance)) {
            verticalStickyMode = true;
            verticalStickPoint = mousePos;
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setPos(QPointF(target.x(), itemPos.y()));
        }
    }
    if (!horizontalStickyMode) {
        if (closeEnough(itemPos.y(), target.y(), stickyDistance)) {
            horizontalStickyMode = true;
            horizontalStickPoint = mousePos;
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setPos(QPointF(itemPos.x(), target.y()));
        }
    }
}

void DiagramScene::tryLeavingStickyMode(QGraphicsItem* item, const QPointF& mousePos)
{
    if (verticalStickyMode) {
        item->moveBy(0, mousePos.y() - verticalStickPoint.y());
        verticalStickPoint.setY(mousePos.y());
        if (!closeEnough(mousePos.x(), verticalStickPoint.x(), stickyDistance)) {
            verticalStickyMode = false;
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
    }
    if (horizontalStickyMode) {
        item->moveBy(mousePos.x() - horizontalStickPoint.x(), 0);
        horizontalStickPoint.setX(mousePos.x());
        if (!closeEnough(mousePos.y(), horizontalStickPoint.y(), stickyDistance)) {
            horizontalStickyMode = false;
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
    }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    hasItemSelected = false;
    horizontalStickyMode = false;
    verticalStickyMode = false;
    foreach(QGraphicsItem* p, selectedItems())
        p->setFlag(QGraphicsItem::ItemIsMovable);
    clearOrthogonalLines();
    QGraphicsScene::mouseReleaseEvent(mouseEvent); // 调用基类的 mouseReleaseEvent 方法
}

void DiagramScene::wheelEvent(QGraphicsSceneWheelEvent* wheelEvent)
{
    if ((wheelEvent->modifiers() & Qt::KeyboardModifier::ControlModifier) != 0) {
        emit scaleChanging(wheelEvent->delta());
        wheelEvent->accept();
    } else {
        QGraphicsScene::wheelEvent(wheelEvent);
    }
}

bool DiagramScene::closeEnough(qreal x, qreal y, qreal delta)
{
    return std::abs(x - y) < delta;
}

DiagramScene::LineAttr DiagramScene::getPointsRelationship(const QPointF& p1,const QPointF& p2)
{
    int ret = Other;
    ret |= closeEnough(p1.x(), p2.x(), Delta) ? Vertical : Other;
    ret |= closeEnough(p1.y(), p2.y(), Delta) ? Horizontal : Other;
    return static_cast<DiagramScene::LineAttr>(ret);
}


