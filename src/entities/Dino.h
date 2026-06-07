#pragma once

#include <QRect>

#include "../input/InputState.h"

class Dino {
public:
    void reset();
    bool update(const InputState &input);  // 更新小恐龙运动状态，返回这一帧是否起跳
    void updateAnimation();
    void stopSprinting();

    QRect drawRect() const;
    QRect collisionRect() const;
    bool isInAir() const;
    bool isSprinting() const;
    int currentRunFrame() const;

private:
    QRect dinoRect_ {20,500,200,200};  // 恐龙的位置和大小
    int velocityY_ = 0;  // 竖直方向速度
    bool inAir_ = false; // 是否在空中，防止无限连跳
    bool sprint_ = false;  // 是否下蹲冲刺

    int motionRateCount_ = 0;  // 计数器，控制切图时机
    int currentRunFrame_ = 0;  // 显示当前为第几张图片
};
