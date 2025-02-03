#include "undosystem.h"
#include <QDebug>
void UndoSystem::backup(const QList<QGraphicsItem*>&& items) {
    qDebug() << "inside backup." << items.size();
    int stackSize = itemsStack.length();
    // 已经执行了某些撤销操作，此时需要清理从当前索引之后的所有状态（即未来的重做部分），以确保撤销栈的一致性
    if (currentIndex < stackSize - 1) {
        for (int i = currentIndex + 1; i < stackSize; ++i) {
            free(itemsStack[i]);
        }
        itemsStack.erase(itemsStack.begin() + currentIndex + 1, itemsStack.end());
    }
    itemsStack.push_back(items);
    currentIndex++;
}

QList<QGraphicsItem*> UndoSystem::undo() {
    return itemsStack[--currentIndex];
}

QList<QGraphicsItem*> UndoSystem::redo() {
    return itemsStack[++currentIndex];
}

void UndoSystem::free(QList<QGraphicsItem*> const& items) {
    foreach(QGraphicsItem* p, items) {
        delete p;
    }
}
