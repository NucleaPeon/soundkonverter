#ifndef SOUNDKONVERTERAPP_H
#define SOUNDKONVERTERAPP_H

#include <QtCore/QtGlobal>
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <kuniqueapplication.h>
#else
#include <QApplication>
#endif

class soundKonverter;

/**
 * @short The soundKonverter application. It ensures that there can only be one instance of soundKonverter.
 * @author Daniel Faust <hessijames@gmail.com>
 * @version 0.3
 */
class soundKonverterApp : public QApplication
{
    Q_OBJECT
public:
    /** Constructor */
    soundKonverterApp(int argc, char **argv);

    /** Destructor */
    ~soundKonverterApp();

    /** This function is called, when a new instance of soundKonverter should be created */
    virtual int newInstance();

// private:
    soundKonverter *mainWindow;
};

#endif // SOUNDKONVERTERAPP_H
