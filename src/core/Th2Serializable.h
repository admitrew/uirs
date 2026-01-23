#pragma once
#include <QString>

class Th2Serializable {
public:
    virtual ~Th2Serializable() = default;
    virtual QString toTh2() const = 0;
};
