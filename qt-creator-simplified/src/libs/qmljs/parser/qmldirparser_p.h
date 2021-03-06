/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QDebug>
#include "qmljsengine_p.h"
#include "qmljsglobal_p.h"

QT_BEGIN_NAMESPACE

class QmlError;
class QmlEngine;
class QML_PARSER_EXPORT QmlDirParser
{
public:
    QmlDirParser();
    ~QmlDirParser();

    bool parse(const QString &source);

    bool hasError() const;
    void setError(const QmlError &);
    QList<QmlError> errors(const QString &uri) const;

    QString typeNamespace() const;
    void setTypeNamespace(const QString &s);

    struct Plugin
    {
        Plugin() {}

        Plugin(const QString &name, const QString &path)
            : name(name), path(path) {}

        QString name;
        QString path;
    };

    struct Component
    {
        Component() {}

        Component(const QString &typeName, const QString &fileName, int majorVersion, int minorVersion)
            : typeName(typeName), fileName(fileName), majorVersion(majorVersion), minorVersion(minorVersion),
            internal(false), singleton(false) {}

        QString typeName;
        QString fileName;
        int majorVersion = 0;
        int minorVersion = 0;
        bool internal = false;
        bool singleton = false;
    };

    struct Script
    {
        Script() {}

        Script(const QString &nameSpace, const QString &fileName, int majorVersion, int minorVersion)
            : nameSpace(nameSpace), fileName(fileName), majorVersion(majorVersion), minorVersion(minorVersion) {}

        QString nameSpace;
        QString fileName;
        int majorVersion = 0;
        int minorVersion = 0;
    };

    QHash<QString,Component> components() const;
    QHash<QString,Component> dependencies() const;
    QList<Script> scripts() const;
    QList<Plugin> plugins() const;
    bool designerSupported() const;

#ifdef QT_CREATOR
    struct TypeInfo
    {
        TypeInfo() {}
        TypeInfo(const QString &fileName)
            : fileName(fileName) {}

        QString fileName;
    };

    QList<TypeInfo> typeInfos() const;
#endif

    QString className() const;

private:
    bool maybeAddComponent(const QString &typeName, const QString &fileName, const QString &version, QHash<QString,Component> &hash, int lineNumber = -1, bool multi = true);
    void reportError(quint16 line, quint16 column, const QString &message);

private:
    QList<QmlJS::DiagnosticMessage> _errors;
    QString _typeNamespace;
    QHash<QString,Component> _components; // multi hash
    QHash<QString,Component> _dependencies;
    QList<Script> _scripts;
    QList<Plugin> _plugins;
    bool _designerSupported;
#ifdef QT_CREATOR
    QList<TypeInfo> _typeInfos;
#endif
    QString _className;
};

typedef QHash<QString,QmlDirParser::Component> QmlDirComponents;
typedef QList<QmlDirParser::Script> QmlDirScripts;
typedef QList<QmlDirParser::Plugin> QmlDirPlugins;

QDebug &operator<< (QDebug &, const QmlDirParser::Component &);
QDebug &operator<< (QDebug &, const QmlDirParser::Script &);

QT_END_NAMESPACE

