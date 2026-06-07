#include "Assets.h"

// 字体相关类库：
#include <QFontDatabase>
#include <QStringList>
#include <QUrl>  // 加载音频资源

bool Assets::load(const QString &appDir) {
    // 加载小恐龙图片
    const QString imagePath = appDir + "/Resources/Textures/";
    run1Pixmap_.load(imagePath + "Run1.png");
    run2Pixmap_.load(imagePath + "Run2.png");
    idlePixmap_.load(imagePath + "Idle.png");
    sprint1Pixmap_.load(imagePath + "Sprint1.png");
    sprint2Pixmap_.load(imagePath + "Sprint2.png");

    // 仙人掌照片：
    cactusSmall1Pixmap_.load(imagePath + "Cactus_SMALL1.png");
    cactusSmall2Pixmap_.load(imagePath + "Cactus_SMALL2.png");
    cactusSmall3Pixmap_.load(imagePath + "Cactus_SMALL3.png");
    cactusBig1Pixmap_.load(imagePath + "Cactus_BIG1.png");
    cactusBig2Pixmap_.load(imagePath + "Cactus_BIG2.png");
    cactusMixPixmap_.load(imagePath + "Cactus_MIX.png");

    // 小鸟照片：
    bird1Pixmap_.load(imagePath + "Bird1.png");
    bird2Pixmap_.load(imagePath + "Bird2.png");

    // 背景照片：
    starPixmap_.load(imagePath + "LAYER_STAR.png");
    moonPixmap_.load(imagePath + "LAYER_MOON.png");
    cloudPixmap_.load(imagePath + "LAYER_CLOUD.png");
    groundPixmap_.load(imagePath + "LAYER_GROUND.png");

    // 血量图片：
    heart1Pixmap_.load(imagePath + "Heart1.png");
    heart2Pixmap_.load(imagePath + "Heart2.png");
    heart3Pixmap_.load(imagePath + "Heart3.png");

    // 体力图片：
    stamina0Pixmap_.load(imagePath + "Stamia0.png");
    stamina1Pixmap_.load(imagePath + "Stamia1.png");
    stamina2Pixmap_.load(imagePath + "Stamia2.png");
    stamina3Pixmap_.load(imagePath + "Stamia3.png");

    // 火球图片：
    fireballPixmap_.load(imagePath + "Fireball.png");

    // 能量球图片：
    energyballPixmap_.load(imagePath + "Energyball.png");

    // 加载字体：
    const QString resourcePath = appDir + "/Resources/";
    const int fontId  = QFontDatabase::addApplicationFont(resourcePath + "TEXTS.ttf");  // 把字体文件注册到当前 Qt 程序中
    if(fontId >= 0) {  // 如果加载成功（返回一个编号）：失败是返回-1；
        const QStringList families = QFontDatabase::applicationFontFamilies(fontId); // 根据编号获取字体名称列表
        if(!families.isEmpty()) {   // 如果列表不为空，就取列表中第一个字体名称存入 fontFamily_
            fontFamily_ = families.first();
        }
    }


    // 加载音频资源：
    const QString audioPath = appDir + "/Resources/Audios/";

    // 匿名函数，避免重复代码
    auto setupSound = [](QSoundEffect &sound, const QString &filePath) {
        sound.setSource(QUrl::fromLocalFile(filePath));  // 设置音效路径
        sound.setVolume(0.6f);  // 设置音量
    };

    setupSound(jumpSound_, audioPath + "JUMP.wav");
    setupSound(hurtSound_, audioPath + "HURT.wav");
    setupSound(shootSound_, audioPath + "SHOOT.wav");
    setupSound(godSound_, audioPath + "GOD.wav");
    setupSound(scoreSound_, audioPath + "SCORE.wav");

    return true;
}
