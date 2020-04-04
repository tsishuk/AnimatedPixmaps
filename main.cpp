#include <QtWidgets>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QPixmap calc(":/calculator.png");
    QPixmap dict(":/dictionary.png");
    QPixmap elipse(":/ellipse.png");

    QGraphicsScene scene(QRectF(-200, -200, 500, 500));
    QGraphicsView view(&scene);

    QGraphicsPixmapItem* pPixmapItem = new QGraphicsPixmapItem(calc);

    QGraphicsPixmapItem* pPixmapItem2 = new QGraphicsPixmapItem(dict);
    pPixmapItem2->setOffset(60,60);

    QGraphicsPixmapItem* pPixmapItem3 = new QGraphicsPixmapItem(elipse);
    pPixmapItem3->setOffset(120,120);

    QPushButton* btn = new QPushButton("Press");
    btn->setProperty("pos",QPointF(20,-150));

    QPushButton* btnSecond = new QPushButton("Second");
    btnSecond->setProperty("pos",QPointF(30,150));


    scene.addItem(pPixmapItem);
    scene.addItem(pPixmapItem2);
    scene.addItem(pPixmapItem3);
    scene.addWidget(btn);
    scene.addWidget(btnSecond);


    QStateMachine*  psm = new QStateMachine;

    QState* pFirstState = new QState(psm);
    QPoint firstStatePoint(100,200);
    pFirstState->assignProperty(btnSecond, "pos", firstStatePoint);
    psm->setInitialState(pFirstState);

    QState* pSecondState = new QState(psm);
    pSecondState->assignProperty(btnSecond, "pos", QPointF(200,100));

    pFirstState->addTransition(btn, SIGNAL(clicked()), pSecondState);
    pSecondState->addTransition(btn, SIGNAL(clicked()), pFirstState);

    psm->start();

    view.show();

    return app.exec();
}

