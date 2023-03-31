//
// Created by Breki on 10/03/2023.
//

#ifndef MAIN_QML_PRESETS_H
#define MAIN_QML_PRESETS_H

#include <vector>
#include <QPoint>
#include <chrono>
#include <QObject>
#include <string>

struct Preset:public QObject {
    Q_OBJECT
public:
    //Preset(int id, QObject* parent=0);
    Preset(int id, std::string name);
    int id;
    std::string name;
    Q_INVOKABLE int getId();
    Q_INVOKABLE std::string getName();
    Q_INVOKABLE void setId(int id);
    Q_INVOKABLE void setName(std::string name);
};


#endif //MAIN_QML_PRESETS_H
