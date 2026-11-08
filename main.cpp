#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Create the main Qt application instance
    QApplication app(argc, argv);

    // Set application metadata
    QApplication::setApplicationName("StudentRecordSystem");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("UniversityProject");

    // Instantiate and show the main window
    MainWindow window;
    window.show();

    // Start the Qt event loop
    return app.exec();
}
