#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QGroupBox>
#include <QTreeView>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <renderer.h>
#include <guile_thread.h>
#include <editorController.h>

#include <ui_main.h>
#include <doc-server.h>

class editorController;

class MainWindow: public QMainWindow {
  Q_OBJECT
public:
  MainWindow(int argc, char** argv);
  ~MainWindow();

  void registerController(editorController *ec);

  void newTileset();

  Ui::MainWindow ui;

  document_server s;
  
  Renderer *map_view, *tileset_view, *tileview;
  Guile_Thread t;
  editorController *ec;
  QGroupBox* toolbox();

  void setupTree();

  QTreeView tree;

  void setupTreeCtxMenu();
  void editObject();

  QString getTextureLocation();

  QPushButton *server_connection;

  void prepare_server_button(QVBoxLayout *toolbox_layout);
  void prepareResizeBtn(QVBoxLayout *toolbox_layout);
  void prepareStartEngine(QVBoxLayout *toolbox_layout);
};

#endif //MAIN_H
