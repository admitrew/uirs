#include "Th2Writer.h"
#include "core/Th2Serializable.h"
#include <QFile>
#include <QTextStream>

void Th2Writer::write(QGraphicsScene* s, const QString& f) {
    QFile file(f);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    for(auto* i:s->items())
        if(auto* t=dynamic_cast<Th2Serializable*>(i))
            out<<t->toTh2()<<'\n';
}
