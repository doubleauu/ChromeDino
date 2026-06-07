# MyDino

一个使用 C++17 和 Qt6 编写的横版跑酷小游戏，玩法参考经典 Chrome 小恐龙。玩家需要控制小恐龙跳跃、下蹲冲刺、二段跳、发射火球，躲避仙人掌和飞鸟，并尽量获得更高分数。

## 项目特点

- 使用 Qt Widgets 绘制游戏窗口
- 使用 Qt Multimedia 播放跳跃、受伤、射击等音效
- 支持分层背景、角色动画、障碍物和能量球随机生成
- 包含生命值、体力值、分数、最高分和前三名排行榜显示
- 游戏结束后将分数保存到 `Resources/scores.json`
- 代码按核心状态、实体、渲染、资源、场景和输入模块拆分
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
- 空中再次按 `Space` / `Up` / `W`：消耗 1 格体力二段跳
- `Down` / `S`：地面下蹲冲刺，空中快速下落
- `Right`：消耗体力发射火球
- `Left`：体力满时进入无敌状态
- `Esc`：暂停或继续游戏
- `R`：游戏结束后重新开始

## 目录结构

```text
.
├── CMakeLists.txt                 # CMake 构建配置，声明 Qt 模块、源文件和资源复制规则
├── README.md                      # 项目说明文档
├── Resources
│   ├── Audios                     # 跳跃、受伤、射击、分数等音效
│   ├── Textures                   # 恐龙、障碍物、背景、体力和排行榜贴图
│   └── TEXTS.ttf                  # 游戏界面字体
└── src
    ├── core                       # 游戏规则、全局状态和分数存档
    │   ├── GameRules.h            # 保存窗口尺寸、重力、血量、体力等规则常量
    │   ├── GameState.cpp
    │   ├── GameState.h            # 管理速度、暂停、生命值、体力、分数和无敌状态
    │   ├── SaveData.cpp
    │   └── SaveData.h             # 读写 `scores.json`，加载最高分和排行榜分数
    ├── entities                   # 游戏中的可交互对象
    │   ├── Dino.cpp
    │   ├── Dino.h                 # 管理小恐龙跳跃、下蹲冲刺、二段跳、动画和碰撞区域
    │   ├── Obstacle.cpp
    │   ├── Obstacle.h             # 随机生成并更新仙人掌、飞鸟和能量球
    │   ├── Projectile.cpp
    │   └── Projectile.h           # 管理火球发射、移动、命中和绘制区域
    ├── input
    │   └── InputState.h           # 记录当前键盘输入状态，供每帧逻辑读取
    ├── rendering
    │   ├── GameRenderer.cpp
    │   └── GameRenderer.h         # 统一绘制背景、角色、障碍物、UI、失败界面和排行榜
    ├── resources
    │   ├── Assets.cpp
    │   └── Assets.h               # 加载并保存图片、字体和音效资源
    ├── scene
    │   ├── Background.cpp
    │   └── Background.h           # 管理多层背景滚动和地面位置
    ├── GameWidget.cpp
    ├── GameWidget.h               # 游戏主控件，连接输入、计时器、状态更新、碰撞和绘制
    ├── MainWindow.cpp
    ├── MainWindow.h               # 主窗口，承载 GameWidget
    └── main.cpp                   # Qt 应用入口
```
