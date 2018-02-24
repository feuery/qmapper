#include <engine.h>

Engine::Engine(editorController *ec):ec(ec) {
    ui.setupUi(this);
    er = new Engine_Renderer(this);

    QPushButton *btn = new QPushButton(this);
    btn->setText("Close");

    connect(btn, &QPushButton::clicked, [&](){close();});

    QVBoxLayout *vb = new QVBoxLayout(this);
    vb->addWidget(er);
    vb->addWidget(btn);

    
    ui.centralwidget->setLayout(vb);
}
