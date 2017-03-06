#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QtQuick>
#include <algorithms/qgraph.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<QGraph,1>("QGraph", 1, 1, "Graph");
   // QQuickView view;
    QQmlApplicationEngine engine;
    //view.engine()->rootContext()->setContextProperty("graph",&graph);
    //view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //view.show();
    return app.exec();
}
