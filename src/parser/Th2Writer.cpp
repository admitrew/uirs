#include "Th2Writer.h"

#include "../core/Th2Serializable.h"

#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <algorithm>

void Th2Writer::write(QGraphicsScene* scene, const QString& filePath)
{
    if (!scene) {
        return;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);

    out << "scrap scrap1\n";

    QList<QGraphicsItem*> items = scene->items();

    std::reverse(items.begin(), items.end());

    for (QGraphicsItem* item : items) {
        auto* th2Item = dynamic_cast<Th2Serializable*>(item);

        if (th2Item) {
            out << th2Item->toTh2() << "\n";
        }
    }

    out << "endscrap\n";
}