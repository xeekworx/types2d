#pragma once
#include <sstream>

namespace xeekworx {
    namespace types2d {

        template <typename T = int>
        class point {
        public:
            T x, y;

            point(const T x = T(), const T y = T()) : x(x), y(y) {}
            point(const point& pt) : x(pt.x), y(pt.y) {}
            point(const T * array) : point(array[0], array[1]) {}
            virtual ~point() {}

            void clear() { x = y = T(); }

            void set(const T x, const T y)
            {
                *this = point(x, y);
            }

            double distance(const T x, const T y)
            {
                const double x_distance = (double)this->x - (double)x;
                const double y_distance = (double)this->y - (double)y;
                return sqrt(x_distance*x_distance + y_distance * y_distance);
            }

            double distance(const point& pt)
            {
                const double x_distance = (double)x - (double)pt.x;
                const double y_distance = (double)y - (double)pt.y;
                return sqrt(x_distance*x_distance + y_distance * y_distance);
            }

            void offset(const T x, const T y)
            {
                this->x += x;
                this->y += y;
            }

            void offset(const point& pt)
            {
                this->x += pt.x;
                this->y += pt.y;
            }

            bool empty() const
            {
                if (!x && !y) return true;
                else return false;
            }

            std::string str() const
            {
                std::stringstream ss;
                ss << x << ", " << y;
                return ss.str();
            }

            point operator+(const point& pt) { return point(x + pt.x, y + pt.y); }
            void operator+=(const point& pt) { offset(pt); }
            point operator-(const point& pt) { return point(x - pt.x, y - pt.y); }
            void operator-=(const point& pt) { offset(-pt.x, -pt.y); }
            bool operator==(const point& pt) { return (x == pt.x && y == pt.y); }
            bool operator!=(const point& pt) { return (x != pt.x || y != pt.y); }
        };

    }
}