#ifndef _KUTIL_MISC_H_
#define _KUTIL_MISC_H_

#include <functional>

#include <QObject>
#include <QString>
#include <QUuid>
#include <QByteArray>
#include <QVariant>
#include <QDebug>
#include <QTextCodec>
#include <QDataStream>
#include <QDir>
#include <QTemporaryFile>
#include <QDateTime>
#include <QCryptographicHash>

namespace kutil {
inline QString GBK2UTF8(const QString &inStr) {
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    // QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    return gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8
}

inline QString UTF82GBK(const QString &inStr) {
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    // QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
    return utf2gbk;
}

inline std::string gbk2utf8(const QString &inStr) {
    return GBK2UTF8(inStr).toStdString();
}

inline QString utf82gbk(const std::string &inStr) {
    QString str = QString::fromStdString(inStr);
    return UTF82GBK(str);
}

// 唯一字符串
// 去掉 ｛｝括号
inline QString uuidString() {
    return QUuid::createUuid().toString().remove('{').remove('}');
}

// 以时间生成可读的编号，适用于调用间隔在1秒以上的使用
/*
 *	注意！！！
 *      不要使用默认参数，这个函数是故意设计的这么难用的
 *      因为不建议直接调用这个函数，而是在各自的库里在此基础上实现自己的函数
 */
inline QString readableUniqueString(const QString& prefix, const QString& postfix /*= ""*/) {
    Q_ASSERT(prefix.length() >= 8); //

    static int _no = 1; // 递增的
    QDateTime now = QDateTime::currentDateTime();

    QString s = QString("%1%2%3")
                .arg(now.toString("yyyyMMdd"))
                .arg(now.time().msecsSinceStartOfDay() / 1000, 5, 10, QChar('0'))
                .arg(++_no);
    return prefix + s + postfix;
}

enum EnumWriteTextMode {
    WriteAppend,
    WritePreAppend,
    OverWrite,
};


// 一次性读取所有的内容
inline QString readTextFile(const QString& file_path, QTextCodec* codec = nullptr) {
    QString s_all;
    QFile file(file_path);
    if (file.open(QFile::ReadOnly)) {
        QTextStream ts(&file);
        if (nullptr == codec) {
            codec = QTextCodec::codecForName("GBK");
        }
        ts.setCodec(codec);
        s_all = ts.readAll();
        file.close();
        return s_all;
    }
    return QString("");
}

inline bool writeTextFile(const QString& file_path, const QString& content,
                          EnumWriteTextMode mode = EnumWriteTextMode::OverWrite, QTextCodec* codec = QTextCodec::codecForName("UTF-8")) {
    QString old_content;
    if (mode != EnumWriteTextMode::OverWrite) {
        old_content = kutil::readTextFile(file_path, codec);
    }

    QFile::remove(file_path);
    QFile file(file_path);
    if (file.open(QFile::ReadWrite)) {
        QTextStream ts(&file);
        if (nullptr != codec) {
            ts.setCodec(codec);
            ts.setGenerateByteOrderMark(true);
        }

        switch (mode) {
        case EnumWriteTextMode::OverWrite:
            ts << content;
            break;
        case EnumWriteTextMode::WriteAppend:
            ts << old_content;
            ts << content;
            break;
        case EnumWriteTextMode::WritePreAppend:
            ts << content;
            ts << old_content;
            break;
        }
        file.close();
        return true;
    }
    return false;
}

// 信号槽连接是否成功
inline bool checkedConnect(QObject* sender, const char* signal,
                           QObject* reciver, const char* slot, Qt::ConnectionType typ = Qt::AutoConnection) {
    Q_ASSERT(nullptr != sender);
    Q_ASSERT(nullptr != reciver);
    Q_ASSERT(nullptr != slot);
    Q_ASSERT(nullptr != signal);

    if (!sender->connect(sender, signal, reciver, slot, typ)) {
        qDebug() << "Signal:" << signal << "\n"<< "Slot:" << slot;
        Q_ASSERT(false);
        return false;
    }
    return true;
}

// 去掉最后n个字符
inline QString removeLast(const QString &s, int n) {
    if (s.length() <= n)
        return "";
    QString ret = s;
    return ret.remove(s.length() - n, n);
}

// 当前日期
inline QString currentDate() {
    return QDate::currentDate().toString("yyyyMMdd");
}
};

#endif // _KUTIL_MISC_H_
