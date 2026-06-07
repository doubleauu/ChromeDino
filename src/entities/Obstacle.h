#pragma once

#include <QRect>

// 添加飞鸟资源和障碍物类型，还有能量球
enum class ObstacleType {
    CactusSmall1,
    CactusSmall2,
    CactusSmall3,
    CactusBig1,
    CactusBig2,
    CactusMix,
    Bird,
    Energy
};

class Obstacle {
public:
    void spawn(int startX);  // 随机生成下一个障碍物
    void update(int obstacleSpeed);
    void updateAnimation();

    QRect drawRect() const;
    QRect collisionRect() const;
    bool isOffScreen() const;
    ObstacleType type() const;
    int currentBirdFrame() const;

private:
    QRect obstacleRect_ {1600,580,60,120};  // 障碍物所在矩形
    ObstacleType obstacleType_ = ObstacleType::CactusSmall1;  // 当前障碍物类型，默认小仙人掌

    int birdFrameCount_ = 0;  // 小鸟动画计数器，判断应该画哪一帧动画
    int currentBirdFrame_ = 0;  // 当前显示图片
};
