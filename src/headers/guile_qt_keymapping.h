#ifndef GUILE_QT_KEYMAPPING_H
#define GUILE_QT_KEYMAPPING_H

#include <guile_fn.h>
#include <unordered_map>
#include <Qt>

extern std::unordered_map<cl_object, Qt::Key> cl_qt_key_pairs;
void init_keypairs() ;


#endif //GUILE_QT_KEYMAPPING_H
