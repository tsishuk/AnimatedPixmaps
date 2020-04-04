#include <QtWidgets>

#include <QtWidgets/QtWidgets>
#include <QtCore/qmath.h>
#include <QtCore/qrandom.h>
#include <QtCore/qstate.h>


class Pixmap : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Pixmap(const QPixmap &pix)
        : QObject(), QGraphicsPixmapItem(pix)
    {
        setCacheMode(DeviceCoordinateCache);
    }
};


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QPixmap calc(":/calculator.png");
    QPixmap dict(":/dictionary.png");
    QPixmap elipse(":/ellipse.png");

    QGraphicsScene scene(QRectF(-200, -200, 500, 500));
    QGraphicsView view(&scene);

    //QGraphicsPixmapItem* pPixmapItem = new QGraphicsPixmapItem(calc);
    Pixmap* pPixmapItem = new Pixmap(calc);
    pPixmapItem->setPos(-20,20);

    //QGraphicsPixmapItem* pPixmapItem2 = new QGraphicsPixmapItem(dict);
    Pixmap* pPixmapItem2 = new Pixmap(dict);
    pPixmapItem2->setOffset(60,60);

    //QGraphicsPixmapItem* pPixmapItem3 = new QGraphicsPixmapItem(elipse);
    Pixmap* pPixmapItem3 = new Pixmap(elipse);
    pPixmapItem3->setOffset(0,0);

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
    pFirstState->assignProperty(pPixmapItem, "pos", QPointF(-60,60));
    pFirstState->assignProperty(pPixmapItem2, "pos", QPointF(120,30));
    pFirstState->assignProperty(pPixmapItem3, "pos", QPointF(140,-40));


    QState* pSecondState = new QState(psm);
    pSecondState->assignProperty(btnSecond, "pos", QPointF(200,100));
    pSecondState->assignProperty(pPixmapItem, "pos", QPointF(60,-60));
    pSecondState->assignProperty(pPixmapItem2, "pos", QPointF(-120,30));
    pSecondState->assignProperty(pPixmapItem3, "pos", QPointF(40,140));

    psm->setInitialState(pFirstState);


    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    QPropertyAnimation* pAnim1 = new QPropertyAnimation(btnSecond, "pos");
    pAnim1->setEasingCurve(QEasingCurve::InBack);
    QPropertyAnimation* pAnim2 = new QPropertyAnimation(pPixmapItem, "pos");
    pAnim2->setEasingCurve(QEasingCurve::InOutCirc);
    QPropertyAnimation* pAnim3 = new QPropertyAnimation(pPixmapItem2, "pos");
    pAnim3->setEasingCurve(QEasingCurve::InExpo);
    QPropertyAnimation* pAnim4 = new QPropertyAnimation(pPixmapItem3, "pos");
    pAnim4->setEasingCurve(QEasingCurve::InOutCubic);

    group->addAnimation(pAnim1);
    group->addAnimation(pAnim2);
    group->addAnimation(pAnim3);
    group->addAnimation(pAnim4);


    QSignalTransition* pTrans1 = pFirstState->addTransition(btn, SIGNAL(clicked()), pSecondState);
    pTrans1->addAnimation(group);

    QSignalTransition* pTrans2 = pSecondState->addTransition(btn, SIGNAL(clicked()), pFirstState);
    pTrans2->addAnimation(group);

    psm->start();

    view.show();

    return app.exec();
}






#include "main.moc"
