
template<typename T1, typename T2>
class Tuple {
public:
    Tuple(T1 first, T2 second) : first(first), second(second) {}
    T1 first;
    T2 second;

    static Tuple<T1, T2> make(T1 first, T2 second) {
        return Tuple<T1, T2>(first, second);
    }

    void split(T1& first, T2& second) {
        first = this->first;
        second = this->second;
    }

    bool operator==(const Tuple<T1, T2>& other) const {
        return first == other.first && second == other.second;
    }

    bool operator!=(const Tuple<T1, T2>& other) const {
        return !(*this == other);
    }

    bool operator<(const Tuple<T1, T2>& other) const {
        if (first < other.first) {
            return true;
        } else if (first == other.first) {
            return second < other.second;
        }
        return false;
    }

    bool operator>(const Tuple<T1, T2>& other) const {
        return !(*this < other || *this == other);
    }

    bool operator<=(const Tuple<T1, T2>& other) const {
        return *this < other || *this == other;
    }

    bool operator>=(const Tuple<T1, T2>& other) const {
        return *this > other || *this == other;
    }
};