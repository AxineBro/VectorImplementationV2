#include "Vector.h"
#include <iostream>
#include <vector>
#include <limits>
#include <variant>
#include <string>

/**
* Тип для хранения векторов разного типа данных.
*/
using VectorVariant = std::variant<
    Vector<int>,
    Vector<long>,
    Vector<double>,
    Vector<float>,
    Vector<char>,
    Vector<std::string>>;

/**
* @brief Напечатать меню операций.
* 
* Печатает меню операций которые можно выполнить с вектором.
*/
void printMenu() {
    std::cout << "\n=== Vector Operations Menu ===" << std::endl;
    std::cout << "1. Create new vector" << std::endl;
    std::cout << "2. Push back element" << std::endl;
    std::cout << "3. Push front element" << std::endl;
    std::cout << "4. Insert element at position" << std::endl;
    std::cout << "5. Erase element at position" << std::endl;
    std::cout << "6. Display current vector" << std::endl;
    std::cout << "7. Vector addition" << std::endl;
    std::cout << "8. Vector multiplication" << std::endl;
    std::cout << "9. Compare vectors" << std::endl;
    std::cout << "10. Show all vectors" << std::endl;
    std::cout << "11. Change current vector" << std::endl;
    std::cout << "12. Delete vector" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

/**
* @brief Напечатать все существующие векторы.
 * @param vectors Вектор, содержащий варианты векторов разных типов.
 *        Используется для хранения всех созданных векторов.
 * @param currentIndex Индекс текущего активного вектора.
 *        Если индекс выходит за границы, будет выведено сообщение об ошибке.
*
* Печатает меню операций которые можно выполнить с вектором.
*/
void showAllVectors(const std::vector<VectorVariant>& vectors, int currentVectorIndex) {
    if (vectors.empty()) {
        std::cout << "No vectors created yet!" << std::endl;
        return;
    }

    std::cout << "\nAll vectors:" << std::endl;
    for (size_t i = 0; i < vectors.size(); ++i) {
        bool isCurrent = (currentVectorIndex == static_cast<int>(i));
        std::cout << "Vector #" << i;

        std::visit([isCurrent](auto&& vec) {
            using T = std::decay_t<decltype(vec)>;
            if constexpr (std::is_same_v<T, Vector<int>>) std::cout << " [int]: ";
            else if constexpr (std::is_same_v<T, Vector<long>>) std::cout << " [long]: ";
            else if constexpr (std::is_same_v<T, Vector<double>>) std::cout << " [double]: ";
            else if constexpr (std::is_same_v<T, Vector<float>>) std::cout << " [float]: ";
            else if constexpr (std::is_same_v<T, Vector<char>>) std::cout << " [char]: ";
            else if constexpr (std::is_same_v<T, Vector<std::string>>) std::cout << " [string]: ";

            std::cout << vec << " (size: " << vec.get_size() << ")"
                << (isCurrent ? " [CURRENT]" : "") << std::endl;
            }, vectors[i]);
    }
}

/**
* @brief Создать новый вектор.
* @return VectorVariant Тип с векторами разного типа.
*
* Создает новый веткор типа, выбранного пользователем.
*/
VectorVariant createVector() {
    std::cout << "Select type:\n"
        << "1 - int\n"
        << "2 - long\n"
        << "3 - double\n"
        << "4 - float\n"
        << "5 - char\n"
        << "6 - string\n"
        << "Enter choice: ";
    int typeChoice;
    std::cin >> typeChoice;

    size_t size = 0;
    std::cout << "Enter initial size (0 for empty): ";
    std::cin >> size;

    switch (typeChoice) {
    case 1: return (size == 0) ? Vector<int>() : Vector<int>(size);
    case 2: return (size == 0) ? Vector<long>() : Vector<long>(size);
    case 3: return (size == 0) ? Vector<double>() : Vector<double>(size);
    case 4: return (size == 0) ? Vector<float>() : Vector<float>(size);
    case 5: return (size == 0) ? Vector<char>() : Vector<char>(size);
    case 6: return (size == 0) ? Vector<std::string>() : Vector<std::string>(size);
    default:
        std::cout << "Invalid type, defaulting to int.\n";
        return (size == 0) ? Vector<int>() : Vector<int>(size);
    }
}


/**
 * @brief Создать новый вектор.
 *
 * @param vectors Вектор, содержащий варианты векторов разных типов.
 *        Используется для хранения всех созданных векторов.
 * @param currentIndex Индекс текущего активного вектора.
 *        Если индекс выходит за границы, будет выведено сообщение об ошибке.
 * @param func Лямбда-функция, применяемая к текущему вектору.
 *        Передается в std::visit для выполнения операции над содержимым.
 *
 * Функция проверяет корректность индекса и вызывает std::visit для
 * применения переданной функции `func` к текущему элементу вектора `vectors`.
 */
template <typename Func>
void visitCurrentVector(std::vector<VectorVariant>& vectors, int currentIndex, Func func) {
    if (currentIndex < 0 || currentIndex >= static_cast<int>(vectors.size())) {
        std::cout << "No vector selected!\n";
        return;
    }
    std::visit(func, vectors[currentIndex]);
}

int main() {
    std::vector<VectorVariant> vectors;
    int currentVectorIndex = -1;
    int choice;

    std::cout << "Welcome to Vector Interactive Program!" << std::endl;

    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: {
            VectorVariant vec = createVector();
            vectors.push_back(vec);
            currentVectorIndex = vectors.size() - 1;
            std::cout << "Created vector #" << currentVectorIndex << std::endl;
            break;
        }

        case 2: {
            visitCurrentVector(vectors, currentVectorIndex, [](auto& vec) {
                using T = typename std::decay_t<decltype(vec)>::value_type;
                T value;
                std::cout << "Enter value to push_back: ";

                if constexpr (std::is_same_v<T, std::string>) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, value);
                }
                else {
                    std::cin >> value;
                }

                vec.push_back(value);
                std::cout << "Result: " << vec << std::endl;
            });
            break;
        }

        case 3: {
            visitCurrentVector(vectors, currentVectorIndex, [](auto& vec) {
                using T = typename std::decay_t<decltype(vec)>::value_type;
                T value;
                std::cout << "Enter value to push_front: ";
                
                if constexpr (std::is_same_v<T, std::string>) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, value);
                }
                else {
                    std::cin >> value;
                }

                vec.push_front(value);
                std::cout << "Result: " << vec << std::endl;
            });
            break;
        }

        case 4: {
            visitCurrentVector(vectors, currentVectorIndex, [](auto& vec) {
                using T = typename std::decay_t<decltype(vec)>::value_type;
                size_t pos;
                T value;
                std::cout << "Enter position: ";
                std::cin >> pos;
                std::cout << "Enter value: ";

                if constexpr (std::is_same_v<T, std::string>) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, value);
                }
                else {
                    std::cin >> value;
                }

                vec.insert(pos, value);
                std::cout << "Result: " << vec << std::endl;
            });
            break;
        }

        case 5: {
            visitCurrentVector(vectors, currentVectorIndex, [](auto& vec) {
                size_t pos;
                std::cout << "Enter position to erase: ";
                std::cin >> pos;
                vec.erase(pos);
                std::cout << "Result: " << vec << std::endl;
                });
            break;
        }

        case 6: {
            if (currentVectorIndex == -1)
                std::cout << "No vector selected.\n";
            else
                showAllVectors(vectors, currentVectorIndex);
            break;
        }

        case 7:
        case 8: {
            if (vectors.size() < 2) {
                std::cout << "Need at least 2 vectors.\n";
                break;
            }

            showAllVectors(vectors, currentVectorIndex);
            int i1, i2;
            std::cout << "Enter first vector index (0-" << vectors.size() - 1 << "): ";
            std::cin >> i1;
            std::cout << "Enter second vector index (0-" << vectors.size() - 1 << "): ";
            std::cin >> i2;

            if (i1 < 0 || i1 >= vectors.size() || i2 < 0 || i2 >= vectors.size()) {
                std::cout << "Invalid indices.\n";
                break;
            }

            VectorVariant result;
            bool success = false;

            std::visit([&](auto& v1) {
                using T1 = std::decay_t<decltype(v1)>;
                std::visit([&](auto& v2) {
                    using T2 = std::decay_t<decltype(v2)>;

                    if constexpr (std::is_same_v<typename T1::value_type, typename T2::value_type>) {
                        if constexpr (requires(typename T1::value_type a, typename T2::value_type b) { a* b; a + b; }) {
                            if (choice == 7)
                                result = v1 + v2;
                            else
                                result = v1 * v2;
                            success = true;
                        }
                        else {
                            std::cout << "This type does not support arithmetic operations.\n";
                        }
                    }
                    }, vectors[i2]);
                }, vectors[i1]);

            if (success) {
                vectors.push_back(result);
                currentVectorIndex = vectors.size() - 1;
                std::cout << "Result saved as vector #" << currentVectorIndex << std::endl;
            }
            else {
                std::cout << "Type mismatch or unsupported operation.\n";
            }

            break;
        }

        case 9: {
            if (vectors.size() < 2) {
                std::cout << "Need at least 2 vectors.\n";
                break;
            }

            showAllVectors(vectors, currentVectorIndex);
            int i1, i2;
            std::cout << "Enter first vector index (0-" << vectors.size() - 1 << "): ";
            std::cin >> i1;
            std::cout << "Enter second vector index (0-" << vectors.size() - 1 << "): ";
            std::cin >> i2;

            bool equal = false;

            std::visit([&](auto& v1) {
                using T1 = std::decay_t<decltype(v1)>;
                std::visit([&](auto& v2) {
                    using T2 = std::decay_t<decltype(v2)>;
                    if constexpr (std::is_same_v<typename T1::value_type, typename T2::value_type>)
                        equal = (v1 == v2);
                    }, vectors[i2]);
                }, vectors[i1]);

            std::cout << (equal ? "Equal" : "Not equal") << std::endl;
            break;
        }

        case 10:
            showAllVectors(vectors, currentVectorIndex);
            break;

        case 11: {
            showAllVectors(vectors, currentVectorIndex);
            std::cout << "Enter index to select: ";
            int idx; std::cin >> idx;
            if (idx >= 0 && idx < vectors.size()) currentVectorIndex = idx;
            else std::cout << "Invalid index.\n";
            break;
        }

        case 12: {
            if (vectors.empty()) {
                std::cout << "No vectors.\n";
                break;
            }
            showAllVectors(vectors, currentVectorIndex);
            int idx;
            std::cout << "Enter index to delete: ";
            std::cin >> idx;
            if (idx >= 0 && idx < vectors.size()) {
                vectors.erase(vectors.begin() + idx);
                currentVectorIndex = vectors.empty() ? -1 : 0;
                std::cout << "Vector deleted.\n";
            }
            else std::cout << "Invalid index.\n";
            break;
        }

        case 0:
            std::cout << "Goodbye!\n";
            break;

        default:
            std::cout << "Invalid choice.\n";
            break;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (choice != 0);

    return 0;
}
