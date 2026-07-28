
#include "lamecodecglobal.h"

#include "lamecodecwidget.h"
#include "lameconversionoptions.h"

#include <QApplication>
#include <QLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLocale>
#include <KComboBox>
#include <QSpinBox>

#include <KLineEdit>


LameCodecWidget::LameCodecWidget()
    : CodecWidget(),
    currentFormat( "mp3" )
{
    const int fontHeight = QFontMetrics(QApplication::font()).boundingRect("M").size().height();

    QGridLayout *grid = new QGridLayout( this );
    grid->setContentsMargins( 0, 0, 0, 0 );

    // set up preset selection

    QHBoxLayout *presetBox = new QHBoxLayout();
    grid->addLayout( presetBox, 0, 0 );

    QLabel *lPreset = new QLabel( tr("Preset:"), this );
    presetBox->addWidget( lPreset );
    cPreset = new KComboBox( this );
    cPreset->addItem( tr("Backend profile","Medium") );
    cPreset->addItem( tr("Backend profile","Standard") );
    cPreset->addItem( tr("Backend profile","Extreme") );
    cPreset->addItem( tr("Backend profile","Insane") );
    cPreset->addItem( tr("Specify bitrate") );
    cPreset->addItem( tr("User defined") );
    connect( cPreset, SIGNAL(activated(const QString&)), this, SLOT(presetChanged(const QString&)) );
    connect( cPreset, SIGNAL(activated(int)), SIGNAL(optionsChanged()) );
    presetBox->addWidget( cPreset );
//     cPreset->setToolTip( tr("Either use one of lames's presets or your own settings.") );

    iPresetBitrate = new QSpinBox( this );
    iPresetBitrate->setRange( 8, 320 );
    iPresetBitrate->setSuffix( " kbps" );
    iPresetBitrate->setValue( 192 );
    connect( iPresetBitrate, SIGNAL(valueChanged(int)), this, SLOT(presetBitrateChanged(int)) );
    connect( iPresetBitrate, SIGNAL(valueChanged(int)), SIGNAL(optionsChanged()) );
    presetBox->addWidget( iPresetBitrate );

    cPresetBitrateCbr = new QCheckBox( tr("cbr"), this );
    connect( cPresetBitrateCbr, SIGNAL(toggled(bool)), SIGNAL(optionsChanged()) );
    presetBox->addWidget( cPresetBitrateCbr );
    cPresetBitrateCbr->setToolTip( tr("Encode using a constant bitrate.\nOnly works with 80, 96, 112, 128, 160, 192, 224, 256 and 320 kbps") );

    presetBox->addSpacing( fontHeight );

    cPresetFast = new QCheckBox( tr("Fast encoding"), this );
    connect( cPresetFast, SIGNAL(toggled(bool)), SIGNAL(optionsChanged()) );
    presetBox->addWidget( cPresetFast );
    cPresetFast->setToolTip( tr("Use a faster encoding algorithm (results in a slightly lower output quality).") );

    presetBox->addStretch();

    // set up user defined options

    userdefinedBox = new QGroupBox( /*tr("User defined"),*/ this );
    grid->addWidget( userdefinedBox, 1, 0 );

    QVBoxLayout *userdefinedBoxLayout = new QVBoxLayout();
    userdefinedBox->setLayout( userdefinedBoxLayout );

    // top box ----------------------------------------

    QHBoxLayout *userdefinedTopBox = new QHBoxLayout();
    userdefinedBoxLayout->addLayout( userdefinedTopBox );

    QLabel *lMode = new QLabel( tr("Mode:"), userdefinedBox );
    userdefinedTopBox->addWidget( lMode );

    cMode = new KComboBox( userdefinedBox );
    cMode->addItem( tr("Quality") );
    cMode->addItem( tr("Bitrate") );
    connect( cMode, SIGNAL(activated(int)), this, SLOT(modeChanged(int)) );
    connect( cMode, SIGNAL(activated(int)), SIGNAL(optionsChanged()) );
    userdefinedTopBox->addWidget( cMode );

    sQuality = new QSlider( Qt::Horizontal, userdefinedBox );
    sQuality->setRange( 8, 320 );
    connect( sQuality, SIGNAL(valueChanged(int)), this, SLOT(qualitySliderChanged(int)) );
    connect( sQuality, SIGNAL(valueChanged(int)), SIGNAL(optionsChanged()) );
    userdefinedTopBox->addWidget( sQuality );

    iQuality = new QSpinBox( userdefinedBox );
    iQuality->setRange( 8, 320 );
    iQuality->setSuffix( " kbps" );
    iQuality->setFixedWidth( iQuality->sizeHint().width() );
    connect( iQuality, SIGNAL(valueChanged(int)), this, SLOT(qualitySpinBoxChanged(int)) );
    connect( iQuality, SIGNAL(valueChanged(int)), SIGNAL(optionsChanged()) );
    userdefinedTopBox->addWidget( iQuality );

    userdefinedTopBox->addSpacing( fontHeight );

    QLabel *lBitrateMode = new QLabel( tr("Bitrate mode:"), this );
    userdefinedTopBox->addWidget( lBitrateMode );
    cBitrateMode = new KComboBox( this );
    cBitrateMode->addItem( tr("Variable") );
    cBitrateMode->addItem( tr("Average") );
    cBitrateMode->addItem( tr("Constant") );
    cBitrateMode->setFixedWidth( cBitrateMode->sizeHint().width() );
    connect( cBitrateMode, SIGNAL(activated(int)), SIGNAL(optionsChanged()) );
    userdefinedTopBox->addWidget( cBitrateMode );

    userdefinedTopBox->addStretch();

    // bottom box

    QHBoxLayout *bottomBox = new QHBoxLayout();
    grid->addLayout( bottomBox, 2, 0 );

    QLabel *lCompressionLevel = new QLabel( tr("Compression level:"), this );
    bottomBox->addWidget( lCompressionLevel );

    sCompressionLevel = new QSlider( Qt::Horizontal, this );
    sCompressionLevel->setRange( 0, 9 );
    sCompressionLevel->setSingleStep( 1 );
    sCompressionLevel->setPageStep( 1 );
    connect( sCompressionLevel, SIGNAL(valueChanged(int)), this, SLOT(compressionLevelSliderChanged(int)) );
    connect( sCompressionLevel, SIGNAL(valueChanged(int)), SIGNAL(optionsChanged()) );
    bottomBox->addWidget( sCompressionLevel );
    sCompressionLevel->setToolTip( tr("Compression level from %1 to %2 where %2 is the best compression.\nThe better the compression, the slower the conversion but the smaller the file size and vice versa.\nA value of %3 is recommended.", 9, 0, 2) );

    iCompressionLevel = new QSpinBox( this );
    iCompressionLevel->setRange( 0, 9 );
    iCompressionLevel->setSingleStep( 1 );
    iCompressionLevel->setFixedWidth( iCompressionLevel->sizeHint().width() );
    connect( iCompressionLevel, SIGNAL(valueChanged(int)), this, SLOT(compressionLevelSpinBoxChanged(int)) );
    connect( iCompressionLevel, SIGNAL(valueChanged(int)), SIGNAL(optionsChanged()) );
    bottomBox->addWidget( iCompressionLevel );
    iCompressionLevel->setToolTip( tr("Compression level from %1 to %2 where %2 is the best compression.\nThe better the compression, the slower the conversion but the smaller the file size and vice versa.\nA value of %3 is recommended.", 9, 0, 2) );

    bottomBox->addSpacing( fontHeight );

    cCmdArguments = new QCheckBox( tr("Additional encoder arguments:"), this );
    bottomBox->addWidget( cCmdArguments );
    lCmdArguments = new KLineEdit( this );
    lCmdArguments->setEnabled( false );
    bottomBox->addWidget( lCmdArguments );
    connect( cCmdArguments, SIGNAL(toggled(bool)), lCmdArguments, SLOT(setEnabled(bool)) );

    grid->setRowStretch( 3, 1 );

    presetChanged( cPreset->currentText() );
    modeChanged( 0 );
    iCompressionLevel->setValue( 2 );
}

LameCodecWidget::~LameCodecWidget()
{}

// TODO optimize
int LameCodecWidget::bitrateForQuality( int quality )
{
    return 320-quality*32;
}

// TODO optimize
int LameCodecWidget::qualityForBitrate( int bitrate )
{
    return (320-bitrate)/32;
}

ConversionOptions *LameCodecWidget::currentConversionOptions()
{
    LameConversionOptions *options = new LameConversionOptions();
    options->data.preset = (LameConversionOptions::Data::Preset)cPreset->currentIndex();
    options->data.presetBitrate = iPresetBitrate->value();
    options->data.presetBitrateCbr = cPresetBitrateCbr->isEnabled() && cPresetBitrateCbr->isChecked();
    options->data.presetFast = cPresetFast->isEnabled() && cPresetFast->isChecked();
    if( cMode->currentText() == tr("Quality") )
    {
        options->qualityMode = ConversionOptions::Quality;
        options->quality = iQuality->value();
        options->bitrate = bitrateForQuality( options->quality );
    }
    else
    {
        options->qualityMode = ConversionOptions::Bitrate;
        options->bitrate = iQuality->value();
        options->quality = qualityForBitrate( options->bitrate );
        options->bitrateMode = ( cBitrateMode->currentText()==tr("Average") ) ? ConversionOptions::Abr : ConversionOptions::Cbr;
    }
    options->compressionLevel = iCompressionLevel->value();
    options->cmdArguments = cCmdArguments->isChecked() ? lCmdArguments->text() : "";

    return options;
}

bool LameCodecWidget::setCurrentConversionOptions( const ConversionOptions *_options )
{
    if( !_options || _options->pluginName != global_plugin_name )
        return false;

    const LameConversionOptions *options = dynamic_cast<const LameConversionOptions*>(_options);
    cPreset->setCurrentIndex( (int)options->data.preset );
    presetChanged( cPreset->currentText() );
    iPresetBitrate->setValue( options->data.presetBitrate );
    cPresetBitrateCbr->setChecked( options->data.presetBitrateCbr );
    cPresetFast->setChecked( options->data.presetFast );
    if( options->qualityMode == ConversionOptions::Quality )
    {
        cMode->setCurrentIndex( cMode->findText(tr("Quality")) );
        modeChanged( cMode->currentIndex() );
        iQuality->setValue( options->quality );
        cBitrateMode->setCurrentIndex( cBitrateMode->findText(tr("Variable")) );
    }
    else
    {
        cMode->setCurrentIndex( cMode->findText(tr("Bitrate")) );
        modeChanged( cMode->currentIndex() );
        iQuality->setValue( options->bitrate );
        if( options->bitrateMode == ConversionOptions::Abr )
            cBitrateMode->setCurrentIndex( cBitrateMode->findText(tr("Average")) );
        else
            cBitrateMode->setCurrentIndex( cBitrateMode->findText(tr("Constant")) );
    }
    iCompressionLevel->setValue( options->compressionLevel );
    cCmdArguments->setChecked( !options->cmdArguments.isEmpty() );
    if( !options->cmdArguments.isEmpty() )
        lCmdArguments->setText( options->cmdArguments );
    else
        lCmdArguments->clear();

    return true;
}

void LameCodecWidget::setCurrentFormat( const QString& format )
{
    if( currentFormat == format )
        return;

    currentFormat = format;
    setEnabled( currentFormat != "wav" );
}

QString LameCodecWidget::currentProfile()
{
    if( currentFormat == "wav" )
    {
        return tr("Lossless");
    }
    else if( cPreset->currentIndex() == 5 && cMode->currentIndex() == 0 && iQuality->value() == 6 )
    {
        return tr("Very low");
    }
    else if( cPreset->currentIndex() == 5 && cMode->currentIndex() == 0 && iQuality->value() == 5 )
    {
        return tr("Low");
    }
    else if( cPreset->currentIndex() == 5 && cMode->currentIndex() == 0 && iQuality->value() == 4 )
    {
        return tr("Medium");
    }
    else if( cPreset->currentIndex() == 5 && cMode->currentIndex() == 0 && iQuality->value() == 3 )
    {
        return tr("High");
    }
    else if( cPreset->currentIndex() == 5 && cMode->currentIndex() == 0 && iQuality->value() == 2 )
    {
        return tr("Very high");
    }

    return tr("User defined");
}

bool LameCodecWidget::setCurrentProfile( const QString& profile )
{
    cCmdArguments->setChecked( false );
    lCmdArguments->clear();

    if( profile == tr("Very low") )
    {
        cPreset->setCurrentIndex( 5 );
        presetChanged( cPreset->currentText() );
        cMode->setCurrentIndex( 0 );
        modeChanged( 0 );
        sQuality->setValue( 6 );
        iQuality->setValue( 6 );
        cBitrateMode->setCurrentIndex( 0 );
        return true;
    }
    else if( profile == tr("Low") )
    {
        cPreset->setCurrentIndex( 5 );
        presetChanged( cPreset->currentText() );
        cMode->setCurrentIndex( 0 );
        modeChanged( 0 );
        sQuality->setValue( 5 );
        iQuality->setValue( 5 );
        cBitrateMode->setCurrentIndex( 0 );
        return true;
    }
    else if( profile == tr("Medium") )
    {
        cPreset->setCurrentIndex( 5 );
        presetChanged( cPreset->currentText() );
        cMode->setCurrentIndex( 0 );
        modeChanged( 0 );
        sQuality->setValue( 4 );
        iQuality->setValue( 4 );
        cBitrateMode->setCurrentIndex( 0 );
        return true;
    }
    else if( profile == tr("High") )
    {
        cPreset->setCurrentIndex( 5 );
        presetChanged( cPreset->currentText() );
        cMode->setCurrentIndex( 0 );
        modeChanged( 0 );
        sQuality->setValue( 3 );
        iQuality->setValue( 3 );
        cBitrateMode->setCurrentIndex( 0 );
        return true;
    }
    else if( profile == tr("Very high") )
    {
        cPreset->setCurrentIndex( 5 );
        presetChanged( cPreset->currentText() );
        cMode->setCurrentIndex( 0 );
        modeChanged( 0 );
        sQuality->setValue( 2 );
        iQuality->setValue( 2 );
        cBitrateMode->setCurrentIndex( 0 );
        return true;
    }

    return false;
}

int LameCodecWidget::currentDataRate()
{
    int dataRate = 0;

    if( currentFormat == "wav" )
    {
        dataRate = 10590000;
    }
    else
    {
        if( cPreset->currentIndex() == 0 )
        {
            dataRate = 1090000;
        }
        else if( cPreset->currentIndex() == 1 )
        {
            dataRate = 1140000;
        }
        else if( cPreset->currentIndex() == 2 )
        {
            dataRate = 1400000;
        }
        else if( cPreset->currentIndex() == 3 )
        {
            dataRate = 2360000;
        }
        else if( cPreset->currentIndex() == 4 )
        {
            dataRate = iPresetBitrate->value()/8*60*1000;
        }
        else if( cPreset->currentIndex() == 5 )
        {
            if( cMode->currentIndex() == 0 )
            {
                dataRate = 1500000 - iQuality->value()*100000;
            }
            else
            {
                dataRate = iQuality->value()/8*60*1000;
            }
        }
        if( cPresetFast->isEnabled() && cPresetFast->isChecked() )
        {
            dataRate *= 1.1f;
        }
    }

    return dataRate;
}

void LameCodecWidget::presetChanged( const QString& preset )
{
    cPreset->setToolTip( "" );

    if( preset == tr("Backend profile","Medium") )
    {
        iPresetBitrate->setEnabled( false );
        cPresetFast->setEnabled( true );
        userdefinedBox->setEnabled( false );
        cPreset->setToolTip( tr("This preset should provide near transparency to most people on most music (~150 kbps abr).") );
    }
    else if( preset == tr("Backend profile","Standard") )
    {
        iPresetBitrate->setEnabled( false );
        cPresetFast->setEnabled( true );
        userdefinedBox->setEnabled( false );
        cPreset->setToolTip( tr("This preset should generally be transparent to most people on most music\nand is already quite high in quality (~230 kbps abr).") );
    }
    else if( preset == tr("Backend profile","Extreme") )
    {
        iPresetBitrate->setEnabled( false );
        cPresetFast->setEnabled( true );
        userdefinedBox->setEnabled( false );
        cPreset->setToolTip( tr("If you have extremely good hearing and similar equipment, this preset will generally provide\nslightly higher quality than the standard mode (~280 kbps abr).") );
    }
    else if( preset == tr("Backend profile","Insane") )
    {
        iPresetBitrate->setEnabled( false );
        cPresetFast->setEnabled( false );
        userdefinedBox->setEnabled( false );
        cPreset->setToolTip( tr("This preset will usually be overkill for most people and most situations, but if you must have\nthe absolute highest quality with no regard to filesize, this is the way to go (320 kbps cbr).") );
    }
    else if( preset == tr("Specify bitrate") )
    {
        iPresetBitrate->setEnabled( true );
        cPresetFast->setEnabled( false );
        userdefinedBox->setEnabled( false );
    }
    else // "User defined"
    {
        iPresetBitrate->setEnabled( false );
        cPresetFast->setEnabled( false );
        userdefinedBox->setEnabled( true );
    }
    presetBitrateChanged( iPresetBitrate->value() );
}

void LameCodecWidget::presetBitrateChanged( int bitrate )
{
    if( iPresetBitrate->isEnabled() && (
        bitrate == 80 || bitrate == 96 || bitrate == 112 ||
        bitrate == 128 || bitrate == 160 || bitrate == 192 ||
        bitrate == 224 || bitrate == 256 || bitrate == 320 ) )
    {
        cPresetBitrateCbr->setEnabled( true );
    }
    else
    {
        cPresetBitrateCbr->setEnabled( false );
    }
}

void LameCodecWidget::modeChanged( int mode )
{
    if( mode == 0 )
    {
        sQuality->setRange( 0, 9 );
//         sQuality->setTickInterval( 100 );
        sQuality->setSingleStep( 1 );
        iQuality->setRange( 0, 9 );
        iQuality->setSingleStep( 1 );
        iQuality->setSuffix( "" );
        sQuality->setValue( 5 );
        iQuality->setValue( 5 );
//         dQuality->setValue( qualityForBitrate(dQuality->value()) );
//         qualitySpinBoxChanged( dQuality->value() );
        sQuality->setToolTip( tr("Quality level from %1 to %2 where %2 is the highest quality.\nThe higher the quality, the bigger the file size and vice versa.", 9, 0) );
        iQuality->setToolTip( tr("Quality level from %1 to %2 where %2 is the highest quality.\nThe higher the quality, the bigger the file size and vice versa.", 9, 0) );

        cBitrateMode->clear();
        cBitrateMode->addItem( tr("Variable") );
        cBitrateMode->setEnabled( false );
    }
    else
    {
        sQuality->setRange( 8, 320 );
//         sQuality->setTickInterval( 800 );
        sQuality->setSingleStep( 8 );
        iQuality->setRange( 8, 320 );
        iQuality->setSingleStep( 1 );
        iQuality->setSuffix( " kbps" );
        sQuality->setValue( 160 );
        iQuality->setValue( 160 );
//         dQuality->setValue( bitrateForQuality(dQuality->value()) );
//         qualitySpinBoxChanged( dQuality->value() );
        sQuality->setToolTip( "" );
        iQuality->setToolTip( "" );

        cBitrateMode->clear();
        cBitrateMode->addItem( tr("Average") );
        cBitrateMode->addItem( tr("Constant") );
        cBitrateMode->setEnabled( true );
    }
}

void LameCodecWidget::qualitySliderChanged( int quality )
{
    if( cMode->currentText() == tr("Quality") )
    {
        iQuality->setValue( 9 - quality );
    }
    else
    {
        iQuality->setValue( quality );
    }
}

void LameCodecWidget::qualitySpinBoxChanged( int quality )
{
    if( cMode->currentText() == tr("Quality") )
    {
        sQuality->setValue( 9 - quality );
    }
    else
    {
        sQuality->setValue( quality );
    }
}

void LameCodecWidget::compressionLevelSliderChanged( int quality )
{
    iCompressionLevel->setValue( 9 - quality );
}

void LameCodecWidget::compressionLevelSpinBoxChanged( int quality )
{
    sCompressionLevel->setValue( 9 - quality );
}


