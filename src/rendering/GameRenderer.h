#pragma once

#include <QRect>

class Assets;
class Background;
class Dino;
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
        bool welcome,
        bool paused,
        bool gameOver,
        bool godMode,
        int hurtProtectFrames,
        int health,
        int stamina,
        int score,
        int highScore
    ) const;
};
