#include <iostream>
#include <iomanip>
#include <stdexcept>


class blank {
    int _n;
public:
    blank(int n) : _n(n) {
        if (_n < 0) {
            throw std::range_error("number of blank chars can't be negative");
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const blank& b) {
        if (b._n > 0) {
            os << std::setw(b._n) << ' ';
        }
        return os;
    }
};
