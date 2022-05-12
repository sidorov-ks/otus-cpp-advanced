#pragma once

#include <type_traits>
#include <string>
#include <list>
#include <vector>
#include <tuple>

/**
 * Флаг, который компилируется в `true` для `std::string` и `char *`, и в `false` во всех остальных случаях
 * @tparam T Тип, для которого выполняется проверка
 */
template<typename T>
inline constexpr bool is_string_v =
        std::is_same_v<std::string, T> ||
        (std::is_array_v<T> && std::is_same_v<char *, typename std::decay<T>::type>) ||
        (std::is_pointer_v<T> && std::is_same_v<char, std::remove_const_t<std::remove_pointer_t<T>>>);

/**
 * Структура, которая наследуется от `std::true_type` для типов `std::list` и `std::vector`, и от `std::false_type` во всех остальных случаях
 * @tparam T Тип, для которого выполняется проверка
 */
template<typename T>
struct is_container : std::false_type {
};
/**
 * Специализация `is_container`, которая наследуется от `std::true_type` для типов `std::list`.
 * @tparam Args Аргументы для шаблона типа `std::list`
 */
template<typename... Args>
struct is_container<std::list<Args...>> : std::true_type {
};
/**
 * Специализация `is_container`, которая наследуется от `std::true_type` для типов `std::vector`.
 * @tparam Args Аргументы для шаблона типа `std::vector`
 */
template<typename... Args>
struct is_container<std::vector<Args...>> : std::true_type {
};

/**
 * Флаг, который компилируется в `true` для `std::vector` и `std::list`, и в `false` во всех остальных случаях
 * @tparam T Тип, для которого выполняется проверка
 */
template<typename T>
inline constexpr bool is_container_v = is_container<T>::value;

/**
 * Структура, которая наследуется от `std::true_type` для 4-кортежей с одинаковым типом элементов
 * (std::tuple<E, E, E, E> для какого-то типа `E`), и от `std::false_type` во всех остальных случаях
 * @tparam T Тип, для которого выполняется проверка
 */
template<typename T>
struct is_quad : std::false_type {
};
/**
 * Специализация `is_quad`, которая наследуется от `std::true_type` для 4-кортежей.
 * @tparam E Тип элементов кортежа
 */
template<typename E>
struct is_quad<std::tuple<E, E, E, E>> : std::true_type {
};

/**
 * Флаг, который компилируется в `true` для 4-кортежей с одинаковым типом элементов
 * (std::tuple<E, E, E, E> для какого-то типа `E`), и в `false` во всех остальных случаях
 * @tparam T Тип, для которого выполняется проверка
 */
template<typename T>
inline constexpr bool is_quad_v = is_quad<T>::value;
