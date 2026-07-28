
#include "codecproblems.h"

#include <QLocale>
#include <QIcon>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>


CodecProblems::CodecProblems( Mode mode, const QList<Problem>& problemList, QWidget* parent, Qt::WindowFlags f )
    : QDialog( parent, f )
{
    setCaption( tr("Solutions for backend problems") );
    setWindowIcon( KIcon("help-about") );
    setButtons( QDialog::Close );
    setButtonFocus( QDialog::Close );

    QWidget *widget = new QWidget( this );
    setMainWidget( widget );
    QVBoxLayout *box = new QVBoxLayout( widget );

    QString message;
    if( mode == Debug )
    {
        if( problemList.isEmpty() )
        {
            message = tr("soundKonverter couldn't find any missing packages.\nIf you are missing some file formats you might need to install an additional plugin via the package manager of your distribution.");
        }
        else
        {
            message = tr("Some of the installed plugins aren't working.\nPossible solutions are listed below.");
        }
    }
    else if( mode == Decode )
    {
        message = tr("Some files can't be decoded.\nPossible solutions are listed below.");
    }
    else if( mode == ReplayGain )
    {
        message = tr("Replay Gain isn't supported for some files.\nPossible solutions are listed below.");
    }
    else if( mode == AudioCd )
    {
        if( problemList.isEmpty() )
        {
            message = tr("Ripping audio CDs is not supported by any installed plugin.\nPlease have a look at your distributions package manager in order to get a cd ripper plugin for soundKonverter.");
        }
        else
        {
            message = tr("Ripping audio CDs is currently not supported because of missing backends.\nPossible solutions are listed below.");
        }
    }
    QLabel *messageLabel = new QLabel( message, this );
    box->addWidget( messageLabel );

    if( !problemList.isEmpty() )
    {
        QStringList messageList;
        for( int i=0; i<problemList.count(); i++ )
        {
            const QString codecName = problemList.at(i).codecName;
            if( codecName != "wav" )
            {
                if( problemList.at(i).affectedFiles.isEmpty() )
                {
                    messageList += "<b>" + tr("Possible solutions for %1", codecName) + "</b>:\n" + problemList.at(i).solutions.join("\n<b>"+trc("like in either or","or")+"</b>\n");
                }
                else
                {
                    messageList += "<b>" + tr("Possible solutions for %1", codecName) + "</b>:\n" + problemList.at(i).solutions.join("\n<b>"+trc("like in either or","or")+"</b>\n") + "\n\n" + tr("Affected files:") + "\n" + problemList.at(i).affectedFiles.join("\n");
                }
            }
        }
        QLabel *solutionsLabel = new QLabel( messageList.join("\n\n").replace("\n","<br>"), this );
        solutionsLabel->setMargin( 8 );
        solutionsLabel->setWordWrap( true );
        solutionsLabel->setTextInteractionFlags( Qt::TextSelectableByMouse );

        QScrollArea *solutionsScrollArea = new QScrollArea();
        solutionsScrollArea->setWidget( solutionsLabel );
        box->addWidget( solutionsScrollArea );
    }
}

CodecProblems::~CodecProblems()
{}

