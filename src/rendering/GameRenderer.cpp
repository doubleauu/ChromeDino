#include "GameRenderer.h"

#include "../entities/Dino.h"
#include "../entities/Obstacle.h"
#include "../entities/Projectile.h"
#include "../resources/Assets.h"
#include "../scene/Background.h"

#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPixmap>
#include <QString>

void GameRenderer::draw(
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
) const {   // 表示不会修改当前 GameRenderer 对象的成员变量。
    // 背景：
    painter.fillRect(screenRect,QColor(32,33,36));  // 填充整个矩形界面
    // 分层画背景：
    painter.drawPixmap(background.starRect(), assets.starPixmap_);
    painter.drawPixmap(background.moonRect(), assets.moonPixmap_);
    painter.drawPixmap(background.cloudRect(), assets.cloudPixmap_);
    painter.drawPixmap(background.groundRect(), assets.groundPixmap_);


    // 绘制障碍物，代码写法同小恐龙，先写一个空指针，然后判断指向哪一个图片，最后进行绘制
    const QPixmap *obstaclePixmap = nullptr;

    switch(obstacle.type()) {
    case ObstacleType::CactusSmall1:
        obstaclePixmap = &assets.cactusSmall1Pixmap_;
        break;
    case ObstacleType::CactusSmall2:
        obstaclePixmap = &assets.cactusSmall2Pixmap_;
        break;
    case ObstacleType::CactusSmall3:
        obstaclePixmap = &assets.cactusSmall3Pixmap_;
        break;
    case ObstacleType::CactusBig1:
        obstaclePixmap = &assets.cactusBig1Pixmap_;
        break;
    case ObstacleType::CactusBig2:
        obstaclePixmap = &assets.cactusBig2Pixmap_;
        break;
    case ObstacleType::CactusMix:
        obstaclePixmap = &assets.cactusMixPixmap_;
        break;
    case ObstacleType::Bird:  // 飞鸟障碍物吗，需要动画判断
        obstaclePixmap = obstacle.currentBirdFrame() == 0 ? &assets.bird1Pixmap_ : &assets.bird2Pixmap_;
        break;
    case ObstacleType::Energy:
        obstaclePixmap = &assets.energyballPixmap_;
        break;
    }

    if(obstaclePixmap && !obstaclePixmap->isNull()) {  // 前者判断是否空指针，后者判断图片是否加载成功
        painter.drawPixmap(obstacle.drawRect(), *obstaclePixmap);  // 注意指针需要解引用，第一个参数是目标矩形（位置），第二个参数是填充图片
    }


    // 火球：
    if(fireball.active() && !assets.fireballPixmap_.isNull()) {
        painter.drawPixmap(fireball.drawRect(), assets.fireballPixmap_);
    }


    // 小恐龙
    const QPixmap *currentPixmap = nullptr;  // 指针指向的对象内容不能修改，但是可以更改指向内容
    if(dino.isInAir()) {
        currentPixmap = &assets.idlePixmap_;
    }else if(dino.isSprinting()) {  // 处理小恐龙下蹲时的动画帧数直接复用之前的判断变量：currentRunFrame_，保持一致
        if(dino.currentRunFrame() == 0) currentPixmap = &assets.sprint1Pixmap_;
        else currentPixmap = &assets.sprint2Pixmap_;
    }else if(dino.currentRunFrame() == 0) {
        currentPixmap = &assets.run1Pixmap_;
    }else {
        currentPixmap = &assets.run2Pixmap_;
    }

    if(currentPixmap && !currentPixmap->isNull()) {  // 如果图片加载成功了就绘制图片
        if(godMode) {  // 无敌状态
            painter.setOpacity(0.35);
        }else if(hurtProtectFrames > 0 && (hurtProtectFrames/6)%2==0) {  // 受伤保护下，透明状态每 6 帧切换一次，实现闪烁
            painter.setOpacity(0.35);  // 设置画笔透明度
        }
        painter.drawPixmap(dino.drawRect(), *currentPixmap);
        painter.setOpacity(1.0);
    }else {  // 未加载成功还绘制简易版矩形代替小恐龙
        painter.setBrush(QColor(80,200,120));  // 设置填充颜色
        painter.setPen(Qt::NoPen);  // 设置无边框
        painter.drawRect(dino.drawRect());  // 绘制矩形
    }


    // 状态绘制：
    painter.setPen(Qt::white);

    if(welcome) {
        painter.setFont(QFont(assets.fontFamily_, 32));
        painter.drawText(screenRect, Qt::AlignCenter, "Welcome to Run's Dino!\nPress Space to Start Running!");
    }else if(gameOver) {
        painter.setFont(QFont(assets.fontFamily_, 32));
        painter.drawText(screenRect, Qt::AlignCenter, "Game Over\nPress R to Restart");
    }else if(paused) {
        painter.setFont(QFont(assets.fontFamily_, 32));
        painter.drawText(screenRect, Qt::AlignCenter, "Paused\nPress ESC to Continue");
    }


    // 分数在欢迎界面不显示
    // 生命值在欢迎界面和失败界面不显示，后者已经在下方实现：生命值为 0，变成空指针不显示
    if(welcome) {
        return;
    }


    // 生命值绘制：如果游戏结束（生命值为零，就会变成空指针，也就不会绘制生命值）
    const QPixmap *heartPixmap = nullptr;

    if(health >= 3) {
        heartPixmap = &assets.heart3Pixmap_;
    }else if(health == 2) {
        heartPixmap = &assets.heart2Pixmap_;
    }else if(health == 1) {
        heartPixmap = &assets.heart1Pixmap_;
    }

    if(heartPixmap && !heartPixmap->isNull()) {
        painter.drawPixmap(QRect(20,700,200,60), *heartPixmap);
    }


    // 分数绘制：
    painter.setPen(Qt::white);
    painter.setFont(QFont(assets.fontFamily_,20));
    painter.drawText(
        QRect(1260,20,100,32),
        Qt::AlignRight | Qt::AlignVCenter,  // 靠右，且上下居中
        // 构造字符串对象：生成分数字符串 %1 是占位符，等待 arg 中的结果， 5 表示长度，10 表示进制，0 表示不足前面补 0
        QString("%1").arg(score / 15, 5, 10, QLatin1Char('0'))  //减小显示的分数，防止增长过快
    );
    painter.drawText(
        QRect(1400, 20, 130, 32),
        Qt::AlignRight | Qt::AlignVCenter,
        QString("HI %1").arg(highScore / 15, 5, 10, QLatin1Char('0'))
    );


    // 失败界面不绘制体力：
    if(gameOver) {
        return;
    }


    // 体力值绘制：
    const QPixmap *staminaPixmap = nullptr;

    if(stamina >= 3) {
        staminaPixmap = &assets.stamina3Pixmap_;
    }else if(stamina == 2) {
        staminaPixmap = &assets.stamina2Pixmap_;
    }else if(stamina == 1) {
        staminaPixmap = &assets.stamina1Pixmap_;
    }else {
        staminaPixmap = &assets.stamina0Pixmap_;
    }

    if(staminaPixmap && !staminaPixmap->isNull()) {
        painter.drawPixmap(QRect(270, 700, 200, 60), *staminaPixmap);
    }
}
