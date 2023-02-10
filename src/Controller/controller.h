#ifndef QTQUICKTEST_CONTROLLER_H
#define QTQUICKTEST_CONTROLLER_H

class Controller {
public:
    static Controller instance;
    void SendNote(int note);
};

#endif