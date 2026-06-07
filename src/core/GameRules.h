#pragma once

// 这些变量只在游戏规则中使用，不属于成员变量，不需要暴露为可修改状态，表示规则常量
// constexpr 表示翻译期常量，强于 const
namespace GameRules {
constexpr int WindowWidth = 1600;
constexpr int WindowHeight = 800;
constexpr int GroundY = 500;  // 恐龙在地面时左上角 y 坐标
constexpr int JumpVelocity = 1840;  // 起跳初始速度
constexpr int Gravity = 80;  // 重力加速度
constexpr int FallVelocity = -1840;  // 空中下蹲快速下落
constexpr int HurtProtectFrames = 60;  // 受伤后保护 60 帧，约 1 秒，防止一个障碍物重复扣血
constexpr int GodModeFrames = 200;  // 最大无敌帧数
constexpr int MaxHealth = 3;
constexpr int MaxStamina = 3;  // 体力
}
