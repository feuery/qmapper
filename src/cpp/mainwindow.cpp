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
	  obj* o = static_cast<obj*>(t);
	  ec->indexOfChosenTileset = t->getId();
	  tileset_view->getObjs().clear();
	  tileset_view->getObjs().push_back(o);
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
   
  btn2("I'm", l);
  btn2("A", l);
  btn2("Dynamically built", l);
  btn2("Toolbox list", l);
  
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
      (*ec->document.registry)[text->getId()] = text;

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
  
  if(t->load_new_texture(texture_file.toStdString().c_str(), tileset_view)) {
    ec->documentTreeModel->begin();
    (*ec->document.registry)[t->getId()] = t;
    ec->documentTreeModel->end();
  }
  else {
    delete t;
    QMessageBox::critical(nullptr, "QMapper",
			  QString("Unable to load tileset texture %1").arg(texture_file));
  }
}

MainWindow::MainWindow(int argc, char** argv) :  QMainWindow(), t(argc, argv), ec(new editorController())
{
  ui.setupUi(this);
  ec->registerWindow(this);

  map_view = new Renderer;
  tileset_view = new Renderer;
  auto _tileview = new tileview_renderer;
  _tileview->setMinimumSize(50,50);
  _tileview->setMaximumSize(50,50);

  tileview = _tileview;

  tileset_view->mouseMoveEvents.push_back([=](QMouseEvent *e) {
      if(e->buttons() != Qt::LeftButton) return;

      // jaa koordinaatit 50:llä ja kopioi indeksi jonnekin missä valittu-tile-shader näkee sen
      // Lisäks tee valitusta tilesetistä sellainen tekstuuri jonka voi liittää myös valittu-tile-shaderiin uniformina
      int x = e->x() / 50, y = e->y() / 50;

      ec->setSelectedTile(x, y, tileset_view, _tileview);

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
