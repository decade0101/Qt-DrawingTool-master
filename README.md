DiagramScene 是一个基于Qt框架开发的图形场景编辑器应用，它允许用户在画布上插入、移动、缩放和修改各种类型的图表项（例如开始/结束节点、条件分支、步骤等）以及文本项。
## 功能特性
- **多样的图表项类型**：支持多种类型的图表项（如开始/结束、条件判断、步骤等），每种类型都有独特的形状。
- **文本编辑功能**：允许在画布中添加可编辑的文本项，支持字体、颜色设置。
- **丰富的交互操作**：提供对图表项和文本项进行选择、移动、调整大小的操作，并且支持撤销/重做功能。
- **便捷的快捷键支持**：通过按下Ctrl键切换视图模式为手型拖动，方便浏览整个场景。
- **粘性模式与正交辅助线**：当移动图表项时，提供粘性对齐功能及显示水平或垂直辅助线以帮助对齐。
- **分组管理**：支持将多个图表项组合成一组，便于统一管理和操作。
- **背景样式选择**：提供多种不同的网格背景选项，以适应不同的视觉需求。
## 如何使用
1. **运行环境**：确保已安装Qt库和相应的开发环境。
2. **编译项目**：使用Qt Creator打开项目文件，然后构建并运行。
3. **操作说明**：
   - 使用工具栏中的按钮来选择不同类型的图表项或文本项并将其添加到场景中。
   - 右击图表项可以访问上下文菜单进行更多操作。
   - 利用键盘上的Ctrl键可以在浏览模式和选择模式之间快速切换。
   - 使用编辑菜单下的命令来进行撤销、重做、复制、粘贴、剪切、删除等操作。
## 文件结构概述
- `diagramitem.h/.cpp`：定义了图表项的基本属性和行为。
- `diagramscene.h/.cpp`：实现了包含图表项和文本项的场景逻辑。
- `diagramtextitem.h/.cpp`：定义了文本项的行为。
- `diagramview.h/.cpp`：提供了定制化的视图类，用于展示和交互。
- `mainwindow.h/.cpp`：主窗口实现，包含了UI布局和主要的业务逻辑。
- `undosystem.h/.cpp`：描述了项目的撤销/重做系统设计。
