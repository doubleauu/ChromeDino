#include "Dino.h"

#include "../core/GameRules.h"

void Dino::reset() {
    // 重置小恐龙状态：
    dinoRect_.moveTop(GameRules::GroundY);
    velocityY_ = 0;
    inAir_ = false;
    sprint_ = false;
    doubleJumpAvailable_ = false;

    // 重置小恐龙动画：
    motionRateCount_ = 0;
    currentRunFrame_ = 0;
}

bool Dino::update(const InputState &input) {
    bool jumped = false;

    // 小恐龙跳跃：
    if(inAir_) {  // 如果在空中，就更新跳跃运动
        const int delta = velocityY_ / 60;  // 这一帧恐龙在 Y 轴上要移动多少像素。= 每秒速度/每秒帧数

        if(dinoRect_.y() - delta > GameRules::GroundY) {  // 当落到地面上时，更新状态，可以进行下一次跳跃；
            dinoRect_.moveTop(GameRules::GroundY);  // 将矩形顶部强制放回 groundY;
            velocityY_ = 0;
            inAir_ = false;
            doubleJumpAvailable_ = false;  // 刷新二段跳状态
        }else {
            dinoRect_.translate(0,-delta); // 在原位置基础上移动恐龙矩形，x 方向不动，y 方向移动 velocityY_（速度）
            velocityY_ -= GameRules::Gravity;  // 速度随重力加速度变化，向上时逐渐变小，向下时逐步变大
        }
    }

    // 每一帧持续判断小恐龙是否冲刺状态，避免落地后卡顿
    if(input.downPressed) {
        if(inAir_) {
            velocityY_ = GameRules::FallVelocity;
            sprint_ = false;
        }else {
            sprint_ = true;
        }
    }else {
        sprint_ = false;
    }

    // 每一帧持续判断小恐龙是否尝试跳跃，避免下蹲期间的跳跃按键无响应
    if(input.spacePressed && !inAir_ && !sprint_) {
        velocityY_ = GameRules::JumpVelocity;  // 初始速度，后续随重力加速度变化
        inAir_ =  true;
        doubleJumpAvailable_ = true;
        jumped = true;
    }

    return jumped;
}

bool Dino::tryDoubleJump() {
    if(!inAir_ || !doubleJumpAvailable_) {
        return false;
    }

    velocityY_ = GameRules::JumpVelocity;  // 如果可以跳跃，就给新的初始速度，模拟二次跳跃
    doubleJumpAvailable_ = false;
    sprint_ = false;
    return true;
}

void Dino::updateAnimation() {
    // 更新小恐龙跑步动画：
    if(!inAir_) {
        ++motionRateCount_;
        const int frameInterval = sprint_ ? 4 : 5;  // 下蹲状态就每 4 帧更新一次照片
        if(motionRateCount_ >= frameInterval) {  // 每五帧更新一次图片
            motionRateCount_ = 0;
            currentRunFrame_ = 1 - currentRunFrame_;
        }
    }
}

void Dino::stopSprinting() {
    sprint_ = false;
}

QRect Dino::drawRect() const {
    return dinoRect_;
}

QRect Dino::collisionRect() const {
    // 可变碰撞框：创建新的矩形，直接复用原版参数
    if(sprint_ && !inAir_) {
        return QRect(
            dinoRect_.x(),   // 小恐龙下蹲后变宽的部分恰好等于 40 像素，所以此处不用加
            dinoRect_.y()+100,  // 变低了 60 像素
            200,100
        );
    }

    return QRect(
        dinoRect_.x()+40,  // 图片本身有透明区域，所以碰撞框范围要减小一些
        dinoRect_.y()+40,
        100,150
    );
}

bool Dino::isInAir() const {
    return inAir_;
}

bool Dino::isSprinting() const {
    return sprint_;
}

int Dino::currentRunFrame() const {
    return currentRunFrame_;
}
