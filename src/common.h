/*!
 * common.h is a header that incorporates all the Qt5/6 and KF5/6 specific includes in one include.
 * It reduces boilerplate, enables tweaks to compatibility to be propagated quickly and
 * hides complexity
 */

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <KPluginFactory>
#else
#include <KGenericFactory>
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QUrl>
#else
#include <KUrl>
#endif
