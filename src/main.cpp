#include <QApplication>
#include <singleapplication.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //QApplication app(argc, argv);
    SingleApplication app(argc, argv);
    MainWindow window;
     // TODO: when minize to system bar, how to show it up again?
     // now it only work to bring up app window when it is below other  window
    QObject::connect(&app, &SingleApplication::instanceStarted,
                        &window, &MainWindow::raise);
    window.show();
   
                        
    return app.exec();
}
