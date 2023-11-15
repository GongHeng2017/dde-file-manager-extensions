// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DISKENCRYPTMENUSCENE_H
#define DISKENCRYPTMENUSCENE_H

#include "gui/encryptparamsinputdialog.h"

#include <dfm-base/interfaces/abstractmenuscene.h>
#include <dfm-base/interfaces/abstractscenecreator.h>

#include <dfm-mount/dmount.h>

#include <QUrl>

class QAction;

namespace dfmplugin_diskenc {

class DiskEncryptMenuCreator : public dfmbase::AbstractSceneCreator
{
    Q_OBJECT
    // AbstractSceneCreator interface
public:
    virtual dfmbase::AbstractMenuScene *create() override;
    static inline QString name()
    {
        return "DiskEncryptMenu";
    }
};

class DiskEncryptMenuScene : public dfmbase::AbstractMenuScene
{
    Q_OBJECT
public:
    explicit DiskEncryptMenuScene(QObject *parent = nullptr);

    // AbstractMenuScene interface
public:
    virtual QString name() const override;
    virtual bool initialize(const QVariantHash &params) override;
    virtual bool create(QMenu *parent) override;
    virtual bool triggered(QAction *action) override;
    virtual void updateState(QMenu *parent) override;

protected:
    static void encryptDevice(const QString &dev, const QString &uuid, bool paramsOnly = false);
    static void deencryptDevice(const QString &dev, const QString &uuid, bool paramsOnly = false);
    static void changePassphrase(const QString &dev, const QString &uuid, bool paramsOnly = false);

    static void doEncryptDevice(const ParamsInputs &inputs);
    static void doDecryptDevice(const QString &dev, const QString &passphrase, bool paramsOnly);
    static void doChangePassphrase(const QString &dev, const QString oldPass, const QString &newPass, bool validateByRec);

    void unmountBefore(const std::function<void(const QString &, const QString &, bool)> &after);
    enum OpType { kUnmount,
                  kLock };
    static void onUnmountError(OpType t, const QString &dev, const dfmmount::OperationErrorInfo &err);

private:
    QMap<QString, QAction *> actions;

    QUrl selectedItem;
    QString devDesc;
    bool itemEncrypted { false };
    bool operatingFstabDevice { false };
    QString uuid;
};

}

#endif   // DISKENCRYPTMENUSCENE_H