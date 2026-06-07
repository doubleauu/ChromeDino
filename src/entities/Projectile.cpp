#include "Projectile.h"

void Projectile::reset() {
    // 重置火球：(删除火球画面）
    fireballActive_ = false;
    fireballRect_ = QRect(0,0,50,50);
}

void Projectile::shoot(const QRect &dinoRect, bool dinoSprinting, bool dinoInAir) {
    fireballActive_  = true;

    // 先判断小恐龙的嘴部位置，找到火球的发射点
    const int mouthY = dinoSprinting && !dinoInAir
        ? dinoRect.y() + 100
        : dinoRect.y() + 40;

    fireballRect_ = QRect(220, mouthY + 20, 50, 50);
}

void Projectile::update() {
    // 火球移动：
    if(fireballActive_) {  // 如果当前有火球发射
        fireballRect_.translate(30,0);

        if(fireballRect_.x() > 2100) {
            fireballActive_ = false;    // 火球发射完毕
        }
    }
}

void Projectile::deactivate() {
    fireballActive_ = false;
}

bool Projectile::active() const {
    return fireballActive_;
}

QRect Projectile::drawRect() const {
    return fireballRect_;
}

QRect Projectile::collisionRect() const {
    return fireballRect_;
}
