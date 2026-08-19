#pragma once
#include <functional>
template <typename T>
class Option {
public:
    Option()
        : has_(false), value_() {}

    Option(T value)
        : has_(true), value_(value) {}

    bool has_value() const {
        return has_;
    }

    explicit operator bool() const {
        return has_;
    }

    const T &operator*() const {
        return value_;
    }

    Option &operator=(const T &t) {
        has_ = true;
        value_ = t;
        return *this;
    }

    bool operator==(const T& t) const {
        if (!has_) return false;
        return value_ == t;
    }

    T &operator*() {
        return value_;
    }

    T value_or(T fallback) const {
        return has_ ? value_ : fallback;
    }

    void reset() {
        has_ = false;
        value_ = T();
    }

    // Converting assignment: accepts anything T can be constructed from
    // (e.g. assigning a const char* literal to an Option<std::string>).
    template <typename U>
    Option &operator=(const U &v) {
        has_ = true;
        value_ = v;
        return *this;
    }

    void handle_if(std::function<void(const T &)> handler) const {
        if (has_) {
            handler(value_);
        }
    }

private:
    bool has_;
    T value_;
};