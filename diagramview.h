#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H
#include <QGraphicsView>
class DiagramView : public QGraphicsView {
    Q_OBJECT
public:
    DiagramView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
signals:
    void needsUndoBackup();// 当需要备份撤销状态时发出
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};
#endif // DIAGRAMVIEW_H
