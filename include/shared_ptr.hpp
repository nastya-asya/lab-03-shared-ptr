// Copyright 2020 Anastasiya Smirnova nastya.asya08@yandex.ru

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <atomic>
#include <iostream>
#include <utility>

template <typename T>
class SharedPtr {
 public:
  SharedPtr();
  explicit SharedPtr(T* ptr);
  SharedPtr(const SharedPtr& r);
  SharedPtr(SharedPtr&& r);
  ~SharedPtr();

  auto operator=(const SharedPtr& r) -> SharedPtr&;
  auto operator=(SharedPtr&& r) -> SharedPtr&;

  operator bool() const;
  auto operator*() const -> T&;
  auto operator->() const -> T*;

  auto get() const -> T*;
  void reset();
  void reset(T* ptr);
  void swap(SharedPtr& r);

  auto useCount() const -> size_t;

 private:
  T* object;
  std::atomic_uint* count_of_links;
};

template <typename T>
SharedPtr<T>::SharedPtr() : object(nullptr), count_of_links(nullptr) {}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& r) : object(r.object) {
  this->count_of_links = r.count_of_links;
  ++*count_of_links;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& r)
    : object(r.object), count_of_links(r.count_of_links) {
  ++*count_of_links;
}

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr)
    : object(ptr), count_of_links(new std::atomic<uint>) {
  *count_of_links = 1;
}

template <typename T>
SharedPtr<T>::~SharedPtr<T>() {
  if (count_of_links) {
    if (*count_of_links == 1) {
      delete count_of_links;
      count_of_links = nullptr;
      object = nullptr;
    } else {
      --(*count_of_links);
    }
  }
}

template <typename T>
SharedPtr<T>::operator bool() const {
  return get();
}

template <typename T>
auto SharedPtr<T>::operator*() const -> T& {
  if (this->object == nullptr) {
    throw std::out_of_range("Object is empty!!!!");
  }
  return *object;
}

template <typename T>
auto SharedPtr<T>::operator->() const -> T* {
  return object;
}

template <typename T>
auto SharedPtr<T>::get() const -> T* {
  return object;
}

template <typename T>
void SharedPtr<T>::reset() {
  if (count_of_links != nullptr) {
    object = nullptr;
    if (*count_of_links != 1) {
      --(*count_of_links);
    } else {
      delete count_of_links;
    }
    count_of_links = nullptr;
  }
}

template <typename T>
void SharedPtr<T>::reset(T* ptr) {
  if (count_of_links != nullptr) {
    --(*count_of_links);
  }
  count_of_links = new std::atomic<uint>;
  *count_of_links = 1;
  object = ptr;
}

template <typename T>
auto SharedPtr<T>::operator=(const SharedPtr& r) -> SharedPtr& {
  object = r.get();
  if (count_of_links != nullptr) {
    --(*count_of_links);
  }
  count_of_links = r.count_of_links;
  ++(*count_of_links);
  return get();
}

template <typename T>
auto SharedPtr<T>::operator=(SharedPtr&& r) -> SharedPtr& {
  object = std::move(r.object);
  if (this->count_of_links != nullptr) {
    --(*count_of_links);
  }
  count_of_links = std::move(r.count_of_links);
  ++(*count_of_links);
  return get();
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr<T>& r) {
  SharedPtr tmp(r);
  r.object = object;
  r.count_of_links = count_of_links;
  object = std::move(tmp.object);
  count_of_links = std::move(tmp.count_of_links);
}

template <typename T>
auto SharedPtr<T>::useCount() const -> size_t {
  return *count_of_links;
}

#endif  // INCLUDE_HEADER_HPP_
