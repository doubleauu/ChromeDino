#pragma once

#include <QTimer>
#include <QVector>
#include <QWidget>

#include "core/GameState.h"
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
    void saveScoreOnGameOver();  // 游戏结束时保存本局分数

    Assets assets_;
    GameState state_;
    InputState input_;
    Dino dino_;
    Background background_;
    Obstacle obstacle_;
    Projectile fireball_;
    GameRenderer renderer_;
    QVector<int> topScores_;
    bool scoreSavedForCurrentGame_ = false;  // 标记每一局游戏存档写入状态；

};
