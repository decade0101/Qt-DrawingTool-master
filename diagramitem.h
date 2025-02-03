#ifndef DIAGRAMITEM_H // 预处理器指令，防止头文件重复包含
#define DIAGRAMITEM_H // 定义宏，用于上述预防措施
#include "qxmlstream.h"
#include <QGraphicsPixmapItem> // 用于处理图片项
#include <QList>               // 用于存储多个元素
// 前向声明那些只用到指针或引用的 Qt 类
QT_BEGIN_NAMESPACE
class QPixmap; // 用于处理图像数据
class QGraphicsItem; //作为所有图形项的基类
class QGraphicsScene; //用于管理图形项
class QTextEdit; // 用于文本编辑器部件
class QGraphicsSceneMouseEvent; // 鼠标事件类，特定于图形场景
class QMenu; //用于上下文菜单
class QGraphicsSceneContextMenuEvent; //特定于图形场景
class QPainter; // 用于绘制操作（在 .cpp 文件中需要完整定义）
class QStyleOptionGraphicsItem; //用于图形项的绘制
class QWidget; // 作为所有窗口小部件的基类
class QPolygonF; //用于多边形几何形状
QT_END_NAMESPACE
class Arrow; // 前向声明 Arrow 类，因为 DiagramItem 只持有指向 Arrow 的指针
class DiagramItem : public QGraphicsPolygonItem//用于表示图表中的不同元素
{
    friend class DiagramView; // 允许 DiagramView 访问 DiagramItem 的私有成员
public:
    enum DiagramType { Step, Conditional, StartEnd, Io };
    //虽然DiagramType和Type表示的是同一个图表项，但它们的用途和应用场景不同
    DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = nullptr);
    enum { Type = UserType + 15 };
    enum Direction { TopLeft = 0, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };
    DiagramType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    QPixmap image() const;
    int type() const override { return Type; }
    QList<QPointF> resizeHandlePoints();
    bool isCloseEnough(QPointF const& p1, QPointF const& p2);
    DiagramItem* clone();
    void writeToXml(QXmlStreamWriter &xmlWriter) const;
    int getId() const { return myId; }
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
private:
    QPolygonF scaledPolygon(QPolygonF const& old, Direction direction, QPointF const& newPos);// 根据新的位置缩放多边形
    DiagramType myDiagramType;      // 图表项的类型
    QPolygonF myPolygon;            // 图表项的多边形形状
    QMenu *myContextMenu;           // 图表项的上下文菜单
    QList<Arrow *> arrows;          // 关联到此图表项的所有箭头列表
    static constexpr qreal resizeHandlePointWidth = 5; // 调整手柄点的宽度
    static constexpr qreal closeEnoughDistance = 5;   // 点之间被认为是“足够接近”的距离
    bool resizeMode = false;        // 是否处于调整模式
    Direction scaleDirection;       // 当前调整的方向
    QPointF movingStartPosition;    // 开始移动的位置
    bool isMoved = false;           // 是否已经移动过
    QPolygonF previousPolygon;      // 上一个多边形形状，用于撤销操作
    bool isResized = false;         // 是否已经调整过大小
    static int nextId; // 下一个可用的唯一ID
    int myId;          // 当前对象的唯一ID
};
#endif // DIAGRAMITEM_H // 结束预处理器指令，防止头文件重复包含
