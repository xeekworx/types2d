#pragma once
#include "point.h"
#include "size.h"
#include <sstream>
#include <vector>

namespace xeekworx {
    namespace types2d {

        template <typename T = int>
        class rectangle {
        public:
            T x, y, w, h;

            rectangle(const T x = T(), const T y = T(), const T w = T(), const T h = T()) : x(x), y(y), w(w), h(h) {}
            rectangle(const rectangle& rect) : rectangle(rect.x, rect.y, rect.w, rect.h) {}
            rectangle(const point<T>& pt, const types2d::size<T>& sz) : rectangle(pt.x, pt.y, sz.w, sz.h) {}
            rectangle(const point<T>& pt1, const point<T>& pt2) : rectangle(pt1.x, pt1.y, pt2.x - pt1.x, pt2.y - pt1.y) {}
            rectangle(const T * array) : rectangle(array[0], array[1], array[2], array[3]) {}

            virtual ~rectangle() {}

            void clear() { x = y = w = h = T(); }

            void set(const T x, const T y, const T w, const T h)
            {
                this->x = x;
                this->y = y;
                this->w = w;
                this->h = h;
            }

            void set(const point<T>& pt, const size<T>& sz)
            {
                this->x = pt.x;
                this->y = pt.y;
                this->w = sz.w;
                this->h = sz.h;
            }

            void set(const point<T>& pt1, const point<T>& pt2)
            {
                this->x = pt1.x;
                this->y = pt1.y;
                this->w = pt2.x - pt1.x;
                this->h = pt2.y - pt1.y;
            }

            void set(const rectangle& rect)
            {
                this->x = rect.x;
                this->y = rect.y;
                this->w = rect.w;
                this->h = rect.h;
            }

            void offset(const T x, const T y, const T w = 0, const T h = 0)
            {
                this->x += x;
                this->y += y;
                this->w += w;
                this->h += h;
            }

            void offset(const point<T>& pt)
            {
                this->x += pt.x;
                this->y += pt.y;
            }

            void offset(const size<T>& sz)
            {
                this->w += sz.w;
                this->h += sz.h;
            }

            void offset(const point<T>& pt, const size<T>& sz)
            {
                this->x += pt.x;
                this->y += pt.y;
                this->w += sz.w;
                this->h += sz.h;
            }

            void inflate(const T w, const T h)
            {
                this->x -= x;
                this->y -= y;
                this->w += x * 2;
                this->h += y * 2;
            }

            void inflate(const size<T>& sz)
            {
                this->x -= sz.w;
                this->y -= sz.h;
                this->w += sz.w * 2;
                this->h += sz.h * 2;
            }

            void deflate(const T w, const T h)
            {
                this->x += x;
                this->y += y;
                this->w -= x * 2;
                this->h -= y * 2;
            }

            void deflate(const size<T>& sz)
            {
                this->x += sz.w;
                this->y += sz.h;
                this->w -= sz.w * 2;
                this->h -= sz.h * 2;
            }

            bool has_intersection(const rectangle& rect) const
            {
                return rectangle::has_intersection(*this, rect);
            }

            static bool has_intersection(const rectangle& rect1, const rectangle& rect2)
            {
                return !(rect2.x > rect1.right() || rect2.right() < rect1.x || rect2.y > rect1.bottom() || rect2.bottom() < rect1.y);
            }

            static rectangle intersect(const rectangle& rect1, const rectangle& rect2)
            {
                rectangle result;
                T min1, max1, min2, max2;

                // Horizontal intersection:
                min1 = rect1.x;
                max1 = min1 + rect1.w;
                min2 = rect2.x;
                max2 = min2 + rect2.w;
                if (min2 > min1) min1 = min2;
                result.x = min1;
                if (max2 < max1) max1 = max2;
                result.w = max1 - min1;

                // Vertical intersection:
                min1 = rect1.y;
                max1 = min1 + rect1.h;
                min2 = rect2.y;
                max2 = min2 + rect2.h;
                if (min2 > min1) min1 = min2;
                result.y = min1;
                if (max2 < max1) max1 = max2;
                result.h = max1 - min1;

                return result;
            }

            static rectangle union_rect(const rectangle& rect1, const rectangle& rect2)
            {
                T min1, max1, min2, max2;

                // Special cases for empty rectangles:
                if (rect1.empty()) {
                    if (rect2.empty()) { /* A and B empty */ return rectangle(); }
                    else { /* A empty, B not empty */ return rect2; }
                }
                else { if (rect2.empty()) { /* A not empty, B empty */ return rect1; } }

                rectangle result;

                // Horizontal union:
                min1 = rect1.x;
                max1 = min1 + rect1.w;
                min2 = rect2.x;
                max2 = min2 + rect2.w;
                if (min2 < min1) min1 = min2;
                result.x = min1;
                if (max2 > max1) max1 = max2;
                result.w = max1 - min1;

                // Vertical union:
                min1 = rect1.y;
                max1 = min1 + rect1.h;
                min2 = rect2.y;
                max2 = min2 + rect2.h;
                if (min2 < min1) min1 = min2;
                result.y = min1;
                if (max2 > max1) max1 = max2;
                result.h = max1 - min1;

                return result;
            }

            bool enclose_points(const std::vector<point<T>> points, const rectangle* clip_rect = nullptr)
            {
                return rectangle::enclose_points(points, clip_rect, this);
            }

            static bool enclose_points(const std::vector<point<T>> points, const rectangle* clip_rect = nullptr, rectangle* result = nullptr)
            {
                T minx = T(), miny = T(), maxx = T(), maxy = T();
                T x, y;

                if (points.size() < 1) {
                    return false;
                }

                if (clip_rect != nullptr) {
                    bool added = false;
                    const T clip_minx = clip_rect->x;
                    const T clip_miny = clip_rect->y;
                    const T clip_maxx = clip_rect->x + clip_rect->w - 1;
                    const T clip_maxy = clip_rect->y + clip_rect->h - 1;

                    if (clip_rect->empty()) return false; // Special case for empty rectangle:

                    for (auto i = points.begin(); i != points.end(); i++) {
                        x = (*i).x;
                        y = (*i).y;

                        if (x < clip_minx || x > clip_maxx || y < clip_miny || y > clip_maxy) continue;

                        if (!added) {
                            // Special case: if no result was requested, we are done:
                            if (result == nullptr) return true;

                            // First point added:
                            minx = maxx = x;
                            miny = maxy = y;
                            added = true;
                            continue;
                        }

                        if (x < minx) minx = x;
                        else if (x > maxx) maxx = x;

                        if (y < miny) miny = y;
                        else if (y > maxy) maxy = y;
                    }

                    if (!added) return false;
                }
                else {
                    // Special case: if no result was requested, we are done:
                    if (result == nullptr) return true;

                    // No clipping, always add the first point:
                    minx = maxx = points[0].x;
                    miny = maxy = points[0].y;

                    for (auto i = points.begin() + 1; i != points.end(); i++) {
                        x = (*i).x;
                        y = (*i).y;

                        if (x < minx) minx = x;
                        else if (x > maxx) maxx = x;

                        if (y < miny) miny = y;
                        else if (y > maxy) maxy = y;
                    }
                }

                if (result != nullptr) {
                    result->x = minx;
                    result->y = miny;
                    result->w = (maxx - minx) + 1;
                    result->h = (maxy - miny) + 1;
                }

                return true;
            }

            bool contains(const point<T>& pt) const
            {
                return ((pt.x >= x) && (pt.x < right()) && (pt.y >= y) && (pt.y < bottom()));
            }

            bool contains(const rectangle& rect) const
            {
                return ((rect.x >= x) && (rect.right() < right()) && (rect.y >= y) && (rect.bottom() < bottom()));
            }

            const T& left() const { return x; }
            const T& top() const { return y; }
            T right() const { return x + w; }
            T bottom() const { return y + h; }
            point<T> topleft() const { return point(x, y); }
            point<T> topright() const { return point(x + w, y); }
            point<T> bottomleft() const { return point(x, y + h); }
            point<T> bottomright() const { return point(x + w, y + h); }
            size<T> size() const { return spacetheory::size(w, h); }
            const T& width() const { return w; }
            const T& height() const { return h; }

            bool empty() const
            {
                if (!w || !h) return true;
                else return false;
            }

            std::string str() const
            {
                std::stringstream ss;
                ss << x << ", " << y << ", " << w << " x " << h;
                return ss.str();
            }

            rectangle& operator=(const rectangle& rect) { x = rect.x; y = rect.y; w = rect.w; h = rect.h; return *this; }
            rectangle operator+(const point<T>& pt) { return rectangle(x + pt.x, y + pt.y, w, h); }
            void operator+=(const point<T>& pt) { offset(pt); }
            rectangle operator-(const point<T>& pt) { return rectangle(x - pt.x, y - pt.y, w, h); }
            void operator-=(const point<T>& pt) { offset(-pt.x, -pt.y); }
            rectangle operator+(const types2d::size<T>& sz) { return rectangle(x, y, w + sz.w, h + sz.h); }
            void operator+=(const types2d::size<T>& sz) { offset(sz); }
            rectangle operator-(const types2d::size<T>& sz) { return rectangle(x, y, w - sz.w, h - sz.h); }
            void operator-=(const types2d::size<T>& sz) { offset(-sz.w, -sz.h); }
            rectangle operator+(const rectangle& rect) { return rectangle(x, y, w + rect.w, h + rect.h); }
            void operator+=(const rectangle& rect) { offset(rect.x, rect.y, rect.w, rect.h); }
            rectangle operator-(const rectangle& rect) { return rectangle(x, y, w - rect.w, h - rect.h); }
            void operator-=(const rectangle& rect) { offset(-rect.x, -rect.y, -rect.w, -rect.h); }
            bool operator==(const rectangle& rect) const { return (x == rect.x && y == rect.y && w == rect.w && h == rect.h); }
            bool operator!=(const rectangle& rect) const { return !(*this == rect); }
        };

    }
}