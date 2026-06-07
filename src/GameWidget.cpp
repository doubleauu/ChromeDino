#include "GameWidget.h"

#include "core/GameRules.h"
#include "core/SaveData.h"

#include <QCoreApplication>  // 应用程序本体的基础功能类
#include <QKeyEvent>
#include <QPainter>

// 构造函数实现：
GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)  // 依旧先初始化父类对象
{
    setFixedSize(GameRules::WindowWidth,GameRules::WindowHeight);  // 固定游戏界面大小
    setFocusPolicy(Qt::StrongFocus);  // 设置焦点策略，使其可以接受键盘输入（焦点：谁在接收键盘输入）

    // 建立信号与槽的连接，配合定时器，周期性调用 tick(),效果相当于一个循环
    // 当 timer_ 发出 timeout 信号，就调用当前对象的 tick（）
    // timer 是一个对象，此处要传地址，因为 connect 需要知道哪个对象发出信号
    connect(&timer_, &QTimer::timeout, this, &GameWidget::tick);  // 连接定时器 timer_ 与刷新函数 tick()
    timer_.start(1000/60);  //启动定时器，每隔约 16 毫秒触发一次 timeout 信号，约等于 60 帧率

    // Path 函数返回 .exe 文件所在路径，cmake构建会自动将资源文件复制到 exe 旁边
    assets_.load(QCoreApplication::applicationDirPath());  // 传入路径
    // 程序启动时先读取本地历史记录，让最高分可以跨运行保存。
    state_.highScore_ = SaveData::loadHighScore();
    topScores_ = SaveData::loadTopScores(3);

    // 随机生成第一个障碍物：
    obstacle_.spawn(width());
}

// 界面刷新函数实现，每一帧都会执行一次
void GameWidget::tick() {  // 作用域限定符写在返回类型后面

    // 欢迎，暂停，失败状态下，只重绘画面，不更新游戏逻辑：
    if(state_.welcome_) {
        state_.speed_ = 0;
        update();
        return;
    }

    if(state_.paused_) {
        update();
        return;
    }

    if(state_.gameOver_) {
        state_.speed_ = 0;
        // Game Over 状态会停留很多帧，所以这里需要通过标记保证只保存一次。
        saveScoreOnGameOver();
        update();
        return;
    }

    state_.updateTimers();
    state_.updateSpeed();

    if(dino_.update(input_)) {
        assets_.jumpSound_.play();  // 播放跳跃音效
    }

    background_.update(state_.speed_);

    obstacle_.update(state_.obstacleSpeed_);
    // 更换随机障碍物：
    if(obstacle_.isOffScreen()) {
        obstacle_.spawn(width());
    }


    fireball_.update();

    const QRect dinoCollisionBox = dino_.collisionRect();
    const QRect obstacleCollisionBox = obstacle_.collisionRect();

    // 火球与障碍物碰撞检测：
    if(fireball_.active() && fireball_.collisionRect().intersects(obstacleCollisionBox)) {
        fireball_.deactivate();
        obstacle_.spawn(width());
    }


    // 碰撞检测时加上保护检测，血量扣光时游戏结束
    if(dinoCollisionBox.intersects(obstacleCollisionBox)) {
        if(obstacle_.type() == ObstacleType::Energy) {  // 如果是撞上能量球，就加体力，上限为 3
            state_.recoverStamina();
            obstacle_.spawn(width());
        }else if(!state_.godMode_ && state_.hurtProtectFrames_ <= 0) {  // 否则，如果不在无敌状态且不在受伤保护期间就扣血
            state_.takeDamage();
            assets_.hurtSound_.play();  // 受伤音效
        }
    }



    if(state_.addScore(dino_.isSprinting())) {
        assets_.scoreSound_.play();
    }

    dino_.updateAnimation();
    obstacle_.updateAnimation();

    // 如果这一帧刚好扣完血，gameOver_ 会在上面的碰撞处理中变成 true，
    // 所以在本帧结束前尝试保存一次分数。
    saveScoreOnGameOver();

    update(); // qt的函数，用来 “请求” 每一帧界面的刷新，之后会触发 paintEvent()
}

// 实现界面绘制函数
// 当需要重画界面是自动调用，变量名可以省略，因为此处不会使用 event
void GameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);  // 创建画笔，属于当前窗口
    renderer_.draw(
        painter,
        rect(),
        assets_,
        background_,
        dino_,
        obstacle_,
        fireball_,
        state_,
        topScores_
    );
}

// 重新开始函数：
void GameWidget::resetGame() {
    dino_.reset();
    input_.downPressed = false;
    input_.spacePressed = false;

    //重置障碍物和背景位置：
    background_.reset();

    state_.reset();
    // 新的一局开始后，允许下一次 Game Over 再写入一条新记录。
    scoreSavedForCurrentGame_ = false;

    fireball_.reset();

    // 刷新随机障碍物
    obstacle_.spawn(width());

}

void GameWidget::saveScoreOnGameOver() {
    // 只有“已经游戏结束”并且“本局还没保存过”时，才真正写入 JSON。
    if(!state_.gameOver_ || scoreSavedForCurrentGame_) {
        return;
    }

    SaveData::appendGameScore(state_.score_);
    topScores_ = SaveData::loadTopScores(3);
    scoreSavedForCurrentGame_ = true;
}

// 键盘接受函数：
void GameWidget::keyPressEvent(QKeyEvent *event) {

    // 删除系统产生的自动重复按键事件，防止界面闪烁
    if(event->isAutoRepeat()) {
        QWidget::keyPressEvent(event);
        return;
    }

    // 重开按键：
    if(event->key()==Qt::Key_R && state_.gameOver_) {
        resetGame();
        state_.welcome_ = false;  // 实现失败后重开直接开跑；
        return;
    }

    // 欢迎界面按跳跃键开始游戏，可放到跳跃逻辑中处理，也可单独处理
    // 此时不处理其他按键
    if(state_.welcome_) {
        if(event->key()==Qt::Key_Space || event->key()==Qt::Key_Up || event->key()==Qt::Key_W) {
            input_.spacePressed = true;
            state_.welcome_ = false;
            state_.speed_ = 2;
        }
        return;
    }


    // Esc 暂停 / 继续：
    if(event->key()==Qt::Key_Escape) {
        if(!state_.gameOver_ && !state_.welcome_) {
            state_.paused_ = !state_.paused_;
        }

        // 其实我感觉接受更好一点，否则下蹲期间上方是飞鸟，暂停直接变成站立就碰上了

        // // 暂停后不再接收按键的释放事件
        // if(paused_) {
        //     downPressed_ = false;
        //     spacePressed_ = false;
        //     sprint_ = false;
        // }

        return;
    }

    // 暂停后只接受 esc 按键
    if(state_.paused_) {
        QWidget::keyPressEvent(event);  // 交给父类处理，其实就是丢掉不处理，写不写无所谓
        return;
    }

    // 游戏结束后不再处理按键
    if(state_.gameOver_) {
        QWidget::keyPressEvent(event);
        return;
    }


    // 右键发射火球：(欢迎界面不处理）
    if(!state_.welcome_ && event->key()==Qt::Key_Right) {
        if(state_.hasStamina() && !fireball_.active()) {  // 不能连续发射，因为只有一个火球矩形；
            state_.consumeStamina();
            assets_.shootSound_.play();
            fireball_.shoot(dino_.drawRect(), dino_.isSprinting(), dino_.isInAir());
        }
        return;
    }

    // 左键开启无敌：
    if(event->key() == Qt::Key_Left) {
        if(state_.canActivateGodMode()) {
            state_.activateGodMode();
            assets_.godSound_.play();
        }
        return;  // 低于三格体力不处理
    }


    // 只记录是否按下按键，具体情况交给 tick() 刷新函数判断
    // 下蹲冲刺按键：支持 down 和 s 键
    if(event->key()==Qt::Key_Down || event->key()==Qt::Key_S) {
        input_.downPressed = true;
        return;
    }

    // 跳跃按键
    if(event->key()==Qt::Key_Space || event->key()==Qt::Key_Up || event->key()==Qt::Key_W) {
        if(state_.hasStamina() && dino_.tryDoubleJump()) {
            state_.consumeStamina();
            assets_.jumpSound_.play();
        }
        input_.spacePressed = true;
        return;  // 其他按键不处理
    }

}

// 持续下蹲冲刺释放按键，松开按键时触发
void GameWidget::keyReleaseEvent(QKeyEvent *event) {

    // 删除系统产生的自动重复按键事件，防止界面闪烁
    if(event->isAutoRepeat()) {
        QWidget::keyReleaseEvent(event);
        return;
    }

    if(event->key()==Qt::Key_Down || event->key()==Qt::Key_S) {
        input_.downPressed = false;
        dino_.stopSprinting();
        return;
    }
    if(event->key()==Qt::Key_Space || event->key()==Qt::Key_Up || event->key()==Qt::Key_W) {
        input_.spacePressed = false;
        return;
    }
    QWidget::keyReleaseEvent(event);  // 其他按键丢该父类，也就是不处理；
}
