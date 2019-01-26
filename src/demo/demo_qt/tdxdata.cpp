#include "tdxdata.h"
#include "misc.h"

TdxData::TdxData() {

}

chartbase::data_key_t toChartKey(const QString& s) {
    qDebug() << s;

    std::tm t;
    memset(&t, 0, sizeof(t));

    QStringList dt = s.split("-");
    if (dt.isEmpty()) {
        return 0;
    }

    // 年月日
    QStringList date = dt[0].split("/");
    t.tm_year = date[0].toInt();
    t.tm_mon = date[1].toInt() - 1; // month:[0, 11]
    t.tm_mday = date[2].toInt();

    if (t.tm_mon >= 12) {
        qDebug() << t.tm_mon;
    }

    // 时分
    if (dt.size() >= 2) {
        QStringList time = dt[1].split(":");
        t.tm_hour = time[0].toInt();
        t.tm_min = time[1].toInt();
    }

    auto d = charty::siaMakeChartKey(t);
    qDebug() << s << "   " << d;
    return d;
}

bool TdxData::loadKLine(const QString &file, KLine &out) {
    QStringList lines = kutil::readTextFile(file).split("\n");
    // 第一行
    QString symbol = lines.first().trimmed();
    out.symbol = symbol.toUtf8().data();
    lines.removeFirst();
    lines.removeFirst();

    for (const QString & l : lines) {
        QStringList item = l.split("\t");
        if (item.size() < 5) {
            continue;
        }

        auto k = toChartKey(item[0]);

        // 1 开
        double open = item[1].trimmed().toDouble();
        out.open.insert({k, open});

        // 2 高
        double high = item[2].trimmed().toDouble();
        out.high.insert({k, high});

        // 3 低
        double low = item[3].trimmed().toDouble();
        out.low.insert({k,low});

        // 4 收
        double close = item[4].trimmed().toDouble();
        out.close.insert({k, close});
    }
    return true;
}

