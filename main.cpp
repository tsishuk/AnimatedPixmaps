#include <QtWidgets>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QPixmap calc(":/calculator.png");
    QPixmap dict(":/dictionary.png");
    QPixmap elipse(":/ellipse.png");
    QPushButton* btn = new QPushButton("Press");

    QGraphicsScene scene(QRectF(-200, -200, 500, 500));
    QGraphicsView view(&scene);

    QGraphicsPixmapItem* pPixmapItem = new QGraphicsPixmapItem(calc);
    QGraphicsPixmapItem* pPixmapItem2 = new QGraphicsPixmapItem(dict);
    pPixmapItem2->setOffset(60,60);
    QGraphicsPixmapItem* pPixmapItem3 = new QGraphicsPixmapItem(elipse);
    pPixmapItem3->setOffset(120,120);
    btn->setGeometry(50,-150,80,80);


    scene.addItem(pPixmapItem);
    scene.addItem(pPixmapItem2);
    scene.addItem(pPixmapItem3);
    scene.addWidget(btn);

    view.show();

    return app.exec();
}

