#pragma once

#include <QPixmap>  // Qt 中用来保存图片的类
#include <QSoundEffect>
#include <QString>

class Assets {
public:
    bool load(const QString &appDir);

    // 添加分层背景图片资源
    QPixmap starPixmap_;
    QPixmap moonPixmap_;
    QPixmap cloudPixmap_;
    QPixmap groundPixmap_;

    // 存放小恐龙图片
    QPixmap run1Pixmap_;
    QPixmap run2Pixmap_;
    QPixmap idlePixmap_;  // 跳跃状态
    QPixmap failPixmap_;
    // 下蹲状态图片：
    QPixmap sprint1Pixmap_;
    QPixmap sprint2Pixmap_;

    // 添加多种类型仙人掌障碍物：
    QPixmap cactusSmall1Pixmap_;
    QPixmap cactusSmall2Pixmap_;
    QPixmap cactusSmall3Pixmap_;
    QPixmap cactusBig1Pixmap_;
    QPixmap cactusBig2Pixmap_;
    QPixmap cactusMixPixmap_;

    //能量球图片：
    QPixmap energyballPixmap_;

    // 飞鸟照片：
    QPixmap bird1Pixmap_;
    QPixmap bird2Pixmap_;

    // 生命值图片：
    QPixmap heart1Pixmap_;
    QPixmap heart2Pixmap_;
    QPixmap heart3Pixmap_;

    // 体力图片：
    QPixmap stamina0Pixmap_;
    QPixmap stamina1Pixmap_;
    QPixmap stamina2Pixmap_;
    QPixmap stamina3Pixmap_;

    // 火球:
    QPixmap fireballPixmap_;

    // 排行榜图片：
    QPixmap leaderboardPanelPixmap_;
    QPixmap leaderboardMedal1Pixmap_;
    QPixmap leaderboardMedal2Pixmap_;
    QPixmap leaderboardMedal3Pixmap_;

    // 保存字体名称，使用默认字体兜底
    QString fontFamily_ = QStringLiteral("Consolas");

    // 音效：
    QSoundEffect jumpSound_;
    QSoundEffect hurtSound_;
    QSoundEffect shootSound_;
    QSoundEffect godSound_;
    QSoundEffect scoreSound_;
};
