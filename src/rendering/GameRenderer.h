#pragma once

#include <QRect>

class Assets;
class Background;
class Dino;
class GameState;
class Obstacle;
class Projectile;
class QPainter;

class GameRenderer {
public:
    void draw(
        QPainter &painter,
        const QRect &screenRect,
        const Assets &assets,
        const Background &background,
        const Dino &dino,
        const Obstacle &obstacle,
        const Projectile &fireball,
        const GameState &state
    ) const;
};
