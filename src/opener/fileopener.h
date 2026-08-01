//
// C++ Interface: opener
//
// Description:
//
//
// Author: Daniel Faust <hessijames@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILEOPENER_H
#define FILEOPENER_H

#include "../options.h"
#include "../config.h"

#include <QFileDialog>

#include <QUrl>

class Config;
class Options;
class QLabel;
class ConversionOptions;
class QFileDialog;
class QPushButton;

/** @author Daniel Faust <hessijames@gmail.com> */
class FileOpener : public QFileDialog
{
    Q_OBJECT
public:
    FileOpener( Config *_config, QWidget *parent, Qt::WindowFlags f=Qt::Widget );
    ~FileOpener();

    /** true if the file dialog was aborted (don't execute the dialog) */
    bool dialogAborted;

private:
    Config *config;

    QFileDialog *fileDialog;
    Options *options;
    QList<QUrl> urls;
    QPushButton *pAdd;
    QPushButton *pCancel;
    QLabel *formatHelp;

private slots:
    void fileDialogAccepted();
    void okClickedSlot();
    void showHelp();

signals:
    void openFiles( const QList<QUrl>& files, ConversionOptions *conversionOptions );

};

#endif
