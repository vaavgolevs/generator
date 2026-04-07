#include "generator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    generator window;
    window.show();
    return app.exec();
}
