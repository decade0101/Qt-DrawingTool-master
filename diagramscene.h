#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H
#include "diagramitem.h"
#include "diagramtextitem.h"
#include <QGraphicsScene> // 包含Qt的图形场景类，本类继承自此基类
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent; // 鼠标事件类，用于处理鼠标移动、按下和释放等事件
class QMenu; // 用于创建弹出菜单
class QPointF; // 用于表示二维平面上的一个点
class QFont; // 用于设置文本项的字体属性
class QGraphicsTextItem; // 用于在场景中显示可编辑文本
class QColor; // 用于设置颜色属性
QT_END_NAMESPACE
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { InsertItem,InsertText, MoveItem };
    explicit DiagramScene(QMenu *itemMenu, QObject *parent = nullptr);
    QColor textColor() const { return myTextColor; }
    QColor itemColor() const { return myItemColor; }
    QFont font() const { return myFont; }
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);//图形项填充色
    void setFont(const QFont &font);
    void deleteItems(QList<QGraphicsItem*> const& items);
public slots:
    void setMode(Mode mode);
    void setItemType(DiagramItem::DiagramType type);//插入的图形项的类型
    void editorLostFocus(DiagramTextItem *item);
signals:
    void itemInserted(DiagramItem *item);
    void itemSelected(QGraphicsItem *item);
    void textInserted(DiagramTextItem *item);
    void textChanged();
    void arrowInserted();
    void scaleChanging(int delta);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void wheelEvent(QGraphicsSceneWheelEvent* wheelEvent) override;
private:
    void mouseDraggingMoveEvent(QGraphicsSceneMouseEvent* event); // 处理拖动中的鼠标移动事件
    void clearOrthogonalLines(); // 清除正交辅助线
    inline bool closeEnough(qreal x, qreal y, qreal delta); // 判断两点是否足够接近
    enum LineAttr { Other = 0, Horizontal, Vertical, Both}; // 线条方向属性枚举
    LineAttr getPointsRelationship(QPointF const& p1, QPointF const& p2); // 获取两点间的关系（水平或垂直）
    void tryEnteringStickyMode(QGraphicsItem* item, QPointF const& target, QPointF const& mousePos); // 尝试进入粘性模式
    void tryLeavingStickyMode(QGraphicsItem* item, QPointF const& mousePos); // 尝试离开粘性模式
    DiagramItem::DiagramType myItemType; // 当前要插入项的类型
    QMenu *myItemMenu; // 项的上下文菜单
    Mode myMode; // 当前操作模式
    QPointF startPoint; // 拖动开始点
    QFont myFont; // 场景使用的字体
    DiagramItem *Digitem;
    DiagramTextItem *textItem; // 当前编辑的文本项
    QColor myTextColor; // 场景中文本的颜色
    QColor myItemColor; // 场景中图形项的颜色
    bool horizontalStickyMode = false; // 是否处于水平粘性模式
    bool verticalStickyMode = false; // 是否处于垂直粘性模式
    QPointF horizontalStickPoint; // 水平粘性模式下的参考点
    QPointF verticalStickPoint; // 垂直粘性模式下的参考点
    QList<QGraphicsLineItem*> orthogonalLines; // 正交辅助线列表
    bool hasItemSelected = false; // 是否有项被选中
    static const QPen penForLines; // 绘制正交辅助线的画笔
    static constexpr qreal Delta = 0.1; // 判断两点接近程度的阈值
    static constexpr qreal stickyDistance = 5; // 粘性模式的距离阈值
    DiagramItem *startPointItem; // 起点项引用
};

#endif // DIAGRAMSCENE_H
