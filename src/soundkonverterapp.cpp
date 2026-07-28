
#include "soundkonverterapp.h"
#include "global.h"
#include "soundkonverter.h"

#include <QApplication>
#include <QStandardPaths>
#include <QUrl>
#include <QFile>


soundKonverterApp::soundKonverterApp(int argc, char **argv)
    : QApplication(argc, argv)
{
    mainWindow = new soundKonverter();
    setActiveWindow( mainWindow );

    this->setApplicationName(SOUNDKONVERTER_NAME);

}

soundKonverterApp::~soundKonverterApp()
{}

int soundKonverterApp::newInstance()
{
    QStringList arguments = this->arguments();
    static bool first = true;
    bool visible = true;
    bool autoclose = false;
    bool autostart = false;
    bool activateMainWindow = true;
    
    if( ( first || !mainWindow->isVisible() ) && arguments.contains("replaygain") && args->count() > 0 )
        visible = false;

    autoclose = arguments.contains( "autoclose" );
    autostart = arguments.contains( "autostart" );

    const QString profile = args->getOption( "profile" );
    const QString format = args->getOption( "format" );
    const QString directory = args->getOption( "output" );
    const QString notifyCommand = args->getOption( "command" );
    const QString fileListPath = args->getOption( "file-list" );

    if( arguments.contains( "invisible" ) )
    {
        autoclose = true;
        autostart = true;
        visible = false;
        mainWindow->showSystemTray();
    }

    if( first && fileListPath.isEmpty() && QFile::exists(KStandardDirs::locateLocal("data","soundkonverter/filelist_autosave.xml")) )
    {
        if( !visible )
        {
            visible = true;
            autoclose = false;
            autostart = false;
            mainWindow->show();
        }
        mainWindow->show();
        kapp->processEvents();
        mainWindow->loadAutosaveFileList();
    }
    else if( !fileListPath.isEmpty() && QFile::exists(fileListPath) )
    {
        mainWindow->loadFileList(fileListPath);
    }

    const QString device = args->getOption( "rip" );
    if( !device.isEmpty() )
    {
        const bool success = mainWindow->ripCd( device, profile, format, directory, notifyCommand );
        if( !success && first )
        {
            kapp->quit();
            return 0;
        }
    }

    if( visible )
        mainWindow->show();

    mainWindow->setAutoClose( autoclose );

    if( arguments.contains( "replaygain" ) )
    {
        QList<QUrl> urls;
        for( int i=0; i<args->count(); i++ )
        {
            urls.append( args->arg(i) );
        }
        if( !urls.isEmpty() )
        {
            mainWindow->addReplayGainFiles( urls );
            activateMainWindow = false;
        }
    }
    else
    {
        QList<QUrl> urls;
        for( int i=0; i<args->count(); i++ )
        {
            urls.append( args->arg(i) );
        }
        if( !urls.isEmpty() )
            mainWindow->addConvertFiles( urls, profile, format, directory, notifyCommand );
    }
    args->clear();

    if( activateMainWindow )
        mainWindow->activateWindow();

    if( autostart )
        mainWindow->startConversion();

    if( first )
        mainWindow->startupChecks();

    first = false;

    return 0;
}

