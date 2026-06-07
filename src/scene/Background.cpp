#include "Background.h"

void Background::reset() {
    //重置障碍物和背景位置：
    groundOffset_ = 0;
    starRect_.moveLeft(0);
    moonRect_.moveLeft(0);
    cloudRect_.moveLeft(0);
    groundRect_.moveLeft(0);
}

void Background::update(int speed) {
    // 分层背景移动：不同层背景移动速率不同
    starRect_.translate(-speed * 2,0);
    moonRect_.translate(-speed * 1,0);
    cloudRect_.translate(-speed * 4,0);
    groundRect_.translate(-speed * 8,0);
    // 移动出窗口时回正
    if(starRect_.x() <= -1600) {
        starRect_.moveLeft(0);
    }
    if(moonRect_.x() <= -1600) {
        moonRect_.moveLeft(0);
    }
    if(cloudRect_.x() <= -1600) {
        cloudRect_.moveLeft(0);
    }
    if(groundRect_.x() <= -1600) {
        groundRect_.moveLeft(0);
    }
}

QRect Background::starRect() const {
    return starRect_;
}

QRect Background::moonRect() const {
    return moonRect_;
}

QRect Background::cloudRect() const {
    return cloudRect_;
}

QRect Background::groundRect() const {
    return groundRect_;
}
