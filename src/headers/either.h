#ifndef EITHER_H
#define EITHER_H

template<typename T, typename E>
struct either
{
  T a;
  E b;
};

template<typename T, typename E>
bool operator==(either<T, E> a, either<T, E> b) {
  return a.a == b.a && a.b == b.b;
}

template<typename T, typename E>
bool operator!=(either<T, E> a, either<T, E> b) {
  return !(a == b);
}

#endif //EITHER_H
