#pragma once
#include <sstream>

namespace xeekworx {
    namespace types2d {

        template <typename T = float>
        class corner_radius {
        private:
            T topleft, topright, bottomright, bottomleft;

        public:
            corner_radius() : corner_radius(T(), T(), T(), T()) {}
            corner_radius(const T topleft, const T topright, const T bottomright, const T bottomleft) :
                topleft(topleft),
                topright(topright),
                bottomright(bottomright),
                bottomleft(bottomleft) {}
            corner_radius(const corner_radius& cr) : corner_radius(cr.topleft, cr.topright, cr.bottomright, cr.bottomleft) {}
            corner_radius(const T radius) : corner_radius(radius, radius, radius, radius) {}
            corner_radius(const T * array) : corner_radius(array[0], array[1], array[2], array[3]) {}

            virtual ~corner_radius() {}

            void clear() { topleft = topright = bottomright = bottomleft = T(); }

            void set(const T topleft, const T topright, const T bottomright, const T bottomleft)
            {
                *this = corner_radius(topleft, topright, bottomright, bottomleft);
            }

            void set(const T radius)
            {
                *this = corner_radius(radius);
            }

            void offset(const T topleft, const T topright, const T bottomright, const T bottomleft)
            {
                this->topleft += topleft;
                this->topright += topright;
                this->bottomright += bottomright;
                this->bottomleft += bottomleft;
            }

            void offset(const T value)
            {
                offset(value, value, value, value);
            }

            bool empty() const
            {
                if (!topleft && !topright && !bottomright && !bottomleft) return true;
                return false;
            }

            std::string str() const
            {
                std::stringstream ss;
                ss << topleft << ", " << topright << ", " << bottomright << ", " << bottomleft;
                return ss.str();
            }

            void operator+=(const T value) { offset(value); }
            void operator-=(const T value) { offset(-value); }
            bool operator==(const corner_radius& cr) const
            {
                return (
                    topleft == cr.topleft &&
                    topright == cr.topright &&
                    bottomright == cr.bottomright &&
                    bottomleft == cr.bottomleft
                    );
            }
            bool operator!=(const corner_radius& cr) const { return !(cr == *this); }
        };

    }
}