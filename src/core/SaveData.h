#pragma once

#include <QVector>

// 负责本地存档读写的工具类：
// 目前只保存每局分数历史，并从历史记录中读取最高分。
class SaveData {
public:
    static int loadHighScore();  // 从 scores.json 中读取历史最高分
    static QVector<int> loadTopScores(int count);  // 读取排行榜分数，返回界面显示分数
    static void appendGameScore(int rawScore);  // 游戏结束时追加保存本局分数
};
