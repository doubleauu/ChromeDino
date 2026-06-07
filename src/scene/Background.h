#pragma once

#include <QRect>

class Background {
public:
    void reset();
    void update(int speed);

    QRect starRect() const;
    QRect moonRect() const;
    QRect cloudRect() const;
    QRect groundRect() const;

private:
    int groundOffset_ = 0;  // 背景偏移量

    // 每一层背景的绘制区域：
    QRect starRect_ {0,0,3200,800};
    QRect moonRect_ {0,0,3200,800};
    QRect cloudRect_ {0,0,3200,800};
    QRect groundRect_ {0,0,3200,800};
};
