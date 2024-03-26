#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

const int BASE = 10;

const int log_base = static_cast<int>(log10(BASE));

class bigint {
    std::vector<int> digits;

public:
    bigint() {
        digits.push_back(0);
    }

    explicit bigint(const std::string &s) {
        for (int i = static_cast<int>(s.size()) - 1; i >= 0; i--) {
            assert(isdigit(s[i]));
        }
        for (int i = static_cast<int>(s.size()) - 1; i >= 0; i -= log_base) {
            std::string tmp;
            if (i >= log_base) {
                tmp = s.substr(i - log_base + 1, log_base);
            } else {
                tmp = s.substr(0, i + 1);
            }
            digits.push_back(std::stoi(tmp));
        }
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

    // cppcheck-suppress noExplicitConstructor
    // NOLINTNEXTLINE
    bigint(unsigned int value) {
        while (value > 0) {
            digits.push_back(static_cast<int>(value) % BASE);
            value /= BASE;
        }
    }

    explicit operator unsigned int() const {
        unsigned int result = 0;
        unsigned int base = 1;
        for (auto el : digits) {
            result += el * base;
            base *= BASE;
        }
        return result;
    }

    static std::vector<int> remove_zeros(std::vector<int> c) {
        std::reverse(c.begin(), c.end());
        int index = 0;
        for (int i = 0; i < static_cast<int>(c.size()); i++) {
            if (c[i] != 0) {
                index = i;
                break;
            }
        }
        if (index == 0 && c.size() > 1 && c[0] == 0) {
            c.erase(c.begin(), c.begin() + static_cast<int>(c.size() - 1));
        } else {
            c.erase(c.begin(), c.begin() + index);
        }
        std::reverse(c.begin(), c.end());
        return c;
    }

    [[nodiscard]] std::string to_string() const {
        std::string tmp;
        for (int i = static_cast<int>(digits.size()) - 1; i >= 0; i--) {
            tmp += std::to_string(digits[i]);
        }
        return tmp;
    }

    [[nodiscard]] friend bigint operator+(const bigint &x, const bigint &y) {
        bigint a = std::max(x, y);
        bigint b = std::min(x, y);
        size_t max_len = a.digits.size();
        size_t min_len = b.digits.size();
        for (unsigned int i = 0; i < min_len; i++) {
            a.digits[i] += b.digits[i];
        }
        for (unsigned int i = 0; i < max_len - 1; i++) {
            if (a.digits[i] >= BASE) {
                a.digits[i] -= BASE;
                a.digits[i + 1]++;
            }
        }
        a.digits = remove_zeros(a.digits);
        return a;
    }

    [[nodiscard]] friend bigint operator-(const bigint &x, const bigint &y) {
        bigint a = std::max(x, y);
        bigint b = std::min(x, y);
        size_t max_len = a.digits.size();
        size_t min_len = b.digits.size();
        for (unsigned int i = 0; i < min_len; i++) {
            a.digits[i] -= b.digits[i];
        }
        for (unsigned int i = 0; i < max_len - 1; i++) {
            if (a.digits[i] < 0) {
                a.digits[i] += BASE;
                a.digits[i + 1]--;
            }
        }
        a.digits = remove_zeros(a.digits);
        return a;
    }

    bigint &operator+=(const bigint &other) {
        return *this = *this + other;
    }

    bigint &operator-=(const bigint &other) {
        return *this = *this - other;
    }

    friend bool operator==(const bigint &x, const bigint &y) {
        return x.digits == y.digits;
    }

    friend bool operator!=(const bigint &x, const bigint &y) {
        return !(x.digits == y.digits);
    }

    bigint &operator++() {
        return *this += 1;
    }

    // NOLINTNEXTLINE
    bigint operator++(int) {
        bigint old = *this;
        *this += 1;
        return old;
    }

    bigint &operator--() {
        return *this -= 1;
    }

    // NOLINTNEXTLINE
    bigint operator--(int) {
        bigint old = *this;
        *this -= 1;
        return old;
    }

    friend bool operator<(bigint x, bigint y) {
        if (x.digits.size() != y.digits.size()) {
            return x.digits.size() < y.digits.size();
        } else if (x == y) {
            return false;
        }
        for (int i = static_cast<int>(x.digits.size()) - 1; i >= 0; i--) {
            if (x.digits[i] < y.digits[i]) {
                return true;
            } else if (x.digits[i] > y.digits[i]) {
                return false;
            }
        }
        return false;
    }

    friend bool operator>(const bigint &x, const bigint &y) {
        return y < x;
    }

    friend bool operator<=(const bigint &x, const bigint &y) {
        return x < y || x == y;
    }

    friend bool operator>=(const bigint &x, const bigint &y) {
        return x > y || x == y;
    }

    friend std::ostream &operator<<(std::ostream &os, const bigint &b) {
        os << b.to_string();
        return os;
    }

    friend std::istream &operator>>(std::istream &is, bigint &b) {
        std::string res;
        is >> res;
        b = bigint(res);
        return is;
    }
};
