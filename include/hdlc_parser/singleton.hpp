#pragma once

template<typename T>
class Singleton {
 public:
  static T& getInstance() {
    static T instance;
    return instance;
  }

 private:
  Singleton() = default;
  ~Singleton() = default;
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
};