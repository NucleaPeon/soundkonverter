

#include "soundkonverterapp.h"
#include "soundkonverter.h"
#include "global.h"
#include "version.h"
#include <QObject>
#include <QString>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <KAboutData>
#include <QLocale>
#include <KLocalizedString>



static const char description[] =
    "soundKonverter is a frontend to various audio converters, Replay Gain tools and CD rippers.\n\nPlease file bug reports at https://github.com/dfaust/soundkonverter/issues";

using namespace SoundKonverter;

int main(int argc, char **argv)
{
    KLocalizedString::setApplicationDomain("soundkonverter");
    
    QString version = QString("%1.%2.%3").arg(Version::RELEASE, Version::MAJOR, Version::MINOR);

    QCommandLineParser parser = QCommandLineParser();
    parser.addHelpOption();
    parser.addVersionOption();

    KAboutData about(
        SOUNDKONVERTER_NAME,
        i18n("soundKonverter"),
        version.toLatin1(),
        i18n(description),
        KAboutLicense::GPL,
        i18n("(C) 2005-2017 Daniel Faust"),
        i18n("Audio Application"));
    about.addAuthor( QObject::tr("Daniel Faust"), "", "hessijames@gmail.com" );
    about.addCredit( QObject::tr("David Vignoni"), QObject::tr("Nuvola icon theme"), 0, "http://www.icon-king.com" );
    about.addCredit( QObject::tr("Scott Wheeler"), QObject::tr("TagLib"), "wheeler@kde.org", "http://ktown.kde.org/~wheeler" );
    about.addCredit( QObject::tr("Marco Nelles"), QObject::tr("Audex"), 0, "http://opensource.maniatek.de/audex" );
    about.addCredit( QObject::tr("Amarok developers"), QObject::tr("Amarok"), 0, "http://amarok.kde.org" );
    about.addCredit( QObject::tr("All programmers of audio converters"), QObject::tr("Backends") );
    about.addCredit( QObject::tr("Patrick Auernig"), QObject::tr("Inital Port to KDE Frameworks 5"), "patrick.auernig@gmail.com" );
    about.addCredit( QObject::tr("Daniel Kettle"), QObject::tr("Inital Port to Qt6"), "initial.dann@gmail.com", "https://github.com/NucleaPeon");

    parser.addOption(QCommandLineOption("replaygain", QObject::tr("Open the Replay Gain tool and add all given files")));
    parser.addOption(QCommandLineOption("rip <device>", QObject::tr("List all tracks on the cd drive <device>, 'auto' will search for a cd")));
    parser.addOption(QCommandLineOption("profile <profile>", QObject::tr("Add all files using the given profile")));
    parser.addOption(QCommandLineOption("format <format>", QObject::tr("Add all files using the given format")));
    parser.addOption(QCommandLineOption("output <directory>", QObject::tr("Output all files to <directory>")));
    parser.addOption(QCommandLineOption("invisible", QObject::tr("Start soundKonverter invisible")));
    parser.addOption(QCommandLineOption("autostart", QObject::tr("Start the conversion immediately (enabled when using '--invisible')")));
    parser.addOption(QCommandLineOption("autoclose", QObject::tr("Close soundKonverter after all files are converted (enabled when using '--invisible')")));
    parser.addOption(QCommandLineOption("command <command>", QObject::tr("Execute <command> after each file has been converted (%i=input file, %o=output file)")));
    parser.addOption(QCommandLineOption("file-list <path>", QObject::tr("Load the file list at <path> after starting soundKonverter")));
    parser.addOption(QCommandLineOption("+[files]", QObject::tr("Audio file(s) to append to the file list")));


    // soundKonverterApp::addCmdLineOptions();
    // if( !soundKonverterApp::start() )
    // {
    //     return 0;
    // }

    soundKonverterApp app;

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}
