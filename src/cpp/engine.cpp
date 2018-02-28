#include <engine.h>
#include <scroll.h>

Engine::Engine(editorController *ec):ec(ec) {
    ui.setupUi(this);
    er = new Engine_Renderer(this);
    er->setFocusPolicy(Qt::StrongFocus);

    QPushButton *btn = new QPushButton(this);
    btn->setText("Close");

    connect(btn, &QPushButton::clicked, [&](){close();});

    Scroll *s = new Scroll(this);
    s->setWidget(er);

    QVBoxLayout *vb = new QVBoxLayout(this);
    vb->addWidget(s);
    vb->addWidget(btn);

    
    ui.centralwidget->setLayout(vb);
}
