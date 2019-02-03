#include <animationLoaderUi.h>
#include <QDebug>
#include <QFileDialog>
#include <editorController.h>

animationLoader::animationLoader(): QDialog() {
  ui.setupUi(this);
  ui.le_frameCount->setValidator(new QIntValidator(0, 99999, this));
  ui.le_lifetime->setValidator(new QIntValidator(0, 99999, this));
  connect(ui.browse_spritesheet, &QPushButton::clicked, [&]() {

      QString path = QFileDialog::getOpenFileName(this, "Load texture file", ".", "Image Files (*.png)");
      if(!path.isNull())
	ui.le_spritesheet_location->setText(path);
    });
  connect(ui.buttonBox, &QDialogButtonBox::accepted, [&]() {
      editorController::instance->loadAnimation(ui.le_spritesheet_location->text().toStdString().c_str(),
						ui.le_frameCount->text().toInt(),
						ui.le_lifetime->text().toInt());
      close();
    });

  connect(ui.buttonBox, &QDialogButtonBox::rejected, [&]() {
      close();
    });
}
