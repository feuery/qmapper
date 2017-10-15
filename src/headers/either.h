#ifndef EITHER_H
#define EITHER_H

template<typename T, typename E>
struct either
{
  T a;
  E b;
};

#endif //EITHER_H
