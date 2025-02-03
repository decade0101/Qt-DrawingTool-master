#include "diagramitem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
int DiagramItem::nextId = 0;
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), myDiagramType(diagramType), myContextMenu(contextMenu),
    myId(nextId++)
{
    QPainterPath path;
    switch (myDiagramType) {
    //并不需要显式地设置场景；当图表项被添加到场景中时，它会自动关联到该场景
    case StartEnd:
        path.moveTo(200, 50);
        path.arcTo(150, 0, 50, 50, 0, 90); // 绘制圆弧
        path.arcTo(50, 0, 50, 50, 90, 90);
        path.arcTo(50, 50, 50, 50, 180, 90);
        path.arcTo(150, 50, 50, 50, 270, 90);
        path.lineTo(200, 50);
        myPolygon = path.toFillPolygon().translated(-125, -50); //平移
        break;
    case Conditional:
        myPolygon << QPointF(-100, 0) << QPointF(0, 100) << QPointF(100, 0) << QPointF(0, -100) << QPointF(-100, 0);
        break;
    case Step:
        myPolygon << QPointF(-100, -100) << QPointF(100, -100) << QPointF(100, 100) << QPointF(-100, 100) << QPointF(-100, -100);
        break;
    default:
        myPolygon << QPointF(-120, -80) << QPointF(-70, 80) << QPointF(120, 80) << QPointF(70, -80) << QPointF(-120, -80);
        break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);//显示光标进行拖动
}

QPixmap DiagramItem::image() const//在画布上画图 画布给一个大概的大小 画图painter本来有个坐标系 将坐标系移到最中心
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);
    return pixmap;
}

QList<QPointF> DiagramItem::resizeHandlePoints() {//旁边的8个控制点
    qreal width = resizeHandlePointWidth;
    QRectF rf = QRectF(boundingRect().topLeft() + QPointF(width/2, width/2),boundingRect().bottomRight() - QPointF(width/2, width/2));
    qreal centerX = rf.center().x();
    qreal centerY = rf.center().y();
    return QList<QPointF>{rf.topLeft(), QPointF(centerX, rf.top()), rf.topRight(),QPointF(rf.left(), centerY),QPointF(rf.right(), centerY),rf.bottomLeft(), QPointF(centerX, rf.bottom()), rf.bottomRight()};
}

bool DiagramItem::isCloseEnough(QPointF const& p1, QPointF const& p2) {
    qreal delta = std::abs(p1.x() - p2.x()) + std::abs(p1.y() - p2.y());
    return delta < closeEnoughDistance;
}

DiagramItem* DiagramItem::clone() {
    DiagramItem* cloned = new DiagramItem(myDiagramType, myContextMenu, nullptr);
    cloned->myPolygon = myPolygon;
    cloned->setPolygon(myPolygon);
    cloned->setPos(scenePos()); // 图形项在场景中的坐标
    cloned->setBrush(brush());
    cloned->setZValue(zValue()); //在第几层
    return cloned;
}

void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    resizeMode = false;
    int index = 0;//记录是哪个方向被调整
    foreach (QPointF const& p, resizeHandlePoints()) {
        if (isCloseEnough(event->pos(), p)) {
            resizeMode = true;
            break;
        }
        index++;
    }
    scaleDirection = static_cast<Direction>(index);
    setFlag(GraphicsItemFlag::ItemIsMovable, !resizeMode);
    //一个准备调整大小 一个准备移动位置
    if (resizeMode) {
        qDebug() << "begin resizing";
        previousPolygon = polygon();
        event->accept(); // 用于明确表示事件已被处理，不再需要进一步传播或处理。
    } else {
        qDebug() << "222-item type " << this->type() << " start moving from" << scenePos();
        movingStartPosition = scenePos();
        QGraphicsItem::mousePressEvent(event); // 调用基类方法以确保默认行为
    }
}

void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    //一个准备调整大小 一个准备移动位置
    if (resizeMode) {
        qDebug() << "after resizing";
        if (polygon() != previousPolygon) {
            isResized = true;
        }
    } else {
        if (scenePos() != movingStartPosition) {
            isMoved = true;//第一次松开就为第二次开始备份做准备
            qDebug() <<"moving to" << scenePos(); // 打印新旧位置
        }
    }
    resizeMode = false;
    QGraphicsItem::mouseReleaseEvent(event);
}

void DiagramItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (resizeMode) {
        prepareGeometryChange(); // 准备几何变化
        myPolygon = scaledPolygon(myPolygon, scaleDirection, event->pos());
        setPolygon(myPolygon);
    }
    QGraphicsItem::mouseMoveEvent(event);
}

QPolygonF DiagramItem::scaledPolygon(const QPolygonF& old, DiagramItem::Direction direction,const QPointF& newPos) {
    qreal oldWidth = old.boundingRect().width();
    qreal oldHeight = old.boundingRect().height();
    qreal scaleWidth, scaleHeight;
    switch(direction) {
    case TopLeft: {
        QPointF fixPoint = old.boundingRect().bottomRight();
        scaleWidth = (fixPoint.x() - newPos.x()) / oldWidth;
        scaleHeight = (fixPoint.y() - newPos.y()) / oldHeight;
        break;
    }
    case Top: {
        QPointF fixPoint = old.boundingRect().bottomLeft();
        scaleWidth = 1;
        scaleHeight = (fixPoint.y() - newPos.y()) / oldHeight;
        break;
    }
    case TopRight: {
        QPointF fixPoint = old.boundingRect().bottomLeft();
        scaleWidth = (newPos.x() - fixPoint.x()) / oldWidth;
        scaleHeight = (fixPoint.y() - newPos.y()) / oldHeight;
        break;
    }
    case Left: {
        QPointF fixPoint = old.boundingRect().bottomRight();
        scaleWidth = (fixPoint.x() - newPos.x()) / oldWidth;
        scaleHeight = 1;
        break;
    }
    case Right: {
        QPointF fixPoint = old.boundingRect().bottomLeft();
        scaleWidth = (newPos.x() - fixPoint.x()) / oldWidth;
        scaleHeight = 1;
        break;
    }
    case BottomLeft: {
        QPointF fixPoint = old.boundingRect().topRight();
        scaleWidth = (fixPoint.x() - newPos.x()) / oldWidth;
        scaleHeight = (newPos.y() - fixPoint.y()) / oldHeight;
        break;
    }
    case Bottom: {
        QPointF fixPoint = old.boundingRect().topLeft();
        scaleWidth = 1;
        scaleHeight = (newPos.y() - fixPoint.y()) / oldHeight;
        break;
    }
    case BottomRight: {
        QPointF fixPoint = old.boundingRect().topLeft();
        scaleWidth = (newPos.x() - fixPoint.x()) / oldWidth;
        scaleHeight = (newPos.y() - fixPoint.y()) / oldHeight;
        break;
    }
    }
    QTransform trans;
    trans.scale(scaleWidth, scaleHeight);
    return trans.map(old); // 返回变换后的多边形
}

void DiagramItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(Qt::ArrowCursor);
    int index = 0;
        /*SizeVerCursor: ↑↓
        SizeHorCursor: ←→
        SizeBDiagCursor: ↖↘
        SizeFDiagCursor: ↗↙*/
    foreach (QPointF const& p, resizeHandlePoints()) {
        if (isCloseEnough(p, event->pos())) {
            switch (static_cast<Direction>(index)) {
            case TopLeft:setCursor(Qt::SizeBDiagCursor); break;
            case Top:setCursor(Qt::SizeVerCursor); break;
            case TopRight:setCursor(Qt::SizeFDiagCursor); break;
            case Left:setCursor(Qt::SizeHorCursor); break;
            case Right: setCursor(Qt::SizeHorCursor); break;
            case BottomLeft: setCursor(Qt::SizeFDiagCursor); break;
            case Bottom: setCursor(Qt::SizeVerCursor); break;
            case BottomRight: setCursor(Qt::SizeBDiagCursor); break;
            }
            break;
        }
        index++;
    }
    QGraphicsItem::hoverMoveEvent(event);
}

//图表项自身的 update()\场景的 update()\视图的 update()都会触发视图中可见图表项的重绘
void DiagramItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,QWidget* widget) {
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected; //删除默认的被选择效果 展示8个控制点的效果
    QGraphicsPolygonItem::paint(painter, &myOption, widget);
    if (this->isSelected()) {
        qreal width = resizeHandlePointWidth;
        foreach(QPointF const& point, resizeHandlePoints()) {
            painter->drawEllipse(QRectF(point.x() - width/2, point.y() - width/2, width, width));
        }
    }
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

void DiagramItem::writeToXml(QXmlStreamWriter &xmlWriter) const {
    xmlWriter.writeStartElement("item");
    xmlWriter.writeAttribute("Id", QString::number(myId));
    xmlWriter.writeAttribute("type", QString::number(myDiagramType));
    xmlWriter.writeAttribute("pos", QString("%1,%2").arg(scenePos().x()).arg(scenePos().y()));
    xmlWriter.writeAttribute("color", brush().color().name());
    xmlWriter.writeAttribute("zvalue", QString::number(zValue()));
    xmlWriter.writeEndElement();
}
