#include "SaveData.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVector>

#include <algorithm>

// 匿名命名空间：只在当前 cpp 文件里使用的函数和常量放这里，
// 避免被其他文件直接调用，外部只需要使用 SaveData 类。
namespace {

constexpr int SaveVersion = 1;  // 存档格式版本
constexpr int ScoreScale = 15;  // 内部分数转换成界面显示分数时使用的缩放值
constexpr const char *SaveFileName = "scores.json";  // 最终生成的存档文件名

QString saveFilePath() {
#ifdef PROJECT_RESOURCE_DIR
    // PROJECT_RESOURCE_DIR 来自 CMakeLists.txt，指向项目源码里的 Resources 文件夹。
    // 放到资源目录下，学习调试时可以直接在项目目录中找到 scores.json。
    return QDir(QString::fromUtf8(PROJECT_RESOURCE_DIR)).filePath(SaveFileName);
#else
    // 如果没有通过 CMake 定义资源目录，就退回到当前运行目录下的 Resources。
    return QDir("Resources").filePath(SaveFileName);
#endif
}

// QJsonObject 是 Qt 提供的一个类，用来表示 JSON 对象

QJsonObject defaultSaveObject() {
    // 默认存档结构：没有存档文件，或者存档文件坏了时，就从这个空结构开始。
    QJsonObject root;
    root["version"] = SaveVersion;
    root["games"] = QJsonArray();
    return root;
}

QJsonObject readSaveObject() {
    // QFile 只负责读写原始文件内容，真正的 JSON 解析交给 QJsonDocument。
    QFile file(saveFilePath());
    if(!file.open(QIODevice::ReadOnly)) {
        // 文件不存在或者打不开时，不让游戏崩溃，直接返回一个空存档。
        return defaultSaveObject();
    }

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if(!document.isObject()) {
        // 如果文件内容不是合法 JSON 对象，也当作没有有效存档处理。
        return defaultSaveObject();
    }

    QJsonObject root = document.object();
    // 下面两步是简单的容错：防止手动改坏了字段后，后续代码读取数组时报错。
    if(!root["games"].isArray()) {
        root["games"] = QJsonArray();
    }
    if(!root["version"].isDouble()) {
        root["version"] = SaveVersion;
    }
    return root;
}

bool writeSaveObject(const QJsonObject &root) {
    const QString path = saveFilePath();
    const QFileInfo fileInfo(path);
    // 写入前先保证 Resources 目录存在，否则 QFile 无法创建 scores.json。
    if(!fileInfo.dir().exists() && !QDir().mkpath(fileInfo.absolutePath())) {
        return false;
    }

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    // QJsonDocument 负责把内存中的 JSON 对象转换成文本。
    // Indented 表示带缩进，方便我们直接打开文件查看。
    const QJsonDocument document(root);
    return file.write(document.toJson(QJsonDocument::Indented)) != -1;
}

}

int SaveData::loadHighScore() {
    const QJsonArray games = readSaveObject()["games"].toArray();
    int highScore = 0;

    // 每一局都保存了 rawScore，这里遍历所有历史记录，找出最大的原始分数。
    // 渲染时仍然会除以 15，所以这里不需要保存显示分数。
    for(const QJsonValue &game : games) {
        const QJsonObject gameObject = game.toObject();
        highScore = std::max(highScore, gameObject["rawScore"].toInt());
    }

    return highScore;
}

QVector<int> SaveData::loadTopScores(int count) {
    QVector<int> scores;
    if(count <= 0) {
        return scores;
    }

    const QJsonArray games = readSaveObject()["games"].toArray();
    for(const QJsonValue &game : games) {
        const QJsonObject gameObject = game.toObject();
        scores.append(gameObject["rawScore"].toInt());
    }

    std::sort(scores.begin(), scores.end(), [](int left, int right) {
        return left > right;
    });

    if(scores.size() > count) {
        scores.resize(count);
    }

    for(int &score : scores) {
        score /= ScoreScale;
    }

    return scores;
}

void SaveData::appendGameScore(int rawScore) {
    // 先读取旧存档，再追加新的一局，最后整体写回文件。
    QJsonObject root = readSaveObject();
    QJsonArray games = root["games"].toArray();

    QJsonObject game;
    game["playedAt"] = QDateTime::currentDateTime().toString(Qt::ISODate);  // 本局结束时间
    game["score"] = rawScore / ScoreScale;  // 玩家在界面上看到的分数
    game["rawScore"] = rawScore;  // 游戏内部使用的原始分数

    games.append(game);
    root["version"] = SaveVersion;
    root["games"] = games;

    writeSaveObject(root);
}
