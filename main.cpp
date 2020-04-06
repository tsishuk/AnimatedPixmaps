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



class Button : public QGraphicsWidget
{
    Q_OBJECT
public:
    Button(const QPixmap &pixmap, QGraphicsItem *parent = nullptr):
        QGraphicsWidget(parent), pix(pixmap)
    {
        this->setGeometry(0,0,100,100);
    }

    QRectF boundingRect() const override
    {
        return QRectF(0, 0, 100, 100);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override
    {
        bool down = option->state & QStyle::State_Sunken;

        if (down){
            painter->setBrush(Qt::red);
            painter->drawPixmap(3, 3, pix);
            painter->drawEllipse(1,1,25,25);

        }
        else
            painter->drawPixmap(0, 0, pix);
    }

signals:
    void pressed();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        emit pressed();
        update();
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
        update();
    }

private:
    QPixmap pix;
};



int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QPixmap calc(":/calculator.png");
    QPixmap dict(":/dictionary.png");
    QPixmap elipse(":/ellipse.png");
    QPixmap stateImage1(":/figure8.png");

    QGraphicsScene scene(QRectF(-200, -200, 500, 500));
    QGraphicsView view(&scene);

    Button* btn1 = new Button(stateImage1);
    btn1->setPos(-50, -150);
    btn1->setZValue(1);
    Button* btn2 = new Button(stateImage1);
    btn2->setPos(-200, -150);
    btn2->setZValue(2);


    Pixmap* pPixmapItem = new Pixmap(calc);
    pPixmapItem->setPos(-20,20);

    Pixmap* pPixmapItem2 = new Pixmap(dict);
    pPixmapItem2->setOffset(60,60);

    Pixmap* pPixmapItem3 = new Pixmap(elipse);
    pPixmapItem3->setOffset(0,0);

    QPushButton* btn = new QPushButton("Press");
    btn->setProperty("pos",QPointF(20,-150));

    QPushButton* btnSecond = new QPushButton("Second");
    btnSecond->setProperty("pos",QPointF(30,150));

    scene.addItem(btn1);
    scene.addItem(btn2);
    scene.addItem(pPixmapItem);
    scene.addItem(pPixmapItem2);
    scene.addItem(pPixmapItem3);
    scene.addWidget(btnSecond);




    QStateMachine*  psm = new QStateMachine;

    QState* rootState = new QState;
    QState* pFirstState = new QState(rootState);
    QPoint firstStatePoint(100,200);
    pFirstState->assignProperty(btnSecond, "pos", firstStatePoint);
    pFirstState->assignProperty(pPixmapItem, "pos", QPointF(-60,60));
    pFirstState->assignProperty(pPixmapItem2, "pos", QPointF(120,30));
    pFirstState->assignProperty(pPixmapItem3, "pos", QPointF(140,-40));


    QState* pSecondState = new QState(rootState);
    pSecondState->assignProperty(btnSecond, "pos", QPointF(200,100));
    pSecondState->assignProperty(pPixmapItem, "pos", QPointF(60,-60));
    pSecondState->assignProperty(pPixmapItem2, "pos", QPointF(-120,30));
    pSecondState->assignProperty(pPixmapItem3, "pos", QPointF(40,140));

    psm->addState(rootState);
    psm->setInitialState(rootState);
    rootState->setInitialState(pFirstState);


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


    //QSignalTransition* pTrans1 = pFirstState->addTransition(btn, SIGNAL(clicked()), pSecondState);
    QAbstractTransition* trans = rootState->addTransition(btn1, SIGNAL(pressed()), pFirstState);
    trans->addAnimation(group);
    //pTrans1->addAnimation(group);

    trans = rootState->addTransition(btn2, SIGNAL(pressed()), pSecondState);
    trans->addAnimation(group);

    //QSignalTransition* pTrans2 = pSecondState->addTransition(btn, SIGNAL(clicked()), pFirstState);
    //pTrans2->addAnimation(group);

    psm->start();

    view.show();

    return app.exec();
}






#include "main.moc"
