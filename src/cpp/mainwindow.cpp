#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <mainwindow.h>
#include <QFileDialog>
#include <propertyEditor.h>

#include <QMessageBox>
#include <editorController_guile.h>

#include <tilesetContainer.h>
#include <tileview_renderer.h>
#include <layerContainer.h>

#include <tilelistmodel.h>

#include <tool_list.h>
#include <resize_obj.h>
#include <animationLoaderUi.h>
#include <engine.h>

#define btnW 200
#define btnH 25

#define setSize(btn) btn->setMaximumWidth(btnW);	\
  btn->setMaximumHeight(btnH);						

#define btn(title) { QPushButton *btn = new QPushButton(title, this);	\
    btn->setMaximumWidth(btnW);						\
    btn->setMaximumHeight(btnH);					\
    toolbox_layout->addWidget(btn); }

#define btn2(title, container) { QPushButton *btn = new QPushButton(title, this); \
    btn->setMaximumWidth(btnW);						\
    btn->setMaximumHeight(btnH);					\
    container->addWidget(btn); }

QString MainWindow::getTextureLocation() {
  return QFileDialog::getOpenFileName(this, "Load texture file", ".", "Image Files (*.png)");
}

void MainWindow::setupTree()
{
  if(ec) {
    tree.setModel(ec->documentTreeModel);
    connect(&tree, &QTreeView::clicked, [&](QModelIndex index) {
	if(!index.isValid()) return;
	Propertierbase *b = static_cast<Propertierbase*>(index.internalPointer());
	std::string type = b->type_identifier();

	if(type == "Tileset") {
	  tilesetContainer *t = static_cast<tilesetContainer*>(b);
	  Renderable* o = static_cast<Renderable*>(t);
	  ec->indexOfChosenTileset = t->getId();
	  tileset_view->getDrawQueue().clear();
	  tileset_view->getDrawQueue().push_back(o);
	}
	else if(type == "Map") {
	  Mapcontainer *m = static_cast<Mapcontainer*>(b);
	  Renderable *o = static_cast<Renderable*>(m);
	  ec->indexOfChosenMap = m->getId();
	  ec->indexOfChosenLayer = 0;
	  map_view->getDrawQueue().clear();
	  map_view->getDrawQueue().push_back(o);
	}
	else if(type == "Layer") {
	  Layercontainer *l = static_cast<Layercontainer*>(b);
	  Mapcontainer *m = static_cast<Mapcontainer*>(l->parent());

	  ec->indexOfChosenMap = m->getId();
	  ec->indexOfChosenLayer = indexOf(m->getLayers(), static_cast<Layer*>(l));

	  map_view->getDrawQueue().clear();
	  map_view->getDrawQueue().push_back(static_cast<Renderable*>(m));
	}
      });
  }
  else {
    puts("ec is null\n");
    throw "";
  }      
}

QGroupBox* MainWindow::toolbox()
{
  QGroupBox *grp = new QGroupBox("Tools", this);
  QVBoxLayout *l = new QVBoxLayout(this);

  for(auto i: tool_cache) {
    QPushButton *btn = new QPushButton(i.first.c_str(), this); 
    btn->setMaximumWidth(btnW);						
    btn->setMaximumHeight(btnH);
    connect(btn, &QPushButton::clicked,
	    [=]()
	    {
	      qDebug() << "Setting " << i.first.c_str() << "as selected tool";
	      editorController::instance->t = tool_cache.at(i.first);
	    });
    l->addWidget(btn);
  }
    
  grp->setLayout(l);
  return grp;
}

void MainWindow::editObject()
{
  QModelIndex l = tree.currentIndex();
  // Then, let's fire a property-editor-dialog with this pointer as parameter
  // And update this pointer's data in-between the model's begin-row-update-thing
  Propertierbase *b = static_cast<Propertierbase*>(l.internalPointer());
  Propertyeditor *p = new Propertyeditor(b, this);
  p->show();
}

void MainWindow::prepare_server_button(QVBoxLayout *toolbox_layout)
{
  server_connection = new QPushButton("Start documentation server");
  setSize(server_connection);

  toolbox_layout->addWidget(server_connection);

  connect(server_connection, &QPushButton::clicked,
	  [&]() {
	    if(!s.initialized && s.start_server()) {
	      server_connection->setText("Server running");
	    }
	  });
}

void MainWindow::setupTreeCtxMenu()
{
  QAction *edit = new QAction("&Edit", this);
  edit->setShortcuts(QKeySequence::New);
  edit->setStatusTip("Edit object");
  connect(edit, &QAction::triggered, this, &MainWindow::editObject);

  // TODO
  // Add a menu:
  //       new -> glsl script	
  //           -> scheme script
  //           -> ------------
  //           -> texture
  //           -> ------------			
  //           -> sprite
  //           -> animation

  tree.setContextMenuPolicy(Qt::ActionsContextMenu);
  tree.addAction(edit);
  QMenu *newMenu = new QMenu(this);

  QAction *glsl = new QAction("&GLSL", this),
    *scheme = new QAction("&Scheme", this),
    *tileset = new QAction("T&ileset", this),
    *sprite = new QAction("S&prite", this),
    *animation = new QAction("&Animation", this);
  
  glsl->setStatusTip("New GLSL-script asset");
  scheme->setStatusTip("New Scheme asset");
  tileset->setStatusTip("New tileset");
  sprite->setStatusTip("Load a new sprite");
  animation->setStatusTip("Load a new animation");

  connect(glsl, &QAction::triggered, []() {
      add_glsl_script();
    });
  connect(scheme, &QAction::triggered, []() {
      add_scheme_script();
    });;

  connect(tileset, &QAction::triggered, this, &MainWindow::newTileset) ;
  connect(sprite, &QAction::triggered, [=]() {
        QString texture_path = QFileDialog::getOpenFileName(this, "Load texture file", ".", "Image Files (*.png)");
	editorController::instance->loadSprite(texture_path.toStdString().c_str());
    });

  connect(animation, &QAction::triggered, [=]() {
      animationLoader *l = new animationLoader;
      l->show();
    });

  newMenu->addAction(glsl);
  newMenu->addAction(scheme);
  newMenu->addSeparator();
  newMenu->addAction(tileset);
  newMenu->addAction(sprite);
  newMenu->addAction(animation);

  QAction *newMenu_act = new QAction("&New", this);
  newMenu_act->setMenu(newMenu);
  tree.addAction(newMenu_act);
}

void MainWindow::newTileset() {
  QString texture_file = QFileDialog::getOpenFileName(this, "Load texture file", ".", "Image Files (*.png)");

  tilesetContainer *t = new tilesetContainer(tileset_view, texture_file.toStdString().c_str());

  t->setName("New Tileset");
  
  // if(t->load_new_texture(texture_file.toStdString().c_str(), tileset_view)) {
  ec->documentTreeModel->begin();
  (*ec->document.getTilesets())[t->getId()] = t;
  ec->documentTreeModel->end();
  // }
  // else {
  //   delete t;
  //   QMessageBox::critical(nullptr, "QMapper",
  // 			  QString("Unable to load tileset texture %1").arg(texture_file));
  // }
}

void MainWindow::prepareStartEngine(QVBoxLayout *toolbox_layout)
{
  QPushButton *start = new QPushButton("Start engine");
  setSize(start);

  toolbox_layout->addSpacing(10);
  toolbox_layout->addWidget(start);

  connect(start, &QPushButton::clicked,
	  [&]() {
	    qDebug() << "Starting engine...";

	    auto e = ec->e;
	    
	    e->er->getDrawQueue().clear();
	    int w = e->er->width(),
	      h = e->er->height();
	    auto f = e->er->getGlFns();
	    
	    for(Renderable *dq: map_view->getDrawQueue()) {
	      Renderable *to_render = dq->copyRenderable();
	      // to_render->redoMatrices(e->er, f, w, h);
	      e->er->getDrawQueue().push_back(to_render);
	    }

	    e->er->freeCtx();
	    
	    e->setWindowState(e->windowState()|Qt::WindowMaximized);
	    e->show();
	  });
}

void MainWindow::prepareResizeBtn(QVBoxLayout *toolbox_layout)
{
  QPushButton *btn = new QPushButton("Resize map");
  setSize(btn);

  toolbox_layout->addWidget(btn);

  connect(btn, &QPushButton::clicked,
	  [&]() {
	    Map *currentMap = toMap(ec->document.fetchRegister("Map", ec->indexOfChosenMap));
	    resize_data *o = new resize_data;
	    o->setNew_width(currentMap->width());
	    o->setNew_height(currentMap->height());
	    Propertyeditor *p = new Propertyeditor(o, this);
	    p->onClose = [=]() {
	      qDebug() << "Resizing to " << o->getNew_width() << ", " << o->getNew_height() << ", " << (o->getVertical_anchor() == TOP? "TOP": "BOTTOM") << ", " << (o->getHorizontal_anchor() == LEFT? "LEFT": "RIGHT");
	      currentMap->resize(o->getNew_width(), o->getNew_height(), o->getVertical_anchor(), o->getHorizontal_anchor());
	      delete o;
	      p->accept();
	    };
	    p->show();
	  });
}

MainWindow::MainWindow(int argc, char** argv) :  QMainWindow(), t(argc, argv), ec(new editorController())
{
  ui.setupUi(this);
  ec->registerWindow(this);

  map_view = new Renderer(this);
  map_view->name = "MAP VIEW";
  ec->map_view = map_view;
  ec->populateMaps();
  
  tileset_view = new Renderer(this);
  tileset_view->name = "TILESET VIEW";
  ec->tilesetView = tileset_view;
  
  auto _tileview = new tileview_renderer(this);
  _tileview->name = "TILE VIEW";
  
  _tileview->setMinimumSize(50,50);
  _tileview->setMaximumSize(50,50);

  tileview = _tileview;

  tileset_view->mouseMoveEvents.push_back([=](QMouseEvent *e) {
      if(e->buttons() != Qt::LeftButton) return;

      int x = e->x() / 50, y = e->y() / 50;

      ec->setSelectedTile(x, y, tileset_view, _tileview);

    });

  map_view->mouseDownEvents.push_back([=](QMouseEvent *e) {
      ec->t->mouseDown(e, ec);
    });

  map_view->mouseMoveEvents.push_back([=](QMouseEvent *e) {
      if(e->buttons() != Qt::LeftButton) return;

      int x = e->x() / 50, y = e->y() / 50;

      ec->t->doUse(e, x, y, ec);
      ec->t->registerDrag(x, y);
    });
  map_view->mouseUpEvents.push_back([=](QMouseEvent *E) {
      ec->t->mouseUp(E);
    });

  QWidget *tb = new QWidget(this);
  tb->setMaximumSize(200, INT_MAX);
  QVBoxLayout *toolbox_layout = new QVBoxLayout(this);
  toolbox_layout->setSpacing(0);

  toolbox_layout->addWidget(new QLabel(QString("Selected Tile"), this));
  toolbox_layout->addWidget(tileview);

  toolbox_layout->addWidget(toolbox());
  
  prepare_server_button(toolbox_layout);
  prepareResizeBtn(toolbox_layout);
  prepareStartEngine(toolbox_layout);

  setupTree();
  setupTreeCtxMenu();
  setupMainMenu();
  toolbox_layout->addWidget(&tree);

  QPushButton *close = new QPushButton("Close", this);
  connect(close, &QPushButton::clicked, this, &QMainWindow::close);
  toolbox_layout->addWidget(close);

  tb->setLayout(toolbox_layout);;

  Scroll *s = new Scroll(this);
  s->setWidget(map_view);

  Scroll *s2 = new Scroll(this);
  s2->setWidget(tileset_view);

  QSplitter *splitter = new QSplitter(Qt::Vertical, this);
  splitter->addWidget(s);
  splitter->addWidget(s2);

  ui.splitter->addWidget(tb);
  ui.splitter->addWidget(splitter);
  
  t.start();
}

MainWindow::~MainWindow()
{
  t.running = false;
  if(!t.wait()) {
    qDebug()<<"Terminating guile_thread";
    t.terminate();
  }
} 

void MainWindow::registerController(editorController *ec)
{
  this->ec = ec;

  puts("Controller registered");
  ec->registerWindow(this);
}

void MainWindow::setupMainMenu()
{
  auto filemenu = menuBar()->addMenu("File");
  QAction *save = new QAction("Save project", this),
    *load = new QAction("Load project", this);
  connect(save, &QAction::triggered, [=]() {
      QString save_file = QFileDialog::getSaveFileName(this, "Save project file", ".", "QMapper projects (*.qmapper)");

      if(save_file == "") {
	qDebug() << "Got empty save_file_path";
	return;
      }
      
      if(!save_file.endsWith(".qmapper"))
	save_file = save_file + ".qmapper";
      qDebug() << "Trying to save to " << save_file;
      ec->saveTo(save_file);
    });

  connect(load, &QAction::triggered, [=]() {
      QString load_file = QFileDialog::getOpenFileName(this, "Load project file", ".", "QMapper projects (*.qmapper)");

      if(load_file == "") {
	qDebug() << "Got empty load_file_path";
	return;
      }
      
      if(!load_file.endsWith(".qmapper"))
	load_file = load_file + ".qmapper";
      qDebug() << "Trying to load from " << load_file;
      ec->loadFrom(load_file);
      tree.setModel(ec->documentTreeModel);
    });

  filemenu->addAction(save);
  filemenu->addAction(load);
}
