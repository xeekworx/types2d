#pragma once
#include <sstream>

namespace xeekworx {
    namespace types2d {

        template <typename T = int>
        class size {
        public:
            T w, h;

            size(const T w = T(), const T h = T()) : w(w), h(h) {}
            size(const size& sz) : w(sz.w), h(sz.h) {}
            size(const T * array) : size(array[0], array[1]) {}
            virtual ~size() {}

            void clear() { w = h = T(); }

            void set(const T w, const T h)
            {
                *this = size(w, h);
            }

            void offset(const T w, const T h)
            {
                this->w += h;
                this->h += h;
            }

            void offset(const size& sz)
            {
                this->w += sz.w;
                this->h += sz.h;
            }

            bool empty() const
            {
                if (!w && !h) return true;
                else return false;
            }

            std::string str() const
            {
                std::stringstream ss;
                ss << w << " x " << h;
                return ss.str();
            }

            bool operator>(const size& sz) { return this->w > sz.w && this->h > sz.h; }
            bool operator<(const size& sz) { return this->w < sz.w && this->h < sz.h; }
            bool operator>=(const size& sz) { return this->w >= sz.w && this->h >= sz.h; }
            bool operator<=(const size& sz) { return this->w <= sz.w && this->h <= sz.h; }
            size operator+(size sz) { return size(w + sz.w, h + sz.h); }
            void operator+=(size sz) { offset(sz); }
            size operator-(size sz) { return size(w - sz.w, h - sz.h); }
            void operator-=(size sz) { offset(-sz.w, -sz.h); }
            bool operator==(size sz) { return (w == sz.w && h == sz.h); }
            bool operator!=(size sz) { return (w != sz.w || h != sz.h); }
        };

    }
}