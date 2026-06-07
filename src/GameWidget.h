#pragma once

#include <QTimer>
#include <QWidget>

#include "entities/Dino.h"
#include "entities/Obstacle.h"
#include "entities/Projectile.h"
#include "input/InputState.h"
#include "rendering/GameRenderer.h"
#include "resources/Assets.h"
#include "scene/Background.h"

class GameWidget : public QWidget {  // 继承自 QWitget 类
public:
    explicit GameWidget(QWidget *parent = nullptr);  // 构造函数声明

protected:
    void paintEvent(QPaintEvent *event) override;  // 界面绘制函数
    void keyPressEvent(QKeyEvent *event) override; // 键盘输入函数
    void keyReleaseEvent(QKeyEvent *event) override;  // 下蹲按键触发
private:
    QTimer timer_; // 计时器，负责每隔一段时间提醒程序刷新
    void tick();  // 游戏每一帧要执行的函数，即每一帧的更新入口
    void resetGame();  // 重新开始游戏

    Assets assets_;
    InputState input_;
    Dino dino_;
    Background background_;
    Obstacle obstacle_;
    Projectile fireball_;
    GameRenderer renderer_;

    int speed_ = 2;  // 当前游戏速度档位，分数越高，速度越快
    int scrollSpeed_ = 6;  // 背景偏移速度
    int obstacleSpeed_ = 16;

    // 游戏基础状态：
    bool welcome_ = true;
    bool paused_ = false;
    bool gameOver_ = false;

    // 无敌状态：
    bool godMode_ = false;
    int godModeFrames_ = 0;  // 无敌帧数

    // 生命值和受伤保护：
    int health_ = 3;  // 初始值为 3
    int hurtProtectFrames_ = 0;

    // 体力值：不止是恐龙属性，还用于火球，界面绘制，游戏状态等等，所以放到主界面类中
    int stamina_ = 3;

    // 添加当前分数和最高分显示
    int score_ = 0;
    int highScore_ = 0;

    // 防止分数音效每一帧重复播放
    bool score3000Played_ = false;
    bool score6000Played_ = false;

};
