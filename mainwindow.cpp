#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "diagramview.h"
#include "mainwindow.h"

#include <QtWidgets>

const int InsertTextButton = 10;

// 构造函数，初始化主窗口及其组件
MainWindow::MainWindow() {
    createActions();          // 创建所有动作（菜单项、工具栏按钮等）但只是纯创建
    createToolBox();
    createMenus();
    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)), this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)), this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(arrowInserted()), this, SLOT(backupUndostack()));
    connect(scene, SIGNAL(textChanged()), this, SLOT(backupUndostack()));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(itemSelected(QGraphicsItem*)));
    connect(scene, SIGNAL(scaleChanging(int)), this, SLOT(sceneScaleZooming(int)));
    createToolbars();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new DiagramView(scene);
    layout->addWidget(view);
    connect(view, SIGNAL(needsUndoBackup()), this, SLOT(backupUndostack()));

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle(tr("Diagramscene"));
    setUnifiedTitleAndToolBarOnMac(true);

    undoStack.backup(QList<QGraphicsItem*>());
    setFixedSize(2500, 1500);
}

void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button) {
    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));
    scene->update();
    view->update();
}

void MainWindow::buttonGroupClicked(int id) {
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    QAbstractButton* clickedButton = buttonGroup->button(id);
    foreach (QAbstractButton *button, buttons) {
        if (clickedButton != button)
            button->setChecked(false);
    }
    if (!clickedButton->isChecked()) {
        scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
        return;
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}

void MainWindow::copyItem() {
    foreach(QGraphicsItem* p, pasteBoard) {
        delete p;
    }
    pasteBoard = cloneItems(scene->selectedItems());//可能不止一项
}

void MainWindow::pasteItem() {
    if (!scene->selectedItems().empty())
        undoStack.backup(cloneItems(scene->items()));
    //因为swap 每次粘贴操作都会创建新的对象实例，并且这些实例在实质上是不同的
    QList<QGraphicsItem*> pasteBoardCopy(cloneItems(pasteBoard));
    foreach(QGraphicsItem* p, scene->items())
        p->setSelected(false);
    foreach(QGraphicsItem* item, pasteBoard) {
        item->setPos(item->scenePos() + QPointF(20, 20));
        item->setZValue(item->zValue() + 0.1);
        scene->addItem(item);
        item->setSelected(true);
    }
    pasteBoard.swap(pasteBoardCopy);
}

void MainWindow::cutItem() {
    copyItem();
    deleteItem();
}

void MainWindow::deleteItem() {
    bool needsBackup = !scene->selectedItems().empty();
    if (needsBackup)
        undoStack.backup(cloneItems(scene->items()));
    scene->deleteItems(scene->selectedItems());
}

void MainWindow::undo() {
    if (undoStack.isEmpty()) return;
    scene->deleteItems(scene->items());
    QList<QGraphicsItem*> undoneItems = cloneItems(undoStack.undo());
    foreach(QGraphicsItem* item, undoneItems) {
        scene->addItem(item);
    }
}

void MainWindow::redo() {
    if (undoStack.isFull()) return;
    scene->deleteItems(scene->items());
    QList<QGraphicsItem*> redoneItems = cloneItems(undoStack.redo());
    foreach(QGraphicsItem* item, redoneItems) {
        scene->addItem(item);
    }
}

void MainWindow::groupItems() {
    QGraphicsItemGroup* group = scene->createItemGroup(scene->selectedItems());
    group->setFlag(QGraphicsItem::ItemIsMovable, true);
    group->setFlag(QGraphicsItem::ItemIsSelectable, true);
    group->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    scene->addItem(group);
    backupUndostack();
}

void MainWindow::ungroupItems() {
    foreach(QGraphicsItem* p, scene->selectedItems()) {
        if (p->type() == QGraphicsItemGroup::Type) {
            scene->destroyItemGroup(qgraphicsitem_cast<QGraphicsItemGroup*>(p));
        }
    }
    backupUndostack();
}

void MainWindow::pointerGroupClicked(int) {
    foreach(QAbstractButton* b, buttonGroup->buttons()) {
        b->setChecked(false);
    }
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::bringToFront() {
    if (scene->selectedItems().isEmpty())
        return;
    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();
    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
    backupUndostack();
}

void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;
    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();
    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
    backupUndostack();
}

void MainWindow::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
    backupUndostack();
}

void MainWindow::textInserted(QGraphicsTextItem *)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(InsertTextButton)->setChecked(false);
}

void MainWindow::backupUndostack() {
    undoStack.backup(cloneItems(scene->items()));//这里的克隆是深拷贝
}

void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}
void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}

// sceneScaleChanged() 函数用于更改视图的比例尺。
void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);//似乎并没有怎么保持原来的位置
}

// sceneScaleZooming() 函数用于处理滚轮缩放事件。
void MainWindow::sceneScaleZooming(int delta) {
    int changingPercent = delta > 0 ? 10 : -10;
    QString comboText = sceneScaleCombo->currentText();
    int newScale = comboText.toInt() + changingPercent;
    if (newScale > 0 && newScale <= 200) {
        // 更新组合框中的比例文本。
        sceneScaleCombo->setCurrentText(QString().number(newScale));
    }
}

void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon( ":/images/textpointer.png",qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
    undoStack.backup(cloneItems(scene->items()));
}

void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(":/images/floodfill.png",qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
    undoStack.backup(cloneItems(scene->items()));
}

void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}

void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());
    scene->setFont(font);
}

void MainWindow::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =qgraphicsitem_cast<DiagramTextItem *>(item);//传信号时明显文字被选择
    QFont font = textItem->font();
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Diagram Scene"), tr("A drawing tool based on Qt Example."));
}

void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step), 0, 1);
    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 1, 0);
    layout->addWidget(createCellWidget(tr("Start/End"), DiagramItem::StartEnd), 1, 1);
    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 2, 0);
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);
    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));
    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),":/images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),":/images/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),":/images/background4.png"), 1, 1);
    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);
    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}

void MainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"), tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));
    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));
    boldAction = new QAction(QIcon(":/images/bold.png"),tr("Bold"), this);
    boldAction->setCheckable(true);
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));
    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));
    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));
    copyAction = new QAction(QIcon(":/images/copy.png"), tr("C&opy"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("Copy items from diagram"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyItem()));
    pasteAction = new QAction(QIcon(":/images/paste.png"), tr("P&aste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("Paste items from clipboard to diagram"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(pasteItem()));
    cutAction = new QAction(QIcon(":/images/cut.png"), tr("C&ut"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Cut items from diagram"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cutItem()));
    undoAction = new QAction(QIcon(":images/undo.png"), tr("U&ndo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setStatusTip(tr("Undo last operation"));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    redoAction = new QAction(QIcon(":images/redo.png"), tr("R&edo"), this);
    redoAction->setShortcut(tr("Ctrl+Shift+Z"));
    redoAction->setStatusTip(tr("Redo last operation"));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));
    groupAction = new QAction(QIcon(":images/group.png"), tr("G&roup"), this);
    groupAction->setStatusTip(tr("Group graphic items"));
    connect(groupAction, SIGNAL(triggered()), this, SLOT(groupItems()));
    ungroupAction = new QAction(QIcon(":images/ungroup.png"), tr("U&ngroup"), this);
    ungroupAction->setStatusTip(tr("Ungroup graphic items"));
    connect(ungroupAction, SIGNAL(triggered()), this, SLOT(ungroupItems()));
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    importAction = new QAction(QIcon(":/images/import.png"), tr("&Import"), this);
    importAction->setShortcut(tr("Ctrl+I"));
    importAction->setStatusTip(tr("Import diagram from XML file"));
    connect(importAction, SIGNAL(triggered()), this, SLOT(onFileImport()));
    exportAction = new QAction(QIcon(":images/export.png"), tr("&Export"), this);
    exportAction->setShortcut(tr("Ctrl+E"));
    exportAction->setStatusTip(tr("Export diagram to XML file"));
    connect(exportAction, SIGNAL(triggered()), this, SLOT(onFileExport()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);
    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(copyAction);
    itemMenu->addAction(cutAction);
    itemMenu->addAction(pasteAction);
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(undoAction);
    itemMenu->addAction(redoAction);
    itemMenu->addSeparator();
    itemMenu->addAction(groupAction);
    itemMenu->addAction(ungroupAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);
    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
    fileMenu->addAction(importAction);
    fileMenu->addAction(exportAction);
}

void MainWindow::createToolbars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(copyAction);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);
    editToolBar->addAction(groupAction);
    editToolBar->addAction(ungroupAction);
    editToolBar->addAction(importAction);
    editToolBar->addAction(exportAction);
    removeToolBar(editToolBar);
    addToolBar(Qt::LeftToolBarArea, editToolBar);
    editToolBar->show();
    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)), this, SLOT(currentFontChanged(QFont)));
    fontCombo->setEditable(false);
    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);// 为字体大小组合框设置验证器，限制输入范围
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(fontSizeChanged(QString)));
    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);// 设置工具按钮的弹出模式为 MenuButtonPopup，使得按钮可以显示一个下拉菜单，同时可以在点击按钮时不立即关闭菜单，允许用户进行更多交互。
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));// 为工具按钮设置一个颜色选择菜单。该菜单允许用户选择不同的文本颜色。选择一种颜色时，会触发 textColorChanged() 槽函数来处理颜色变化。默认颜色设置为黑色。
    textAction = fontColorToolButton->menu()->defaultAction();// 获取菜单中的默认动作（通常是第一个动作），并将其存储在 textAction 中，以便后续操作或状态跟踪使用。
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);// 启用工具按钮的自动填充背景功能，确保按钮能够正确显示背景颜色或渐变，从而更好地融入界面设计。
    connect(fontColorToolButton, SIGNAL(clicked()), this, SLOT(textButtonTriggered()));
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()), this, SLOT(fillButtonTriggered()));
    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));//设置按钮的id
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));
    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50") << tr("75") << tr("100") << tr("125") << tr("150");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    sceneScaleCombo->setEditable(true);
    QIntValidator *scaleValidator = new QIntValidator(1, 200, this);
    sceneScaleCombo->setValidator(scaleValidator);
    connect(sceneScaleCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(sceneScaleChanged(QString)));
    percentLabel = new QLabel(tr("%"), this);
    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
    pointerToolbar->addWidget(percentLabel);
}

QWidget *MainWindow::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

QWidget *MainWindow::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{
    DiagramItem item(type, itemMenu);
    QIcon icon(item.image());
    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")<< tr("yellow");
    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}

QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);
    return QIcon(pixmap);
}

QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);
    return QIcon(pixmap);
}

QList<QGraphicsItem*> MainWindow::cloneItems(const QList<QGraphicsItem*>& items)
{
    QHash<QGraphicsItem*, QGraphicsItem*> copyMap;
    foreach (QGraphicsItem* item, items) {
        if (item->type() == DiagramItem::Type) {
            copyMap[item] = qgraphicsitem_cast<DiagramItem*>(item)->clone();
        } else if (item->type() == DiagramTextItem::Type) {
            copyMap[item] = qgraphicsitem_cast<DiagramTextItem*>(item)->clone();
        }
    }
    return copyMap.values();
}

void MainWindow::onFileExport()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Diagram"), "", tr("XML files (*.xml)"));
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("diagram");
    foreach (QGraphicsItem *item, scene->items()) {
        if (DiagramItem *diagramItem = qgraphicsitem_cast<DiagramItem *>(item)) {
            diagramItem->writeToXml(xmlWriter);
        } else if (DiagramTextItem *textItem = qgraphicsitem_cast<DiagramTextItem *>(item)) {
            textItem->writeToXml(xmlWriter);
        }
    }

    xmlWriter.writeEndElement(); // 结束 "diagram" 元素
    xmlWriter.writeEndDocument();

    file.close();
}

void MainWindow::onFileImport()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Diagram"), "", tr("XML files (*.xml)"));
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    QXmlStreamReader xmlReader(&file);
    QHash<QString, DiagramItem*> itemMap; // 用于保存已创建的节点，以便连接箭头

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;

        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == "item") {
                int type = xmlReader.attributes().value("type").toString().toInt();
                int id = xmlReader.attributes().value("Id").toString().toInt();
                QPointF pos = QPointF(xmlReader.attributes().value("pos").toString().split(',').first().toDouble(),
                                      xmlReader.attributes().value("pos").toString().split(',').last().toDouble());
                QColor color = QColor(xmlReader.attributes().value("color").toString());
                qreal zValue = xmlReader.attributes().value("zvalue").toString().toDouble();
                DiagramItem *item = new DiagramItem(static_cast<DiagramItem::DiagramType>(type), nullptr);
                item->setPos(pos);
                item->setBrush(QBrush(color));
                item->setZValue(zValue); // 设置 zValue
                scene->addItem(item);
                itemMap[QString::number(id)] = item; // 保存到哈希表中

            } else if (xmlReader.name() == "text") {
                QPointF pos = QPointF(xmlReader.attributes().value("pos").toString().split(',').first().toDouble(),
                                      xmlReader.attributes().value("pos").toString().split(',').last().toDouble());
                QColor color = QColor(xmlReader.attributes().value("color").toString());
                int fontSize = xmlReader.attributes().value("font-size").toString().toInt();
                QString fontFamily = xmlReader.attributes().value("font-family").toString();
                bool bold = xmlReader.attributes().value("bold").toString().toInt();
                bool italic = xmlReader.attributes().value("italic").toString().toInt();
                bool underline = xmlReader.attributes().value("underline").toString().toInt();
                QString textContent = xmlReader.attributes().value("text").toString();
                qreal zValue = xmlReader.attributes().value("zvalue").toString().toDouble();
                DiagramTextItem *textItem = new DiagramTextItem(nullptr);
                textItem->setPos(pos);
                QFont font(fontFamily, fontSize);
                font.setBold(bold);
                font.setItalic(italic);
                font.setUnderline(underline);
                textItem->setFont(font);
                textItem->setZValue(zValue);
                textItem->setPlainText(textContent);
                textItem->setDefaultTextColor(color);
                scene->addItem(textItem);
            }

        }
    }

    if (xmlReader.hasError()) {
        QMessageBox::warning(this, tr("Error"), tr("Error reading file: %1").arg(xmlReader.errorString()));
    }

    file.close();
}
