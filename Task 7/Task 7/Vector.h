#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <functional>

template <typename T> class Vector;
template <typename T> std::ostream& operator<<(std::ostream& os, const Vector<T>& v);

/**
 * @brief Проверка на поддержку действия у данных типов данных.
 * @return True если поддерживается
 *
 * Проверяет поддерживают ли данне данный тип операции.
 */
template<typename A, typename B, typename Op>
constexpr bool supports_operator() {
    return requires(A a, B b) { Op{}(a, b); };
}


/**
 * @class Vector
 * @brief Шаблонный класс динамического массива, имитирующий базовую функциональность std::vector.
 *
 * Этот класс предоставляет упрощённую реализацию динамического массива,
 * поддерживающую изменение размера, добавление и удаление элементов, сравнение,
 * а также поэлементные операции над векторами одного типа.
 * Также включает перегруженные операторы для удобства.
 */
template <typename T>
class Vector {
private:
    T* data; ///< Указатель на динамически выделенный массив.
    size_t size; ///< Текущее количество элементов в векторе.
    size_t capacity; ///< Текущая ёмкость выделенного массива.

    /**
     * @brief Изменяет размер внутреннего массива на новую ёмкость.
     * @param new_capacity Новая ёмкость для массива.
     *
     * Выделяет новый массив, копирует существующие элементы и освобождает старый массив.
     * Эта функция обрабатывает перераспределение памяти.
     */
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity]();
        try {
            std::copy(data, data + size, new_data);
        }
        catch (...) {
            delete[] new_data;
            throw;
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    /**
     * @brief Конструктор по умолчанию.
     *
     * Инициализирует пустой вектор с начальной ёмкостью 10.
     */
    Vector() : size(0), capacity(10) {
        data = new T[capacity]();
    }

    /**
     * @brief Конструктор с начальным размером.
     * @param initial_size Начальное количество элементов.
     *
     * Инициализирует вектор заданного размера, элементы конструируются по умолчанию.
     */
    Vector(size_t initial_size) : size(initial_size), capacity(initial_size * 2) {
        data = new T[capacity]();
    }

    /**
     * @brief Конструктор копирования.
     * @param other Вектор, из которого копировать.
     *
     * Создаёт глубокую копию предоставленного вектора.
     */
    Vector(const Vector& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity]();
        try {
            std::copy(other.data, other.data + size, data);
        }
        catch (...) {
            delete[] data;
            throw;
        }
    }

    /**
     * @brief Оператор присваивания.
     * @param other Вектор, из которого копировать.
     * @return Ссылка на текущий вектор.
     *
     * Создаёт глубокую копию предоставленного вектора.
     */
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            T* new_data = new T[other.capacity]();
            try {
                std::copy(other.data, other.data + other.size, new_data);
            }
            catch (...) {
                delete[] new_data;
                throw;
            }
            delete[] data;
            data = new_data;
            size = other.size;
            capacity = other.capacity;
        }
        return *this;
    }

    /**
     * @brief Деструктор.
     *
     * Освобождает динамический массив.
     */
    ~Vector() {
        delete[] data;
    }

    /**
     * @brief Добавляет элемент в конец вектора.
     * @param val Значение для добавления.
     */
    void push_back(const T& val) {
        if (size >= capacity) {
            resize(capacity * 2);
        }
        data[size++] = val;
    }

    /**
     * @brief Добавляет элемент в начало вектора.
     * @param val Значение для добавления.
     */
    void push_front(const T& val) {
        insert(0, val);
    }

    /**
     * @brief Вставляет элемент в указанную позицию.
     * @param pos Позиция для вставки (с нуля).
     * @param val Значение для вставки.
     *
     * Если pos >= size, вставляет в конец.
     */
    void insert(size_t pos, const T& val) {
        if (pos > size) {
            pos = size;
        }
        if (size >= capacity) {
            resize(capacity * 2);
        }
        std::copy_backward(data + pos, data + size, data + size + 1);
        data[pos] = val;
        ++size;
    }

    /**
     * @brief Удаляет элемент в указанной позиции.
     * @param pos Позиция для удаления (с нуля).
     *
     * Ничего не делает, если pos >= size.
     */
    void erase(size_t pos) {
        if (pos >= size) return;
        std::copy(data + pos + 1, data + size, data + pos);
        --size;
    }

    /**
     * @brief Возвращает текущий размер вектора.
     * @return Количество элементов.
     */
    size_t get_size() const {
        return size;
    }

    /**
     * @brief Доступ к элементу по индексу (константная версия).
     * @param index Индекс для доступа.
     * @return Константная ссылка на элемент.
     *
     * Без проверки границ.
     */
    const T& operator[](size_t index) const {
        return data[index];
    }

    /**
     * @brief Доступ к элементу по индексу.
     * @param index Индекс для доступа.
     * @return Ссылка на элемент.
     *
     * Без проверки границ.
     */
    T& operator[](size_t index) {
        return data[index];
    }

    /**
     * @brief Проверяет два вектора на равенство.
     * @param other Вектор для сравнения.
     * @return True, если размеры и элементы совпадают, иначе false.
     */
    template <typename U>
    bool operator==(const Vector<U>& other) const {
        using Op = std::equal_to<>;

        if constexpr (!supports_operator<T, U, Op>()) {
            std::cerr << "Multiplication not supported between these types.\n";
            return false;
        }
        else {
            if (size != other.get_size()) {
                std::cerr << "Vectors must be same size for multiplication.\n";
                return false;
            }

            using R = decltype(std::declval<T>()== std::declval<U>());
            Vector<R> result;
            for (size_t i = 0; i < size; ++i)
                if (data[i] != other.data[i]) return false;
            return true;
        }
    }

    /**
     * @brief Выполняет поэлементное сложение двух векторов.
     * @param other Вектор для сложения.
     * @return Новый вектор с суммированными элементами.
     *
     * Требует одинаковых размеров; возвращает пустой вектор, если размеры отличаются.
     * Предполагает, что T поддерживает operator+.
     */
    template <typename U>
    Vector operator+(const Vector<U>& other) const {
        using Op = std::plus<>;

        if constexpr (!supports_operator<T, U, Op>()) {
            std::cerr << "Addition not supported between these types.\n";
            return Vector<T>();
        }
        else {
            if (size != other.get_size()) {
                std::cerr << "Vectors must be same size for addition.\n";
                return Vector<T>();
            }

            using R = decltype(std::declval<T>() + std::declval<U>());
            Vector<R> result;
            for (size_t i = 0; i < size; ++i)
                result.push_back(data[i] + other[i]);
            return result;
        }
    }

    /**
     * @brief Выполняет поэлементное умножение двух векторов.
     * @param other Вектор для умножения.
     * @return Новый вектор с умноженными элементами.
     *
     * Требует одинаковых размеров; возвращает пустой вектор, если размеры отличаются.
     * Предполагает, что T поддерживает operator*.
     */
    template <typename U>
    Vector operator*(const Vector<U>& other) const {
        using Op = std::multiplies<>;

        if constexpr (!supports_operator<T, U, Op>()) {
            std::cerr << "Multiplication not supported between these types.\n";
            return Vector<T>();
        }
        else {
            if (size != other.get_size()) {
                std::cerr << "Vectors must be same size for multiplication.\n";
                return Vector<T>();
            }

            using R = decltype(std::declval<T>()* std::declval<U>());
            Vector<R> result;
            for (size_t i = 0; i < size; ++i)
                result.push_back(data[i] * other[i]);
            return result;
        }
    }

    // Объявление дружественной функции для оператора вывода
    friend std::ostream& operator<< <T> (std::ostream& os, const Vector<T>& v);
};

/**
 * @brief Оператор вывода для печати вектора в поток.
 * @param os Выходной поток.
 * @param v Вектор для печати.
 * @return Ссылка на выходной поток.
 *
 * Печатает вектор в формате: [1, 2, 3]
 * Предполагает, что T поддерживает operator<<.
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
    os << "[";
    for (size_t i = 0; i < v.size; ++i) {
        os << v.data[i];
        if (i < v.size - 1) os << ", ";
    }
    os << "]";
    return os;
}

#endif // VECTOR_H