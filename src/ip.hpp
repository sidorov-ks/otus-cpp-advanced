#pragma once

#include "custom_type_traits.hpp"

#include <iostream>
#include <type_traits>

/**
 * Структура, реализующая обёртку над `std::ostream`. Форматирует выводимые объекты как IP-адреса (см. также перегрузки метода `operator<<`)
 */
struct ip_proxy {
  /**
   * Конструктор, собирающий объект ip_proxy, который отправляет вывод на поток os
   * @param os Выходной поток, на который будут выводиться IP-адреса
   */
  explicit ip_proxy(std::ostream &os) : os(os) {}

  /**
   * Выводит IP-адрес, заданный в виде строки. Данная реализация выводит входную строку на выходной поток без изменений.
   * @tparam T Тип данных, для которого реализуется логика вывода. Удовлетворяет ограничению из #is_string_v
   * @param val Строка, выводимая на выходной поток
   * @return Объект класса ip_proxy (`*this`)
   */
  template<typename T, typename std::enable_if_t<is_string_v<T>> * = nullptr>
  ip_proxy &operator<<(T const &val) {
    os << val;
    return *this;
  }

  /**
   * Выводит IP-адрес, заданный в виде числа.
   *
   * Данная реализация выводит каждый из четырёх октетов из младших 4 байтов числа, выделяя на каждый октет по 1 байту,
   * начиная с младших октетов. Так, для `val = 2130706433` будет выведен IP-адрес `127.0.0.1`, поскольку `val == (1 + (127 << 24))`.
   * @tparam T Тип данных, для которого реализуется логика вывода. Удовлетворяет ограничению из `std::is_integral_v`
   * @param val Число, выводимое на выходной поток
   * @return Объект класса ip_proxy (`*this`)
   */
  template<typename T, std::enable_if_t<std::is_integral_v<T>> * = nullptr>
  ip_proxy &operator<<(T const &val) {
    for (size_t octet = 1; octet <= 4; ++octet) {
      auto octet_val = (val >> (8 * (4 - octet))) & 0xFF;
      os << octet_val;
      if (octet < 4) os << ".";
    }
    return *this;
  }

  /**
   * Выводит IP-адрес, заданный в виде контейнера. Данная реализация выводит все элементы контейнера как октеты,
   * следуя от старшего до младшего октета в том порядке, в котором они записаны в контейнере.
   * @tparam T Тип данных, для которого реализуется логика вывода. Удовлетворяет ограничению из #is_container_v
   * @param val Контейнер, выводимый на выходной поток
   * @return Объект класса ip_proxy (`*this`)
   */
  template<typename T, std::enable_if_t<is_container_v<T>> * = nullptr>
  ip_proxy &operator<<(T const &val) {
    for (auto it = val.begin(); it != val.end();) {
      os << *it;
      if (++it != val.end()) os << ".";
    }
    return *this;
  }

  /**
   * Выводит IP-адрес, заданный в виде 4-кортежа. Данная реализация выводит все элементы кортежа как октеты,
   * следуя от старшего до младшего октета в том порядке, в котором они записаны в кортеже.
   * @tparam T Тип данных, для которого реализуется логика вывода. Удовлетворяет ограничению из #is_quad_v
   * @param val Кортеж, выводимый на выходной поток
   * @return Объект класса ip_proxy (`*this`)
   */
  template<typename T, std::enable_if_t<is_quad_v<T>> * = nullptr>
  ip_proxy &operator<<(T const &val) {
    auto[octet1, octet2, octet3, octet4] = val;
    os << octet1 << "." << octet2 << "." << octet3 << "." << octet4;
    return *this;
  }

private:
  std::ostream &os;
};

struct ip_proxy_creator {
}
/**
 * Манипулятор выходного потока, который при подстановке настраивает вывод объектов в виде IP-адресов.
 *
 * Так, в коде `std::cout << output_ip << char(-1)` объект `char(-1)` будет выведен как `255.255.255.255`.
 */
output_ip;

/**
 * Операция, применяющая манипулятор output_ip к потоку вывода.
 * @param os Поток вывода, к которому применяется манипулятор
 * @return Объект ip_proxy, который выводит весь дальнейший поток на os в виде IP-адресов
 */
ip_proxy operator<<(std::ostream &os, ip_proxy_creator) {
  return ip_proxy(os);
}
