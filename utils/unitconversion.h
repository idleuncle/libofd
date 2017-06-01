#ifndef __UTILS_UNITCONVERSION_H__
#define __UTILS_UNITCONVERSION_H__

namespace utils{

    double mm_to_inch(double mm){return mm / 25.4;};
    double inch_to_mm(double inch){return inch * 25.4;};
    double mm_to_point(double mm){return mm * 72 / 25.4;};
    double inch_to_point(double inch){return inch * 72;};
    double point_to_mm(double point){return point * 25.4 / 72;};
    double point_to_inch(double point){return point / 72;};
    double mm_to_pixel(double mm, double dpi){return dpi * mm / 25.4;};
    double inch_to_pixel(double inch, double dpi){return dpi * inch;};
    double point_to_pixel(double point, double dpi){return dpi * point / 72;};

} // namespace utils

#endif // __UTILS_UNITCONVERSION_H__
