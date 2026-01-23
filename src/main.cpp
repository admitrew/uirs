#include <QApplication>
#include "ui/EditorMainWindow.h"

int main(int argc,char** argv){
    QApplication a(argc,argv);
    EditorMainWindow w;
    w.show();
    return a.exec();
}
