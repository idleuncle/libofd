#ifndef __UTILS_GEOMETRY_H__
#define __UTILS_GEOMETRY_H__

namespace utils{
    namespace geometry{

        typedef struct Point{
            double x = 0;
            double y = 0;

            Point(){};
            Point(double _x, double _y):x(_x), y(_y){};
            void Offset(double cx, double cy){x+=cx; y+=cy;};

        } *_Point_t;

        typedef struct Size{
            double width = 0;
            double height = 0;

            Size(){};
            Size(double _width, double _height):width(_width), height(_height){};

            void Zoom(double scaling){
                width = width * scaling;
                height = height * scaling;
            }
        } *Size_t;

        typedef struct Rect{
            double x = 0;
            double y = 0;
            double width = 0;
            double height = 0;

            Rect(){};
            Rect(double _x, double _y, double _width, double _height):
                x(_x), y(_y), width(_width), height(_height){
                }

            Point GetLeftTop() const {return Point(x, y);};
            Point GetLeftBottom() const {return Point(x, y+height);};
            Point GetRightTop() const {return Point(x+width, y);};
            Point GetRightBottom() const {return Point(x+width, y+height);};
            Point GetCenter() const {return Point(x + width / 2, y + height / 2);};
            void Offset(double cx, double cy){x+=cx; y+=cy;};

            bool InRect(double _x, double _y) const{
                return _x >= x && _x < x + width &&
                    _y >= y && _y < y + height;
            }

        } *Rect_t;

    } // namespace geometry
} // namespace utils

#endif // __UTILS_GEOMETRY_H__
