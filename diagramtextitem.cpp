#include "diagramtextitem.h"
#include "diagramscene.h"
#include <QDebug>
#include <QTextCursor>
DiagramTextItem::DiagramTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    positionLastTime = QPointF(0, 0);  // 初始化位置记录为 (0, 0)，用于后续比较
}

DiagramTextItem* DiagramTextItem::clone() {
    DiagramTextItem* cloned = new DiagramTextItem(nullptr);
    cloned->setPlainText(toPlainText());
    cloned->setFont(font());
    cloned->setTextWidth(textWidth());
    cloned->setDefaultTextColor(defaultTextColor());
    cloned->setPos(scenePos());
    cloned->setZValue(zValue());
    return cloned;
}

QVariant DiagramTextItem::itemChange(GraphicsItemChange change, const QVariant &value) {//选择某个文本
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}

void DiagramTextItem::focusInEvent(QFocusEvent* event) {
    qDebug() << "start editing";
    if (positionLastTime == QPointF(0, 0))  // 如果初始位置未设置就初始化为当前场景中的位置
        positionLastTime = scenePos();
    QGraphicsTextItem::focusInEvent(event);
}

void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (textInteractionFlags() == Qt::NoTextInteraction) {
        setTextInteractionFlags(Qt::TextEditorInteraction);
    }
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

//键盘焦点的改变确实可以通过鼠标点击来触发，但这并不是唯一的方式
void DiagramTextItem::focusOutEvent(QFocusEvent *event) {
    setTextInteractionFlags(Qt::NoTextInteraction);
    qDebug() << "after editing" << this;
    if (contentLastTime == toPlainText()) {
        contentHasChanged = false;
    } else {
        contentLastTime = toPlainText();
        contentHasChanged = true;
    }
    emit lostFocus(this);  // 通知外部监听者该文本项已经失去了焦点
    QGraphicsTextItem::focusOutEvent(event);
}

void DiagramTextItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << "text begin move";
    QGraphicsTextItem::mousePressEvent(event);
}

void DiagramTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (scenePos() != positionLastTime) {
        qDebug() << positionLastTime << "::" << scenePos();
        isMoved = true;
    }
    positionLastTime = scenePos();
    qDebug() << "text end moving";
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void DiagramTextItem::writeToXml(QXmlStreamWriter &xmlWriter) const {
    xmlWriter.writeStartElement("text");
    xmlWriter.writeAttribute("pos", QString("%1,%2").arg(scenePos().x()).arg(scenePos().y()));
    xmlWriter.writeAttribute("color", defaultTextColor().name());
    xmlWriter.writeAttribute("font-size", QString::number(font().pointSize()));
    xmlWriter.writeAttribute("font-family", font().family());
    xmlWriter.writeAttribute("bold", QString::number(font().bold()));
    xmlWriter.writeAttribute("italic", QString::number(font().italic()));
    xmlWriter.writeAttribute("underline", QString::number(font().underline()));
    xmlWriter.writeAttribute("text", toPlainText());
    xmlWriter.writeAttribute("zvalue", QString::number(zValue()));
    xmlWriter.writeEndElement();
}
