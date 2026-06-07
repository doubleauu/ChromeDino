#include "GameRenderer.h"

#include "../entities/Dino.h"
#include "../entities/Obstacle.h"
#include "../entities/Projectile.h"
#include "../core/GameState.h"
#include "../resources/Assets.h"
#include "../scene/Background.h"

#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include <QVector>

namespace {

QString scoreText(const QVector<int> &topScores, int index) {
    if(index < topScores.size()) {
        return QString("%1").arg(topScores[index], 5, 10, QLatin1Char('0'));
    }
    return QStringLiteral("-----");
}

void drawLeaderboard(QPainter &painter, const QRect &screenRect, const Assets &assets, const QVector<int> &topScores) {
    // 排行榜面板位置和大小：
    // x 使用窗口宽度居中；245 控制面板顶部 y 坐标；560/297 控制面板绘制宽高。
    const QRect panelRect((screenRect.width() - 560) / 2, 245, 560, 297);

    if(!assets.leaderboardPanelPixmap_.isNull()) {
        painter.drawPixmap(panelRect, assets.leaderboardPanelPixmap_);
    }else {
        painter.setBrush(QColor(48, 49, 52, 220));
        painter.setPen(QColor(180, 180, 180));
        painter.drawRect(panelRect);
    }

    const QPixmap *medals[] = {
        &assets.leaderboardMedal1Pixmap_,
        &assets.leaderboardMedal2Pixmap_,
        &assets.leaderboardMedal3Pixmap_
    };

    painter.setPen(Qt::white);
    painter.setFont(QFont(assets.fontFamily_, 20));
    for(int i = 0; i < 3; ++i) {
        // 每一行排行榜的位置：
        // 170 控制三条记录整体在面板内部的中心高度；52 控制每两行之间的垂直间距。
        const int rowY = panelRect.top() + 170 + (i - 1) * 52;
        // 奖牌位置和大小：
        // center.x - 105 控制奖牌横向位置；rowY - 19 控制奖牌相对本行的上下位置；34/45 控制奖牌大小。
        const QRect medalRect(panelRect.center().x() - 105, rowY - 19, 34, 45);

        if(!medals[i]->isNull()) {
            painter.drawPixmap(medalRect, *medals[i]);
        }else {
            painter.drawText(medalRect, Qt::AlignCenter, QString::number(i + 1));
        }

        painter.drawText(
            // 分数位置：
            // center.x - 50 控制分数横向位置；rowY - 12 控制分数上下位置；140/32 控制文字区域大小。
            QRect(panelRect.center().x() - 50, rowY - 12, 140, 32),
            Qt::AlignLeft | Qt::AlignVCenter,
            scoreText(topScores, i)
        );
    }
}

}

void GameRenderer::draw(
    QPainter &painter,
    const QRect &screenRect,
    const Assets &assets,
    const Background &background,
    const Dino &dino,
    const Obstacle &obstacle,
    const Projectile &fireball,
    const GameState &state,
    const QVector<int> &topScores
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
        if(state.godMode_) {  // 无敌状态
            painter.setOpacity(0.35);
        }else if(state.hurtProtectFrames_ > 0 && (state.hurtProtectFrames_/6)%2==0) {  // 受伤保护下，透明状态每 6 帧切换一次，实现闪烁
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

    if(state.welcome_) {
        painter.setFont(QFont(assets.fontFamily_, 32));
        // 欢迎界面标题位置：110 控制标题顶部 y 坐标，70 控制文字区域高度。
        painter.drawText(QRect(0, 110, screenRect.width(), 70), Qt::AlignCenter, "Welcome to Run's Dino!");
        drawLeaderboard(painter, screenRect, assets, topScores);
        painter.setFont(QFont(assets.fontFamily_, 22));
        // 欢迎界面开始提示位置：710 控制提示顶部 y 坐标，让文字完整显示在地面下方。
        painter.drawText(QRect(0, 710, screenRect.width(), 44), Qt::AlignCenter, "Press Space to Start Running!");
    }else if(state.gameOver_) {
        painter.setFont(QFont(assets.fontFamily_, 32));
        // 结束界面标题位置：70 控制标题顶部 y 坐标，64 控制文字区域高度。
        painter.drawText(QRect(0, 70, screenRect.width(), 64), Qt::AlignCenter, "Game Over");
        painter.setFont(QFont(assets.fontFamily_, 22));
        painter.drawText(
            // 结束界面分数位置：140 控制分数行顶部 y 坐标，36 控制文字区域高度。
            QRect(0, 140, screenRect.width(), 36),
            Qt::AlignCenter,
            QString("Score %1     HI %2")
                .arg(state.score_ / 15, 5, 10, QLatin1Char('0'))
                .arg(state.highScore_ / 15, 5, 10, QLatin1Char('0'))
        );
        drawLeaderboard(painter, screenRect, assets, topScores);
        // 结束界面重开提示位置：710 控制提示顶部 y 坐标，让文字完整显示在地面下方。
        painter.drawText(QRect(0, 710, screenRect.width(), 36), Qt::AlignCenter, "Press R to Restart");
    }else if(state.paused_) {
        painter.setFont(QFont(assets.fontFamily_, 32));
        painter.drawText(screenRect, Qt::AlignCenter, "Paused\nPress ESC to Continue");
    }


    // 分数在欢迎界面不显示
    // 生命值在欢迎界面和失败界面不显示，后者已经在下方实现：生命值为 0，变成空指针不显示
    if(state.welcome_) {
        return;
    }


    // 生命值绘制：如果游戏结束（生命值为零，就会变成空指针，也就不会绘制生命值）
    const QPixmap *heartPixmap = nullptr;

    if(state.health_ >= 3) {
        heartPixmap = &assets.heart3Pixmap_;
    }else if(state.health_ == 2) {
        heartPixmap = &assets.heart2Pixmap_;
    }else if(state.health_ == 1) {
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
        QString("%1").arg(state.score_ / 15, 5, 10, QLatin1Char('0'))  //减小显示的分数，防止增长过快
    );
    painter.drawText(
        QRect(1400, 20, 130, 32),
        Qt::AlignRight | Qt::AlignVCenter,
        QString("HI %1").arg(state.highScore_ / 15, 5, 10, QLatin1Char('0'))
    );


    // 失败界面不绘制体力：
    if(state.gameOver_) {
        return;
    }


    // 体力值绘制：
    const QPixmap *staminaPixmap = nullptr;

    if(state.stamina_ >= 3) {
        staminaPixmap = &assets.stamina3Pixmap_;
    }else if(state.stamina_ == 2) {
        staminaPixmap = &assets.stamina2Pixmap_;
    }else if(state.stamina_ == 1) {
        staminaPixmap = &assets.stamina1Pixmap_;
    }else {
        staminaPixmap = &assets.stamina0Pixmap_;
    }

    if(staminaPixmap && !staminaPixmap->isNull()) {
        painter.drawPixmap(QRect(270, 700, 200, 60), *staminaPixmap);
    }
}
