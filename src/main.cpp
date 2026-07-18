

#include "soundkonverterapp.h"
#include "soundkonverter.h"
#include "global.h"
#include "version.h"

#include <KMainWindow>
#include <QApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>
#include <KLocalizedString>

static const char description[] =
I18N_NOOP("soundKonverter is a frontend to various audio converters, Replay Gain tools and CD rippers.\n\nPlease file bug reports at https://github.com/dfaust/soundkonverter/issues");

using namespace SoundKonverter;

int main(int argc, char **argv)
{
    KLocalizedString::setApplicationDomain("soundkonverter");
    
    QString version = QString("%1.%2.%3").arg(Version::RELEASE, Version::MAJOR, Version::MINOR);

    KAboutData about("soundkonverter", ki18n("soundKonverter").toString(), version.toLatin1(), ki18n(description).toString(),
                     KAboutLicense::GPL, ki18n("(C) 2005-2017 Daniel Faust").toString(), KLocalizedString().toString(), "hessijames@gmail.com");
    about.addAuthor( ki18n("Daniel Faust").toString(), "", "hessijames@gmail.com" );
    about.addCredit( ki18n("David Vignoni").toString(), ki18n("Nuvola icon theme").toString(), 0, "http://www.icon-king.com" );
    about.addCredit( ki18n("Scott Wheeler").toString(), ki18n("TagLib").toString(), "wheeler@kde.org", "http://ktown.kde.org/~wheeler" );
    about.addCredit( ki18n("Marco Nelles").toString(), ki18n("Audex").toString(), 0, "http://opensource.maniatek.de/audex" );
    about.addCredit( ki18n("Amarok developers").toString(), ki18n("Amarok").toString(), 0, "http://amarok.kde.org" );
    about.addCredit( ki18n("All programmers of audio converters").toString(), ki18n("Backends").toString() );
    about.addCredit( ki18n("Patrick Auernig").toString(), ki18n("Inital Port to KDE Frameworks 5").toString(), "patrick.auernig@gmail.com" );
    about.addCredit( ki18n("Daniel Kettle").toString(), ki18n("Inital Port to Qt6").toString(), "initial.dann@gmail.com", "https://github.com/NucleaPeon");
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add( "replaygain", ki18n("Open the Replay Gain tool and add all given files") );
    options.add( "rip <device>", ki18n("List all tracks on the cd drive <device>, 'auto' will search for a cd") );
    options.add( "profile <profile>", ki18n("Add all files using the given profile") );
    options.add( "format <format>", ki18n("Add all files using the given format") );
    options.add( "output <directory>", ki18n("Output all files to <directory>") );
    options.add( "invisible", ki18n("Start soundKonverter invisible") );
    options.add( "autostart", ki18n("Start the conversion immediately (enabled when using '--invisible')") );
    options.add( "autoclose", ki18n("Close soundKonverter after all files are converted (enabled when using '--invisible')") );
    options.add( "command <command>", ki18n("Execute <command> after each file has been converted (%i=input file, %o=output file)") );
    options.add( "file-list <path>", ki18n("Load the file list at <path> after starting soundKonverter") );
    options.add( "+[files]", ki18n("Audio file(s) to append to the file list") );
    KCmdLineArgs::addCmdLineOptions(options);

    soundKonverterApp::addCmdLineOptions();
    if( !soundKonverterApp::start() )
    {
        return 0;
    }

    soundKonverterApp app;

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}
