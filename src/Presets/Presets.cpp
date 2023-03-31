//
// Created by Breki on 10/03/2023.
//
#include "Presets.h"
#include <iostream>
#include <list>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <string>

#include <QQmlContext>

//cout << "List Elements: ";
//    for(int preset : presets) {
//        cout << preset <<", ";
//    }

//presets.push_back(4);


//Preset::Preset(int id, QObject* parent)
//    : id(id), QObject(parent), name() {}

Preset::Preset(int id, std::string name)
 : id(id), name() {}

void Preset::setId(int id) {
    this->id = id;
}

void Preset::setName(std::string name) {
    this->name = name;
}

int Preset::getId() {
    return this->id;
}

std::string Preset::getName() {
    return this->name;
}

Preset testpreset(5, "breki");



