//
// C++ Implementation: configgeneralpage
//
// Description:
//
//
// Author: Daniel Faust <hessijames@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configgeneralpage.h"

#include "../config.h"

#include <QLocale>
#include <KComboBox>
#include <KLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <KIntSpinBox>

#include <QLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QDir>

#include <solid/device.h>


ConfigGeneralPage::ConfigGeneralPage( Config *_config, QWidget *parent )
    : ConfigPageBase( parent ),
    config( _config )
{
    const int processorsCount = Solid::Device::listFromType(Solid::DeviceInterface::Processor, QString()).count();

    QVBoxLayout *box = new QVBoxLayout( this );

    QFont groupFont;
    groupFont.setBold( true );

    QLabel *lUserInterface = new QLabel( tr("User interface"), this );
    lUserInterface->setFont( groupFont );
    box->addWidget( lUserInterface );

    box->addSpacing( spacingSmall );

    QHBoxLayout *startTabBox = new QHBoxLayout();
    startTabBox->addSpacing( spacingOffset );
    box->addLayout( startTabBox );
    QLabel *lStartTab = new QLabel( tr("Start in Mode:"), this );
    startTabBox->addWidget( lStartTab );
    cStartTab = new KComboBox( this );
    cStartTab->addItem( tr("Last used") );
    cStartTab->addItem( tr("Simple") );
    cStartTab->addItem( tr("Detailed") );
    cStartTab->setCurrentIndex( config->data.general.startTab );
    startTabBox->addWidget( cStartTab );
    connect( cStartTab, SIGNAL(activated(int)), this, SLOT(somethingChanged()) );

    box->addSpacing( spacingSmall );

    QHBoxLayout *defaultProfileBox = new QHBoxLayout();
    defaultProfileBox->addSpacing( spacingOffset );
    box->addLayout( defaultProfileBox );
    QLabel *lDefaultProfile = new QLabel( tr("Default profile:"), this );
    defaultProfileBox->addWidget( lDefaultProfile );
    cDefaultProfile = new KComboBox( this );
    QStringList sDefaultProfile;
    sDefaultProfile += tr("Last used");
    sDefaultProfile += tr("Very low");
    sDefaultProfile += tr("Low");
    sDefaultProfile += tr("Medium");
    sDefaultProfile += tr("High");
    sDefaultProfile += tr("Very high");
    sDefaultProfile += tr("Lossless");
//     sDefaultProfile += tr("Hybrid"); // currently unused
    sDefaultProfile += config->customProfiles();
    cDefaultProfile->addItems( sDefaultProfile );
    cDefaultProfile->setCurrentIndex( cDefaultProfile->findText(config->data.general.defaultProfile) );
    defaultProfileBox->addWidget( cDefaultProfile );
    connect( cDefaultProfile, SIGNAL(activated(int)), this, SLOT(profileChanged()) );
    connect( cDefaultProfile, SIGNAL(activated(int)), this, SLOT(somethingChanged()) );
    QLabel *lDefaultFormat = new QLabel( tr("Default format:"), this );
    defaultProfileBox->addWidget( lDefaultFormat );
    cDefaultFormat = new KComboBox( this );
    cDefaultFormat->setCurrentIndex( cDefaultFormat->findText(config->data.general.defaultFormat) );
    defaultProfileBox->addWidget( cDefaultFormat );
    connect( cDefaultFormat, SIGNAL(activated(int)), this, SLOT(somethingChanged()) );
    profileChanged();

    box->addSpacing( spacingBig );

    QLabel *lConversion = new QLabel( tr("Conversion"), this );
    lConversion->setFont( groupFont );
    box->addWidget( lConversion );

    box->addSpacing( spacingSmall );

//     QHBoxLayout *priorityBox = new QHBoxLayout();
//     box->addLayout( priorityBox );
//     QLabel *lPriority = new QLabel( tr("Process priority of the backends:"), this );
//     priorityBox->addWidget( lPriority );
//     cPriority = new KComboBox( this );
//     sPriority += tr("Normal");
//     sPriority += tr("Low");
//     cPriority->addItems( sPriority );
//     cPriority->setCurrentIndex( config->data.general.priority / 10 ); // NOTE that just works for 'normal' and 'low'
//     priorityBox->addWidget( cPriority );
//     connect( cPriority, SIGNAL(activated(int)), this, SIGNAL(configChanged()) );
//
//     box->addSpacing( 5 );

    QHBoxLayout *conflictHandlingBox = new QHBoxLayout();
    conflictHandlingBox->addSpacing( spacingOffset );
    box->addLayout( conflictHandlingBox );
    QLabel *lConflictHandling = new QLabel( tr("Conflict handling:"), this );
    conflictHandlingBox->addWidget( lConflictHandling );
    cConflictHandling = new KComboBox( this );
    cConflictHandling->addItem( tr("Generate new file name") );
    cConflictHandling->addItem( tr("Skip file") );
//     cConflictHandling->addItem( tr("Overwrite file") );
    cConflictHandling->setToolTip( tr("Do that if the output file already exists") );
    cConflictHandling->setCurrentIndex( (int)config->data.general.conflictHandling );
    conflictHandlingBox->addWidget( cConflictHandling );
    connect( cConflictHandling, SIGNAL(activated(int)), this, SLOT(somethingChanged()) );

    box->addSpacing( spacingSmall );

    QHBoxLayout *numFilesBox = new QHBoxLayout();
    numFilesBox->addSpacing( spacingOffset );
    box->addLayout( numFilesBox );
    QLabel *lNumFiles = new QLabel( tr("Number of files to convert at once:"), this );
    numFilesBox->addWidget( lNumFiles );
    iNumFiles = new KIntSpinBox( this );
    iNumFiles->setToolTip( tr("You shouldn't set this number higher than the amount of installed processor cores.\nThere have been %1 processor cores detected.", processorsCount) );
    iNumFiles->setRange( 1, 100 );
    iNumFiles->setValue( config->data.general.numFiles );
    numFilesBox->addWidget( iNumFiles );
    connect( iNumFiles, SIGNAL(valueChanged(int)), this, SLOT(somethingChanged()) );
    numFilesBox->setStretch( 0, 3 );
    numFilesBox->setStretch( 1, 1 );

    box->addSpacing( spacingSmall );

    QHBoxLayout *waitForAlbumGainBox = new QHBoxLayout();
    waitForAlbumGainBox->addSpacing( spacingOffset );
    box->addLayout( waitForAlbumGainBox );
    cWaitForAlbumGain = new QCheckBox( tr("Apply album gain to converted files"), this );
    cWaitForAlbumGain->setToolTip( tr("Keep songs of the same album waiting in file list in order to apply album gain to all files.") );
    cWaitForAlbumGain->setChecked( config->data.general.waitForAlbumGain );
    waitForAlbumGainBox->addWidget( cWaitForAlbumGain );
    connect( cWaitForAlbumGain, SIGNAL(toggled(bool)), this, SLOT(somethingChanged()) );

    box->addSpacing( spacingSmall );

    QHBoxLayout *copyIfSameCodecBox = new QHBoxLayout();
    copyIfSameCodecBox->addSpacing( spacingOffset );
    box->addLayout( copyIfSameCodecBox );
    cCopyIfSameCodec = new QCheckBox( tr("Copy files instead of converting to the same file format"), this );
    cCopyIfSameCodec->setToolTip( tr("If the input and the output file format are the same, copy the files instead of re-encoding them.") );
    cCopyIfSameCodec->setChecked( config->data.general.copyIfSameCodec );
    copyIfSameCodecBox->addWidget( cCopyIfSameCodec );
    connect( cCopyIfSameCodec, SIGNAL(toggled(bool)), this, SLOT(somethingChanged()) );

    box->addSpacing( spacingBig );

    QLabel *lReplayGainTool = new QLabel( tr("Replay Gain tool"), this );
    lReplayGainTool->setFont( groupFont );
    box->addWidget( lReplayGainTool );

    box->addSpacing( spacingSmall );

    QHBoxLayout *replayGainGroupingBox = new QHBoxLayout();
    replayGainGroupingBox->addSpacing( spacingOffset );
    box->addLayout( replayGainGroupingBox );
    QLabel* lReplayGainGrouping = new QLabel( tr("Group files in the Replay Gain tool by:"), this );
    replayGainGroupingBox->addWidget( lReplayGainGrouping );
    cReplayGainGrouping = new KComboBox( this );
    cReplayGainGrouping->addItem( trc("Group files in the Replay Gain tool by","Album tags and directories") );
    cReplayGainGrouping->addItem( trc("Group files in the Replay Gain tool by","Album tags only") );
    cReplayGainGrouping->addItem( trc("Group files in the Replay Gain tool by","Directories only") );
    cReplayGainGrouping->setCurrentIndex( (int)config->data.general.replayGainGrouping );
    replayGainGroupingBox->addWidget( cReplayGainGrouping );
    connect( cReplayGainGrouping, SIGNAL(activated(int)), this, SLOT(somethingChanged()) );

    box->addSpacing( spacingSmall );

    QHBoxLayout *numReplayGainFilesBox = new QHBoxLayout();
    numReplayGainFilesBox->addSpacing( spacingOffset );
    box->addLayout( numReplayGainFilesBox );
    QLabel *lNumReplayGainFiles = new QLabel( tr("Number of items to process at once:"), this );
    numReplayGainFilesBox->addWidget( lNumReplayGainFiles );
    iNumReplayGainFiles = new KIntSpinBox( this );
    iNumReplayGainFiles->setToolTip( tr("You shouldn't set this number higher than the amount of installed processor cores.\nThere have been %1 processor cores detected.", processorsCount) );
    iNumReplayGainFiles->setRange( 1, 100 );
    iNumReplayGainFiles->setValue( config->data.general.numReplayGainFiles );
    numReplayGainFilesBox->addWidget( iNumReplayGainFiles );
    connect( iNumReplayGainFiles, SIGNAL(valueChanged(int)), this, SLOT(somethingChanged()) );
    numReplayGainFilesBox->setStretch( 0, 3 );
    numReplayGainFilesBox->setStretch( 1, 1 );

    box->addStretch();
}

ConfigGeneralPage::~ConfigGeneralPage()
{}

void ConfigGeneralPage::resetDefaults()
{
    const int processorsCount = Solid::Device::listFromType(Solid::DeviceInterface::Processor, QString()).count();

    cStartTab->setCurrentIndex( 0 );
    cDefaultProfile->setCurrentIndex( 0 );
    cDefaultFormat->setCurrentIndex( 0 );
//     cPriority->setCurrentIndex( 1 );
    cConflictHandling->setCurrentIndex( 0 );
    iNumFiles->setValue( processorsCount > 0 ? processorsCount : 1 );
    cWaitForAlbumGain->setChecked( true );
    cCopyIfSameCodec->setChecked( false );
    cReplayGainGrouping->setCurrentIndex( 0 );
    iNumReplayGainFiles->setValue( processorsCount > 0 ? processorsCount : 1 );

    emit configChanged( true );
}

void ConfigGeneralPage::saveSettings()
{
    config->data.general.startTab = cStartTab->currentIndex();
    config->data.general.defaultProfile = cDefaultProfile->currentText();
    config->data.general.defaultFormat = cDefaultFormat->currentText();
//     config->data.general.priority = cPriority->currentIndex() * 10; // NOTE that just works for 'normal' and 'low'
    config->data.general.conflictHandling = (Config::Data::General::ConflictHandling)cConflictHandling->currentIndex();
    config->data.general.numFiles = iNumFiles->value();
    config->data.general.waitForAlbumGain = cWaitForAlbumGain->isChecked();
    config->data.general.copyIfSameCodec = cCopyIfSameCodec->isChecked();
    config->data.general.replayGainGrouping = (Config::Data::General::ReplayGainGrouping)cReplayGainGrouping->currentIndex();
    config->data.general.numReplayGainFiles = iNumReplayGainFiles->value();
}

void ConfigGeneralPage::somethingChanged()
{
    const bool changed = cStartTab->currentIndex() != config->data.general.startTab ||
                         cDefaultProfile->currentText() != config->data.general.defaultProfile ||
                         cDefaultFormat->currentText() != config->data.general.defaultFormat ||
                         cConflictHandling->currentIndex() != (int)config->data.general.conflictHandling ||
                         iNumFiles->value() != config->data.general.numFiles ||
                         cWaitForAlbumGain->isChecked() != config->data.general.waitForAlbumGain ||
                         cCopyIfSameCodec->isChecked() != config->data.general.copyIfSameCodec ||
                         cReplayGainGrouping->currentIndex() != (int)config->data.general.replayGainGrouping ||
                         iNumReplayGainFiles->value() != config->data.general.numReplayGainFiles;

    emit configChanged( changed );
}

void ConfigGeneralPage::profileChanged()
{
    const QString profile = cDefaultProfile->currentText();
    QString lastFormat = cDefaultFormat->currentText();
    if( lastFormat.isEmpty() )
        lastFormat = config->data.general.defaultFormat;

    cDefaultFormat->clear();

    if( profile == tr("Last used") )
    {
        cDefaultFormat->addItem( tr("Last used") );
    }
    else if( profile == tr("Very low") || profile == tr("Low") || profile == tr("Medium") || profile == tr("High") || profile == tr("Very high") )
    {
        cDefaultFormat->addItem( tr("Last used") );
        cDefaultFormat->addItems( config->pluginLoader()->formatList(PluginLoader::Encode,PluginLoader::Lossy) );
    }
    else if( profile == tr("Lossless") )
    {
        cDefaultFormat->addItem( tr("Last used") );
        cDefaultFormat->addItems( config->pluginLoader()->formatList(PluginLoader::Encode,PluginLoader::Lossless) );
    }
    else if( profile == tr("Hybrid") )
    {
        cDefaultFormat->addItem( tr("Last used") );
        cDefaultFormat->addItems( config->pluginLoader()->formatList(PluginLoader::Encode,PluginLoader::Hybrid) );
    }
    else
    {
        const ConversionOptions *conversionOptions = config->data.profiles.value( profile );
        if( conversionOptions )
            cDefaultFormat->addItem( conversionOptions->codecName );
    }

    if( cDefaultFormat->findText(lastFormat) != -1 )
        cDefaultFormat->setCurrentIndex( cDefaultFormat->findText(lastFormat) );
}
