#ifndef OZ_UTILS_H
#define OZ_UTILS_H
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
}
#endif //  OZ_UTILS_H
