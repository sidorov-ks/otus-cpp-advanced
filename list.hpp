#pragma once

#include <memory>

template<typename T>
struct node {
  T data;
  std::shared_ptr<node> next;
//
//  node(T const &data, node* next) : data(data), next(next) {}
//
//  node(T &&data, node* next) : data(std::move(data)), next(next) {}
};

template<typename T>
struct iter {
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
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
  using alloc_type = typename std::allocator_traits<Alloc>::template rebind_traits<node<T>>;

//  list<T>(alloc_type alloc = alloc_type()) : _alloc(alloc) {}
  list() {}

//  ~list() {
//    node<T>* tmp;
//    while(_head) {
//      tmp = _head;
//      _head = _head->next;
//      delete tmp;
//    }
//  }

  void push_front(T &&data);

  void push_front(T const &data);

  using iterator = iter<T>;

  iterator begin() const;

  iterator end() const;

  std::size_t size() const;

  bool empty() const;

private:
  std::shared_ptr<node<T>> _head;
  std::size_t _elements = 0;

//  alloc_type _alloc;
};

template<typename T, typename Alloc>
void list<T, Alloc>::push_front(T &&data) {
  auto *new_head_ptr = new node<T>;
  new_head_ptr->data = std::move(data);
  new_head_ptr->next = _head;
  std::shared_ptr<node<T>> new_head{new_head_ptr};
  _head.swap(new_head);
  _elements += 1;
}

template<typename T, typename Alloc>
void list<T, Alloc>::push_front(T const &data) {
  assert(false);
//  node<T> new_head {data, _head};
//  this->_head = &new_head;
//  this->_elements += 1;
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
std::size_t list<T, Alloc>::size() const {
  return _elements;
}

template<typename T, typename Alloc>
bool list<T, Alloc>::empty() const {
  return size() == 0;
}