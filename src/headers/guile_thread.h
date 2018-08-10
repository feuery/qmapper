#ifndef GUILE_THREAD_H
#define GUILE_THREAD_H

class lisp_loader {
public:

  void load_lisp();

  bool running = false;
};

#endif //GUILE_THREAD_H
