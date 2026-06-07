#pragma once
// 保留输入状态数据

struct InputState {
    bool downPressed = false;  // 是否持续冲刺
    bool spacePressed = false;  // 是否持续尝试跳跃
};
