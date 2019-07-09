#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QGroupBox>
#include <QTreeWidget>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <renderer.h>
#include <guile_thread.h>
#include <editorController.h>

#include <ui_main.h>
#include <doc-server.h>
#include <holder.h>

class editorController;

class MainWindow: public QMainWindow {
  Q_OBJECT
public:
  MainWindow(int argc, char** argv);
  ~MainWindow();

  void newTileset();

  Ui::MainWindow ui;

  document_server s;
  
  Renderer *map_view, *tileset_view, *tileview;
  lisp_loader t;
  QGroupBox* toolbox();

  void setupTree();

  QTreeWidget tree;

  void setupTreeCtxMenu();
  void editObject();
  void deleteObject();

  QString getTextureLocation();

  QPushButton *server_connection, *undo_btn;

  void prepare_server_button(QVBoxLayout *toolbox_layout);
  void prepareResizeBtn(QVBoxLayout *toolbox_layout);
  void prepareStartEngine(QVBoxLayout *toolbox_layout);
  void prepare_undo_btn(QVBoxLayout *toolbox);

 private:
    void registerController(editorController *ec);
  void setupMainMenu();
  editorController *ec;
  cl_object findSelectedObject();
  cl_object find_path();
};

#endif //MAIN_H
