#ifndef __TUPLE_H__
#define __TUPLE_H__

/// @brief Uma recriacao da std::tuple por causa
/// de relatos de incompatibilidade
/// @tparam T1 O tipo do primeiro item da tupla
/// @tparam T2 O tipo do segundo item da tupla
template<typename T1, typename T2>
class Tuple {
public:

    /// @brief Cria uma tupla com os 2 elementos passados
    Tuple(T1 first, T2 second) : first(first), second(second) {}
    T1 first;
    T2 second;

    /// @brief Metodo estatico para criar uma tupla. Usa o
    /// construtor por baixo. Feito para uma maior simularidade
    /// com a API da std::tuple.
    /// @param first O primeiro item
    /// @param second O segundo item
    /// @return A tupla criada
    static Tuple<T1, T2> make(T1 first, T2 second) {
        return Tuple<T1, T2>(first, second);
    }

    /// @brief Separa a tupla atual nos 2 valores.
    /// @param first A referencia da variavel que recebe o primeiro valor
    /// @param second A referencia da q recebe a segunda
    void split(T1& first, T2& second) {
        first = this->first;
        second = this->second;
    }

    /// @brief Verifica se 2 tuplas sao iguais usando o operador '=='
    /// de seus itens.
    /// @param other A outra tupla a ser avalida
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

#endif
