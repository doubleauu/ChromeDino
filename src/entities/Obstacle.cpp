#include "Obstacle.h"

#include <QRandomGenerator>  // 用于生成随机数

void Obstacle::spawn(int startX) {
    const int typeIndex = QRandomGenerator::global()->bounded(0,8);
    obstacleType_ = static_cast<ObstacleType>(typeIndex);

    birdFrameCount_ = 0;
    currentBirdFrame_ = 0;

    // 对于不同类型障碍物，更改他们的 图片所在矩形
    // 此时也刚好是刷新了新的障碍物位置，从窗口右端进入
    switch(obstacleType_) {
    case ObstacleType::CactusSmall1:
        obstacleRect_ = QRect(startX, 580, 60, 120);
        break;
    case ObstacleType::CactusSmall2:
        obstacleRect_ = QRect(startX, 580, 120, 120);
        break;
    case ObstacleType::CactusSmall3:
        obstacleRect_ = QRect(startX, 580, 180, 120);
        break;
    case ObstacleType::CactusBig1:
        obstacleRect_ = QRect(startX, 500, 100, 200);
        break;
    case ObstacleType::CactusBig2:
        obstacleRect_ = QRect(startX, 500, 200, 200);
        break;
    case ObstacleType::CactusMix:
        obstacleRect_ = QRect(startX, 500, 300, 200);
        break;
    case ObstacleType::Bird: {  // 飞鸟随机生成高度
        const int birdY = QRandomGenerator::global()->bounded(200, 450);
        obstacleRect_ = QRect(startX, birdY, 200, 150);
        break;
    }
    case ObstacleType::Energy: {  // 能量球随机生成高度
        const int energyY = QRandomGenerator::global()->bounded(150, 450);
        obstacleRect_ = QRect(startX, energyY, 50, 50);
        break;
    }
    }
}

void Obstacle::update(int obstacleSpeed) {
    // 障碍物移动
    obstacleRect_.translate(-obstacleSpeed,0);
}

void Obstacle::updateAnimation() {
    // 更新飞鸟动画：
    if(obstacleType_ == ObstacleType::Bird) {
        ++birdFrameCount_;

        if(birdFrameCount_ >= 15) {  // 每 15 帧更新一次图片
            birdFrameCount_ = 0;
            currentBirdFrame_ = 1 - currentBirdFrame_;
        }
    }
}

QRect Obstacle::drawRect() const {
    return obstacleRect_;
}

QRect Obstacle::collisionRect() const {
    // 小鸟的碰撞检测区域较小，创建新的矩形
    QRect obstacleCollisionBox = obstacleRect_;  // 默认使用仙人掌碰撞矩形
    if(obstacleType_ == ObstacleType::Bird) {
        obstacleCollisionBox = QRect (
            obstacleRect_.x()+40,
            obstacleRect_.y()+45,
            120,60
        );
    }
    if(obstacleType_ == ObstacleType::Energy) {
        obstacleCollisionBox = QRect(
            obstacleRect_.x() + 10,
            obstacleRect_.y() + 10,
            30,
            30
        );
    }

    return obstacleCollisionBox;
}

bool Obstacle::isOffScreen() const {
    return obstacleRect_.right() < 0;
}

ObstacleType Obstacle::type() const {
    return obstacleType_;
}

int Obstacle::currentBirdFrame() const {
    return currentBirdFrame_;
}
