#include <resize-form.h>

resize_form::resize_form(int oldw, int oldh): QDialog() {
  ui.setupUi(this);
  connect(ui.buttonBox, &QDialogButtonBox::accepted,
	  this, &resize_form::accept);
  connect(ui.buttonBox, &QDialogButtonBox::rejected,
	  this, &resize_form::rejected);
  ui.newWidthLineEdit->setText(QString::number(oldw));
  ui.newHeightLineEdit->setText(QString::number(oldh));
  printf("set resize_form up with params %d, %d\n", oldw, oldh);
}

void resize_form::accept() {
  QString str_new_w = ui.newWidthLineEdit->text(),
    str_new_h = ui.newHeightLineEdit->text();

  bool w_parsed = false, h_parsed = false;

  int new_w = str_new_w.toInt(&w_parsed),
    new_h = str_new_h.toInt(&h_parsed);

  if(!w_parsed) puts("Width was not a number");
  if(!h_parsed) puts("Height was not a number");

  if(w_parsed && h_parsed) {
    deliverResult({new_w, new_h});
  }
  
  close();
}

void resize_form::rejected() {
  close();
}
