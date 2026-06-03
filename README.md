# MyDino

一个使用 C++17 和 Qt6 编写的横版跑酷小游戏，玩法参考经典 Chrome 小恐龙。玩家需要控制小恐龙跳跃、下蹲冲刺、发射火球，躲避仙人掌和飞鸟，并尽量获得更高分数。

## 项目特点

- 使用 Qt Widgets 绘制游戏窗口
- 使用 Qt Multimedia 播放跳跃、受伤、射击等音效
- 支持分层背景、角色动画、障碍物随机生成
- 包含生命值、体力值、分数和最高分显示
- 使用 CMake 管理项目构建

## 环境要求

- CMake 3.24 或更高版本
- 支持 C++17 的编译器
- Qt 6，需包含以下模块：
  - Widgets
  - Multimedia

## 构建与运行

使用 Qt Creator 打开项目根目录下的 `CMakeLists.txt` 进行构建和运行。
构建完成后，程序会自动将 `Resources` 目录复制到可执行文件所在目录，确保图片、音频和字体资源可以被正常加载。

## 操作说明

- `Space` / `Up` / `W`：开始游戏或跳跃
- `Down` / `S`：下蹲冲刺
- `Right`：消耗体力发射火球
- `Left`：体力满时进入无敌状态
- `Esc`：暂停或继续游戏
- `R`：游戏结束后重新开始

## 目录结构

```text
.
├── CMakeLists.txt
├── README.md
├── Resources
│   ├── Audios
│   ├── Textures
│   └── TEXTS.ttf
└── src
    ├── GameWidget.cpp
    ├── GameWidget.h
    ├── MainWindow.cpp
    ├── MainWindow.h
    └── main.cpp
```

