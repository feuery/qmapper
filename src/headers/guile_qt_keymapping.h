#ifndef GUILE_QT_KEYMAPPING_H
#define GUILE_QT_KEYMAPPING_H

#include <unordered_map>
#include <libguile.h>
#include <Qt>

// scm_from_utf8_symbol

extern std::unordered_map<SCM, Qt::Key> scm_qt_key_pairs;


#endif //GUILE_QT_KEYMAPPING_H
