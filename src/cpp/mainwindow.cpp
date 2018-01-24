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

#define btnW 200
#define btnH 25

#define setSize(btn) btn->setMaximumWidth(btnW); \
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
	std::string type = b->type_identifier().get();

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
	  ec->indexOfChosenLayer = indexOf(m->layers, static_cast<Layer*>(l));

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
    *texture = new QAction("&Texture", this),
    *tileset = new QAction("T&ileset", this);
  
  glsl->setStatusTip("New GLSL-script asset");
  scheme->setStatusTip("New Scheme asset");
  texture->setStatusTip("New texture asset");
  tileset->setStatusTip("New tileset");

  connect(glsl, &QAction::triggered, []() {
      add_glsl_script();
    });
  connect(scheme, &QAction::triggered, []() {
      add_scheme_script();
    });
  connect(texture, &QAction::triggered, [=]() {
      QString textLoc = getTextureLocation();
      auto f = map_view->context()->versionFunctions<QOpenGLFunctions_4_3_Core>();
      if(!f) {
	puts("Couldn't fetch an active QOpenGLContext");
	return;
      }

      ec->documentTreeModel->begin();
      
      Texture *text = new textureContainer(textLoc, f);
      text->parent(&ec->document);
      ec->document.doRegister("Texture", text->getId(), text);

      ec->documentTreeModel->end();
    });

  connect(tileset, &QAction::triggered, this, &MainWindow::newTileset) ;

  newMenu->addAction(glsl);
  newMenu->addAction(scheme);
  newMenu->addSeparator();
  newMenu->addAction(texture);
  newMenu->addAction(tileset);

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
    ec->document.doRegister("Tileset", t->getId(), t);
    ec->documentTreeModel->end();
  // }
  // else {
  //   delete t;
  //   QMessageBox::critical(nullptr, "QMapper",
  // 			  QString("Unable to load tileset texture %1").arg(texture_file));
  // }
}

MainWindow::MainWindow(int argc, char** argv) :  QMainWindow(), t(argc, argv), ec(new editorController())
{
  ui.setupUi(this);
  ec->registerWindow(this);

  map_view = new Renderer;
  map_view->name = "MAP VIEW";
  ec->map_view = map_view;
  ec->populateMaps();
  
  tileset_view = new Renderer;
  tileset_view->name = "TILESET VIEW";
  
  auto _tileview = new tileview_renderer;
  _tileview->name = "TILE VIEW";

  ec->renderers.push_back(map_view);
  ec->renderers.push_back(tileset_view);
  ec->renderers.push_back(_tileview);
  
  _tileview->setMinimumSize(50,50);
  _tileview->setMaximumSize(50,50);

  tileview = _tileview;

  tileset_view->mouseMoveEvents.push_back([=](QMouseEvent *e) {
      if(e->buttons() != Qt::LeftButton) return;

      int x = e->x() / 50, y = e->y() / 50;

      ec->setSelectedTile(x, y, tileset_view, _tileview);

    });

  map_view->mouseDownEvents.push_back([=](QMouseEvent *e) {
      ec->t->mouseDown(ec);
    });

  map_view->mouseMoveEvents.push_back([=](QMouseEvent *e) {
      if(e->buttons() != Qt::LeftButton) return;

      int x = e->x() / 50, y = e->y() / 50;

      ec->t->doUse(e, x, y, ec);
      ec->t->registerDrag(x, y);
    });

  QWidget *tb = new QWidget(this);
  tb->setMaximumSize(200, INT_MAX);
  QVBoxLayout *toolbox_layout = new QVBoxLayout(this);
  toolbox_layout->setSpacing(0);

  toolbox_layout->addWidget(new QLabel(QString("Selected Tile"), this));
  toolbox_layout->addWidget(tileview);

  toolbox_layout->addWidget(toolbox());
  
  prepare_server_button(toolbox_layout);

  setupTree();
  setupTreeCtxMenu();
  toolbox_layout->addWidget(&tree);

  QPushButton *close = new QPushButton("Close", this);
  connect(close, &QPushButton::clicked, this, &QMainWindow::close);
  toolbox_layout->addWidget(close);

  tb->setLayout(toolbox_layout);;

  QSplitter *splitter = new QSplitter(Qt::Vertical, this);
  splitter->addWidget(map_view);
  splitter->addWidget(tileset_view);

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
