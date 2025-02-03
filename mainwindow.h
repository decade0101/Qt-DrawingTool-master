#ifndef MAINWINDOW_H  // 预处理器指令，防止头文件重复包含
#define MAINWINDOW_H  // 定义宏，用于条件编译
#include "diagramitem.h"  // 包含自定义项类的头文件
#include <QMainWindow>    // 包含 QMainWindow 类的头文件
#include "qlabel.h"
#include "undosystem.h"   // 包含撤销系统类的头文件
class DiagramScene;  // 声明 DiagramScene 类（场景类
QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;     // 工具箱类
class QSpinBox;     //整数输入框
class QComboBox;    // 下拉列表框
class QFontComboBox;// 字体选择框
class QButtonGroup; // 按钮组
class QLineEdit;    // 单行文本编辑框
class QGraphicsTextItem;
class QFont;
class QToolButton;  // 工具按钮
class QAbstractButton; // 抽象按钮基类
class QGraphicsView;   // 视图类
QT_END_NAMESPACE
class MainWindow : public QMainWindow  // 定义 MainWindow 类，继承自 QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private slots:
    void backgroundButtonGroupClicked(QAbstractButton *button); // 背景按钮组点击事件处理
    void buttonGroupClicked(int id);                             // 按钮组点击事件处理
    void copyItem();                                             // 复制项
    void pasteItem();                                            // 粘贴项
    void cutItem();                                              // 剪切项
    void deleteItem();                                           // 删除项
    void undo();                                                 // 撤销操作
    void redo();                                                 // 重做操作
    void groupItems();                                           // 分组项
    void ungroupItems();                                         // 解散分组项
    void pointerGroupClicked(int id);                            // 类型按钮组点击事件处理
    void bringToFront();                                         // 将项置于最前
    void sendToBack();                                           // 将项置于最后
    void itemInserted(DiagramItem *item);                        // 项插入事件处理
    void textInserted(QGraphicsTextItem *item);                  // 文本项插入事件处理
    void backupUndostack();                                      // 备份撤销栈
    void currentFontChanged(const QFont &font);                  // 当前字体变化事件处理
    void fontSizeChanged(const QString &size);                   // 字体大小变化事件处理
    void sceneScaleChanged(const QString &scale);                // 场景缩放变化事件处理
    void sceneScaleZooming(int delta);                           // 场景缩放滚轮事件处理
    void textColorChanged();                                     // 文本颜色变化事件处理
    void itemColorChanged();                                     // 项颜色变化事件处理
    void textButtonTriggered();                                  // 文本按钮触发事件处理
    void fillButtonTriggered();                                  // 填充按钮触发事件处理
    void handleFontChange();                                     // 处理字体变化
    void itemSelected(QGraphicsItem *item);                      // 项选中事件处理
    void about();                                                // 关于对话框显示
    void onFileImport(); // 新增：处理导入操作
    void onFileExport(); // 新增：处理导出操作
private:
    void createToolBox();                                        // 创建工具箱
    void createActions();                                        // 创建所有动作
    void createMenus();                                          // 创建菜单栏
    void createToolbars();                                       // 创建工具栏
    QWidget *createBackgroundCellWidget(const QString &text, const QString &image); // 创建背景单元格小部件
    QWidget *createCellWidget(const QString &text, DiagramItem::DiagramType type); // 创建单元格小部件
    QMenu *createColorMenu(const char *slot, QColor defaultColor); // 创建颜色菜单
    QIcon createColorToolButtonIcon(const QString &image, QColor color); // 创建带颜色的工具按钮图标
    QIcon createColorIcon(QColor color);                         // 创建纯色图标
    QList<QGraphicsItem*> cloneItems(QList<QGraphicsItem*> const& items); // 克隆图形项列表
    DiagramScene *scene;                                         // 场景对象
    QGraphicsView *view;                                         // 视图对象
    QList<QGraphicsItem*> pasteBoard;                            // 粘贴板项列表
    UndoSystem undoStack;                                        // 撤销系统对象
    QAction *exitAction;                                         // 退出动作
    QAction *addAction;                                          // 添加项动作
    QAction *deleteAction;                                       // 删除项动作
    QAction *copyAction;                                         // 复制项动作
    QAction *pasteAction;                                        // 粘贴项动作
    QAction *cutAction;                                          // 剪切项动作
    QAction *undoAction;                                         // 撤销动作
    QAction *redoAction;                                         // 重做动作
    QAction *toFrontAction;                                      // 置顶项动作
    QAction *sendBackAction;                                     // 置底项动作
    QAction *groupAction;                                        // 分组项动作
    QAction *ungroupAction;                                      // 解散分组项动作
    QAction *aboutAction;                                        // 关于动作
    QAction *importAction;
    QAction *exportAction;
    QMenu *fileMenu;                                             // 文件菜单
    QMenu *itemMenu;                                             // 项菜单
    QMenu *aboutMenu;                                            // 关于菜单
    QToolBar *textToolBar;                                       // 文本工具栏
    QToolBar *editToolBar;                                       // 编辑工具栏
    QToolBar *colorToolBar;                                      // 颜色工具栏
    QToolBar *pointerToolbar;                                    // 工具栏
    QComboBox *sceneScaleCombo;                                  // 场景缩放下拉列表
    QComboBox *itemColorCombo;                                   // 项颜色下拉列表
    QComboBox *textColorCombo;                                   // 文本颜色下拉列表
    QComboBox *fontSizeCombo;                                    // 字体大小下拉列表
    QFontComboBox *fontCombo;                                    // 字体选择框
    QToolBox *toolBox;                                           // 工具箱
    QButtonGroup *buttonGroup;                                   // 按钮组
    QButtonGroup *pointerTypeGroup;                              // 类型按钮组
    QButtonGroup *backgroundButtonGroup;                         // 背景按钮组
    QToolButton *fontColorToolButton;                            // 字体颜色工具按钮
    QToolButton *fillColorToolButton;                            // 填充颜色工具按钮
    QToolButton *pointerButton;
    QAction *boldAction;                                         // 加粗文本动作
    QAction *underlineAction;                                    // 下划线文本动作
    QAction *italicAction;                                       // 斜体文本动作
    QAction *textAction;                                         // 文本动作
    QAction *fillAction;                                         // 填充动作
    QLabel* percentLabel;
};

#endif // MAINWINDOW_H  // 结束宏定义，防止重复包含
