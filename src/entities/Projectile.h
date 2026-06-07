#pragma once

#include <QRect>

class Projectile {
public:
    void reset();
    void shoot(const QRect &dinoRect, bool dinoSprinting, bool dinoInAir);
    void update();
    void deactivate();

    bool active() const;
    QRect drawRect() const;
    QRect collisionRect() const;

private:
    bool fireballActive_ = false;
    QRect fireballRect_ {0, 0, 50, 50};  // 初始位置，没什么用，反正后面会更新
};
