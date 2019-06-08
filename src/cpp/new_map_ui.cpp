#include <new_map_ui.h>
#include <QIntValidator>

newMapUI::newMapUI(): QDialog() {
  ui.setupUi(this);

  ui.le_maph->setValidator(new QIntValidator(0, 99999, this));
  ui.le_mapw->setValidator(new QIntValidator(0, 99999, this));
  ui.le_layercount->setValidator(new QIntValidator(0, 99999, this));

  connect(ui.buttonBox, &QDialogButtonBox::accepted, [&]() {
						       accept_lambda(ui.le_mapw->text().toInt(),
								     ui.le_maph->text().toInt(),
								     ui.le_layercount->text().toInt());
						       close();
						     });

  connect(ui.buttonBox, &QDialogButtonBox::rejected, [&]() {
						       reject_lambda();
						       close();
						     });
}
