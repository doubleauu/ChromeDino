#include "GameState.h"

#include "GameRules.h"

#include <algorithm>

void GameState::reset() {
    // 重置游戏状态：
    gameOver_ = false;
    welcome_ = true;
    paused_ = false;
    score_ = 0;

    // 重置血量：
    health_ = GameRules::MaxHealth;
    hurtProtectFrames_ = 0;

    // 重置无敌状态：
    godMode_ = false;
    godModeFrames_ = 0;

    // 重置体力：
    stamina_ = GameRules::MaxStamina;

    // 重置速度：
    speed_ = 2;
    scrollSpeed_ = speed_ * 3;
    obstacleSpeed_ = speed_ * 8;

    // 重置分数音效状态
    score3000Played_ = false;
    score6000Played_ = false;
}

void GameState::updateTimers() {
    // 受伤保护时间倒计时：
    if(hurtProtectFrames_ > 0) {
        --hurtProtectFrames_;
    }

    // 无敌时间倒计时
    if(godModeFrames_ > 0) {
        --godModeFrames_;
        if(godModeFrames_ <= 0) {
            godMode_ = false;
        }
    }
}

void GameState::updateSpeed() {
    // 根据分数更新速度档位
    if(score_ >= 6000) {
        speed_ = 4;
    }else if (score_ >= 3000) {
        speed_ = 3;
    }else {
        speed_ = 2;
    }

    scrollSpeed_ = speed_ * 3;  // 背景滚动速度
    obstacleSpeed_ = speed_ * 8;   // 障碍物移动速度
}

bool GameState::addScore(bool sprinting) {
    bool scoreSoundNeeded = false;

    // 更新分数，每一帧加一分，之后显示的时候再缩小，不然不好实现
    if(!gameOver_) {  // 这里进行二次判断，是防止这一帧刚碰撞还继续加分
        score_ += speed_ - 1;
        if(sprinting) score_ += speed_ - 1;  // 冲刺状态加分
        highScore_ = std::max(highScore_, score_);

        // 分数阈值音效：
        if(score_ >= 3000 && !score3000Played_) {
            scoreSoundNeeded = true;
            score3000Played_ = true;
        }

        if(score_ >= 6000 && !score6000Played_) {
            scoreSoundNeeded = true;
            score6000Played_ = true;
        }
    }

    return scoreSoundNeeded;
}

void GameState::recoverStamina() {
    stamina_ = std::min(GameRules::MaxStamina, stamina_ + 1);
}

void GameState::takeDamage() {
    --health_;
    hurtProtectFrames_ = GameRules::HurtProtectFrames;

    if(health_ <= 0) {
        gameOver_ = true;
    }
}

bool GameState::hasStamina() const {
    return stamina_ > 0;
}

void GameState::consumeStamina() {
    --stamina_;
}

bool GameState::canActivateGodMode() const {
    return stamina_ >= GameRules::MaxStamina && !godMode_;
}

void GameState::activateGodMode() {
    stamina_ = 0;
    godMode_ = true;
    godModeFrames_ = GameRules::GodModeFrames;
}
