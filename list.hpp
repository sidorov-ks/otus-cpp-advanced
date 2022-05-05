#pragma once

#include <memory>

template<typename T>
struct node {
  T data;
  std::shared_ptr<node> next;

  node() : data(), next() {}

  node(T data, std::shared_ptr<node> next) : data(data), next(next) {}
};

template<typename T>
struct iter {
  using pointer_type = T *;
  using reference_type = T &;

  iter(std::shared_ptr<node<T>> &&_node) : _node(_node) {}

  reference_type operator*() const { return _node->data; }

  pointer_type operator->() { return _node->data; }

  iter &operator++() {
    _node = _node->next;
    return *this;
  }

  friend bool operator==(const iter &a, const iter &b) { return a._node == b._node; };

  friend bool operator!=(const iter &a, const iter &b) { return a._node != b._node; };
private:
  std::shared_ptr<node<T>> _node;
};

template<typename T, typename Alloc = std::allocator<T>>
class list {
public:
  using alloc_type = typename Alloc::template rebind<node<T>>::other;

  list() : _alloc() {}

  ~list() { clear(); }

  void push_front(T &&data);

  using iterator = iter<T>;

  iterator begin() const;

  iterator end() const;

  void clear();

  [[nodiscard]] std::size_t size() const;

  [[nodiscard]] bool empty() const;

private:
  std::shared_ptr<node<T>> _head;
  std::size_t _elements = 0;
  alloc_type _alloc;
};

template<typename T, typename Alloc>
void list<T, Alloc>::push_front(T &&data) {
  auto *new_head_ptr = std::allocator_traits<alloc_type>::allocate(_alloc, 1);
  std::allocator_traits<alloc_type>::construct(_alloc, new_head_ptr, std::move(data), _head);
  std::shared_ptr<node<T>> new_head{new_head_ptr};
  _head.swap(new_head);
  _elements += 1;
}

template<typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() const {
  std::shared_ptr<node<T>> head_ptr{_head};
  return iter<T>{std::move(head_ptr)};
}

template<typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() const {
  return iter<T>{nullptr};
}

template<typename T, typename Alloc>
void list<T, Alloc>::clear() {
  while (_elements > 0) {
    auto next_head = _head->next;
    std::allocator_traits<alloc_type>::destroy(_alloc, &_head->data);
    // FIXME Здесь падает очистка списка при использовании linear_allocator
    _head = next_head;
    _elements--;
  }
}

template<typename T, typename Alloc>
std::size_t list<T, Alloc>::size() const {
  return _elements;
}

template<typename T, typename Alloc>
bool list<T, Alloc>::empty() const {
  return size() == 0;
}