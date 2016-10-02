#include <QApplication>
#include "<!**WINDOW_CLASS_NAME_LOWER**!>.h"
#include "colouicreator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    <!**WINDOW_CLASS_NAME**!> *mainWindow = new <!**WINDOW_CLASS_NAME**!>();
    ColoUiContainer *ui = new ColoUiContainer(mainWindow);
    mainWindow->setCentralWidget(ui);
    mainWindow->showMaximized();

    // Creating the Ui
    ColoUiCreator creator;
    creator.createFromResource(ui);

    return a.exec();
}
