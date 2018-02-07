#include "QDebug"
#include <QFile>
#include "strategy/record/recordtickinfo.h"
using Qs::RecordTickInfo;
using Qs::TickInfo;

int main(int argc, char* argv[]) {
    QFile file("/tmp/sina.data");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file); // read the data serialized from the file
    RecordTickInfo info;
    while (!in.atEnd()) {
        in >> info;
        QString s("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16 %17 %18 %19 "
                  " %20 %21 %22 %23 %24 %25 %26 %27 %28 %29 %30 %31 %32 %33 %34 %35 %36 %37 %38 %39"
                  " %40 %41 %42 %43 %44 %45 %46 %47 %48 %49 %40 %41 %42 %43 %44 %45 %46 %47 %48 %49"
                  " %50 %51 %52");
        // s = s.arg(info.magic)
        s = s.arg(info.ticker)
                .arg(info.tickerName)
                .arg(info.averagePrice)
                .arg(info.lastPrice)
                .arg(info.preClosePrice)
                .arg(info.openPrice)
                .arg(info.closePrice)
                .arg(info.highPrice)
                .arg(info.lowPrice)
                .arg(info.volume)
                .arg(info.turnover);
        for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
            s = s.arg(info.bidPrice[i]);
        }
        for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
            s = s.arg(info.bidVolume[i]);
        }
        for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
            s = s.arg(info.askPrice[i]);
        }
        for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
            s = s.arg(info.askVolume[i]);
        }
        s = s.arg(info.datetime); // YYYYmmddHHMMSSsss
        qCritical() << s;
    }
    return 0;
}
