#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QObject>
#include <QLabel>

#include "interfaces/constants.h"
#include "interfaces/pluginsiteminterface.h"
#include "interfaces/pluginproxyinterface.h"

<<<<<<< HEAD
#include "pluginwidget.h"
=======
>>>>>>> ee69e7f2e5856481aa5f131a0220dbce2cb7f995

class MyPlugin : public QObject, public PluginsItemInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "MyPlugin.json")

public:
    MyPlugin(QObject *parent = 0);
<<<<<<< HEAD
    //Dock接口函数
    const QString pluginName() const;
    void init(PluginProxyInterface *proxyInter);
    QWidget *itemWidget(const QString &itemKey);    
    QWidget *itemTipsWidget(const QString &itemKey);
    const QString itemContextMenu(const QString &itemKey);
    void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked);
    //bool eventFilter(QObject *watched, QEvent *event);
    const QString itemCommand(const QString &itemKey);

private slots:
    void update();
    void requestContextMenu(const QString &itemKey);

private:
    //QWidget *m_mainWidget;
    PluginWidget *m_mainWidget;
=======

    //Dock接口函数
    const QString pluginName() const;
    void init(PluginProxyInterface *proxyInter);
    QWidget *itemWidget(const QString &itemKey);

    //
    QWidget *itemTipsWidget(const QString &itemKey);

private slots:
    void updateString();

private:
    QWidget *m_mainWidget;
>>>>>>> ee69e7f2e5856481aa5f131a0220dbce2cb7f995
    QLabel *m_tipsLabel;
    QTimer *m_refershTimer;
};

#endif // MYPLUGIN_H
