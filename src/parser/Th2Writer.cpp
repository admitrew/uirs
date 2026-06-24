#include "Th2Writer.h"

#include "../core/Th2Serializable.h"

#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <algorithm>

void Th2Writer::write(QGraphicsScene* scene, const QString& filePath)
{
    write(scene, filePath, QStringList(), "scrap scrap1", "endscrap");
}

void Th2Writer::write(QGraphicsScene* scene,
                      const QString& filePath,
                      const QStringList& headerLines,
                      const QString& scrapLine,
                      const QString& endScrapLine)
{
    if (!scene) {
        return;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);

    for (const QString& line : headerLines) {
        out << line << "\n";
    }

    QString realScrapLine = scrapLine.trimmed().isEmpty()
        ? "scrap scrap1"
        : scrapLine.trimmed();

    QString realEndScrapLine = endScrapLine.trimmed().isEmpty()
        ? "endscrap"
        : endScrapLine.trimmed();

    out << realScrapLine << "\n";

    QList<QGraphicsItem*> items = scene->items();
    std::reverse(items.begin(), items.end());

    for (QGraphicsItem* item : items) {
        auto* th2Item = dynamic_cast<Th2Serializable*>(item);

        if (th2Item) {
            out << th2Item->toTh2() << "\n";
        }
    }

    out << realEndScrapLine << "\n";
}