#ifndef OZ_UTILS_H
#define OZ_UTILS_H
#include <QDateTime>
namespace Qs {
const double epsilon = 1E-10;
struct LessThan {
    const bool operator()(const double& lhs, const double& rhs) const {
        return (rhs - lhs > epsilon);
    }
};

inline bool lessThan(const double& lhs, const double& rhs) {
    LessThan isLessThan;
    return isLessThan(lhs, rhs);
}

inline bool lessThanOrEqualTo(const double& lhs, const double& rhs) {
    LessThan isLessThan;
    return !isLessThan(rhs, lhs);
}

inline bool equalTo(const double& lhs, const double& rhs) {
    LessThan isLessThan;
    return !(isLessThan(lhs, rhs) || isLessThan(rhs, lhs));
}

inline bool notEqualTo(const double& lhs, const double& rhs) {
    LessThan isLessThan;
    return isLessThan(lhs, rhs) || isLessThan(rhs, lhs);
}

inline bool greaterThan(const double& lhs, const double& rhs) {
    LessThan isLessThan;
    return isLessThan(rhs, lhs) && !isLessThan(lhs, rhs);
}

inline bool greaterThanOrEqualTo(const double& lhs, const double& rhs) {
    LessThan isLessThan;
    return !isLessThan(lhs, rhs);
}

// TODO ? use this
// Return part of datetime, which is in form of YYYYMMDDSSSSSssssss
//                                              ----++--+++++------
//                                               4  2 2   5    6
//

// XTP format
// YYYYMMDDHHmmSSsss
inline qint32 yearOf(quint64 datetime) {
    return datetime / static_cast<quint64>(1E13);
}

inline qint32 monthOf(quint64 datetime) {
    return datetime / static_cast<quint64>(1E11) % 100;
}

inline qint32 dayOf(quint64 datetime) {
    return datetime / static_cast<quint64>(1E9) % 100;
}

inline qint32 hourOf(quint64 datetime) {
    return datetime / static_cast<quint64>(1E7) % 100;
}

inline qint32 minuteOf(quint64 datetime) {
    return datetime / static_cast<quint64>(1E5) % 100;
}

inline qint32 secondOf(quint64 datetime) {
    return datetime / static_cast<quint64>(1E3) % 100;
}

inline qint32 millisecondOf(quint64 datetime) {
    return datetime % 1000;
}

inline quint64 msecsSinceEpochOf(quint64 datetime) {
    return QDateTime::fromString(QString::number(datetime), "yyyyMMddHHmmsszzz").toMSecsSinceEpoch();
}
} // namespace Qs
#endif //  OZ_UTILS_H
