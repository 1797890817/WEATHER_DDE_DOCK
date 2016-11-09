#include "myplugin.h"
#include <QLabel>
#include <QtNetwork>
#include <QJsonDocument>
#define MYPLUGIN_KEY    "myplugin"

QString city="",cityId="",swtips="",temp="";

MyPlugin::MyPlugin(QObject *parent) :
    QObject(parent),
    m_mainWidget(new QLabel),
    m_tipsLabel(new QLabel),
    m_refershTimer(new QTimer(this))
{
    QLabel *label = (QLabel *)m_mainWidget;
    label->setText("天气\n温度");
    label->setStyleSheet("color:white;padding:0px;");
    label->setAlignment(Qt::AlignCenter);
    //label->setFixedWidth(50);
    m_tipsLabel->setStyleSheet("color:white;padding:5px;");
    m_refershTimer->setInterval(1800000);
    m_refershTimer->start();
    connect(m_refershTimer, &QTimer::timeout, this, &MyPlugin::updateString);
    updateString();
}

//插件名
const QString MyPlugin::pluginName() const
{
    return "myplugin";
}

//插件初始化
void MyPlugin::init(PluginProxyInterface *proxyInter)
{
    m_proxyInter = proxyInter;
    m_proxyInter->itemAdded(this, MYPLUGIN_KEY);
}


//插件的Widget
QWidget *MyPlugin::itemWidget(const QString &itemKey)
{
    if (itemKey == MYPLUGIN_KEY)
        return m_mainWidget;
    return nullptr;
}

QWidget *MyPlugin::itemTipsWidget(const QString &itemKey)
{
    if (itemKey == MYPLUGIN_KEY) {
        //m_tipsLabel->setText("提示");
        return m_tipsLabel;
    }
    return nullptr;
}

void MyPlugin::updateString()
{
    QString URLSTR = "http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json";
    QUrl url(URLSTR);
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply;

    reply = manager.get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();
    QString codeContent = reply->readAll();
    qDebug() << URLSTR + " -> " << codeContent;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(codeContent.toLatin1(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("city"))
            {
                QJsonValue city_value = obj.take("city");
                if(city_value.isString())
                {
                    city = city_value.toString();
                }
            }
        }
    }

    URLSTR="http://hao.weidunewtab.com/tianqi/city.php?city="+city;
    url.setUrl(URLSTR);
    reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    cityId = reply->readAll();
    qDebug() << URLSTR + " -> " << cityId;


    URLSTR="http://hao.weidunewtab.com/myapp/weather/data/indexInTime.php?cityID="+cityId;
    url.setUrl(URLSTR);
    reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    codeContent = reply->readAll();
    qDebug() << URLSTR + " -> " << codeContent;
    parse_doucment = QJsonDocument::fromJson(codeContent.toLatin1(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("weatherinfo")){
                const QDateTime currentDateTime = QDateTime::currentDateTime();
                QJsonObject::iterator it;
                it = obj.find("weatherinfo");
                QJsonObject weatherinfoObj = it.value().toObject();
                swtips = "城市："+weatherinfoObj.value("city").toString()+"\n温度："+weatherinfoObj.value("temp").toString()+"°C\n湿度："+weatherinfoObj.value("SD").toString()+"\n风力："+weatherinfoObj.value("WD").toString()+weatherinfoObj.value("WS").toString()+"\nPM2.5："+weatherinfoObj.value("pm25").toString()+"\n空气质量指数："+ QString::number(weatherinfoObj.value("aqiLevel").toInt())+"\n刷新："+currentDateTime.toString("HH:mm:ss");
                m_tipsLabel->setText(swtips);
                temp=weatherinfoObj.value("temp").toString()+"°C";
            }
        }
    }


    URLSTR="http://hao.weidunewtab.com/myapp/weather/data/index.php?cityID="+cityId;
    url.setUrl(URLSTR);
    reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    codeContent = reply->readAll();
    qDebug() << URLSTR + " -> " << codeContent;

    parse_doucment = QJsonDocument::fromJson(codeContent.toLatin1(), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("weatherinfo")){
                QJsonObject::iterator it;
                it = obj.find("weatherinfo");
                QJsonObject weatherinfoObj = it.value().toObject();
                QString sw1 = weatherinfoObj.value("weather1").toString();
                QLabel *label = (QLabel *)m_mainWidget;
                label->setText(sw1+"\n"+temp);
            }
        }
    }
}
