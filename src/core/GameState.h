#pragma once
// 统一管理跨模块的规则状态

class GameState {
public:
    void reset();  // 重新开始游戏时重置所有跨模块规则状态
    void updateTimers();
    void updateSpeed();
    bool addScore(bool sprinting);  // 更新分数，返回这一帧是否需要播放分数音效
    void recoverStamina();
    void takeDamage();
    bool hasStamina() const;
    void consumeStamina();
    bool canActivateGodMode() const;
    void activateGodMode();

    // 当前游戏速度档位，分数越高，速度越快
    int speed_ = 2;
    int scrollSpeed_ = 6;  // 背景偏移速度
    int obstacleSpeed_ = 16;

    // 游戏基础状态：
    bool welcome_ = true;
    bool paused_ = false;
    bool gameOver_ = false;

    // 无敌状态：
    bool godMode_ = false;
    int godModeFrames_ = 0;  // 无敌帧数

    // 生命值和受伤保护：
    int health_ = 3;  // 初始值为 3
    int hurtProtectFrames_ = 0;

    // 体力值：不止是恐龙属性，还用于火球，界面绘制，游戏状态等等，所以放到跨模块规则状态中
    int stamina_ = 3;

    // 添加当前分数和最高分显示
    int score_ = 0;
    int highScore_ = 0;

private:
    // 防止分数音效每一帧重复播放
    bool score3000Played_ = false;
    bool score6000Played_ = false;
};
