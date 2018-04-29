#pragma once
#include <sstream>

namespace xeekworx {
    namespace types2d {

        template <typename T = float>
        class padding {
        private:
            T left, top, right, bottom;

        public:
            padding() : padding(T(), T(), T(), T()) {}
            padding(const T left, const T top, const T right, const T bottom) :
                left(left),
                top(top),
                right(right),
                bottom(bottom) {}
            padding(const padding& cr) : padding(cr.left, cr.top, cr.right, cr.bottom) {}
            padding(const T value) : padding(value, value, value, value) {}
            padding(const T * array) : padding(array[0], array[1], array[2], array[3]) {}

            virtual ~padding() {}

            void clear() { left = top = right = bottom = T(); }

            void set(const T left, const T top, const T right, const T bottom)
            {
                *this = padding(left, top, right, bottom);
            }

            void set(const T radius)
            {
                *this = padding(radius);
            }

            void offset(const T left, const T top, const T right, const T bottom)
            {
                this->left += left;
                this->top += top;
                this->right += right;
                this->bottom += bottom;
            }

            void offset(const T value)
            {
                offset(value, value, value, value);
            }

            bool empty() const
            {
                if (!left && !top && !right && !bottom) return true;
                return false;
            }

            std::string str() const
            {
                std::stringstream ss;
                ss << left << ", " << top << ", " << right << ", " << bottom;
                return ss.str();
            }

            void operator+=(const T value) { offset(value); }
            void operator-=(const T value) { offset(-value); }
            bool operator==(const padding& cr) const
            {
                return (
                    left == cr.left &&
                    top == cr.top &&
                    right == cr.right &&
                    bottom == cr.bottom
                    );
            }
            bool operator!=(const padding& cr) const { return !(cr == *this); }
        };

    }
}