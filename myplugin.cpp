#include "myplugin.h"
#include <QLabel>
#include <QtNetwork>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QGridLayout>
#define MYPLUGIN_KEY    "myplugin"

QString city="",cityId="",swtips="",temp="",sw1="";
QLabel *label;
QGridLayout *layout;
QLabel *labelTemp,*labelCity,*labelSD,*labelWind,*labelPM,*labelAQI,*labelRT,*labelDate[7],*labelWImg[7],*labelWeather[7];
QWidget *window;
QDesktopWidget* desktop;

MyPlugin::MyPlugin(QObject *parent) :
    QObject(parent),
    m_mainWidget(new QLabel),
    m_tipsLabel(new QLabel),
    m_refershTimer(new QTimer(this))
{
    label = (QLabel *)m_mainWidget;
    label->setText("天气\n温度");
    label->setStyleSheet("color:white;padding:0px;");
    label->setAlignment(Qt::AlignCenter);
    //label->setFixedWidth(50);
    m_tipsLabel->setStyleSheet("color:white;padding:5px;");

    //window
    desktop = QApplication::desktop();
    window=new QWidget;
    window->setWindowTitle("中国天气预报");
    window->setFixedSize(500,220);
    //背景颜色
    //QPalette p = window->palette();
    //p.setColor(QPalette::Window,QColor(0,0,0));
    //window->setPalette(p);
    //居中
    window->move((desktop->width() - window->width())/2, (desktop->height() - window->height())/2);
    //window->setCentralWidget(widget);
    // 移除最小化
    window->setWindowFlags((window->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMinimizeButtonHint );
    // 不在任务栏显示
    window->setWindowFlags(Qt::Tool);
    // 隐藏标题栏
    //window->setWindowFlags(Qt::FramelessWindowHint);
    // 背景透明
    //window->setAttribute(Qt::WA_TranslucentBackground, true);
    // 窗体透明
    window->setWindowOpacity(0.9);

    layout = new QGridLayout;
    labelCity = new QLabel("城市");
    labelCity->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelCity,0,0);
    labelTemp = new QLabel("温度\n?");
    labelTemp->setStyleSheet("font-size:40px;");
    labelTemp->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelTemp,0,1);
    labelSD = new QLabel("湿度\n?");
    labelSD->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelSD,0,2);
    labelWind = new QLabel("风向?\n风力?");
    labelWind->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelWind,0,3);
    labelPM = new QLabel("PM2.5\n?");
    labelPM->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelPM,0,4);
    labelAQI = new QLabel("空气质量指数\n?");
    labelAQI->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelAQI,0,5);
    labelRT = new QLabel("刷新\n?");
    labelRT->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelRT,0,6);
    for(int i=1;i<8;i++){
        labelDate[i-1] = new QLabel("1月1日\n星期一");
        labelDate[i-1]->setAlignment(Qt::AlignCenter);
        layout->addWidget(labelDate[i-1],1,i-1);
        labelWImg[i-1] = new QLabel("");
        QPixmap pixmap;
        pixmap.load(":/icon.ico");
        labelWImg[i-1]->setPixmap(pixmap);
        labelWImg[i-1]->setAlignment(Qt::AlignCenter);
        layout->addWidget(labelWImg[i-1],2,i-1);
        labelWeather[i-1] = new QLabel("晴\n15°C ~ 20°C\n北风1级");
        labelWeather[i-1]->setAlignment(Qt::AlignCenter);
        layout->addWidget(labelWeather[i-1],3,i-1);
    }
    window->setLayout(layout);

    //Timer
    m_refershTimer->setInterval(1800000);
    m_refershTimer->start();
    connect(m_refershTimer, &QTimer::timeout, this, &MyPlugin::update);
    update();
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

//点击响应
const QString MyPlugin::itemCommand(const QString &itemKey)
{
    Q_UNUSED(itemKey);
    window->hide();
    window->move((desktop->width() - window->width())/2, (desktop->height() - window->height())/2);
    window->show();
    return "";
}

//右键菜单
const QString MyPlugin::itemContextMenu(const QString &itemKey)
{
    Q_UNUSED(itemKey)
    QList<QVariant> items;
    items.reserve(2);
    QMap<QString, QVariant> forecast;
    forecast["itemId"] = "forecast";
    forecast["itemText"] = "预报";
    forecast["isActive"] = true;
    items.push_back(forecast);
    QMap<QString, QVariant> about;
    about["itemId"] = "about";
    about["itemText"] = "关于";
    about["isActive"] = true;
    items.push_back(about);
    QMap<QString, QVariant> menu;
    menu["items"] = items;
    menu["checkableMenu"] = false;
    menu["singleCheck"] = false;
    return QJsonDocument::fromVariant(menu).toJson();
}

void MyPlugin::invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked)
{
    Q_UNUSED(itemKey);
    Q_UNUSED(checked);
//    if (menuId == MenuIdOpenEyes) {
//        QProcess::startDetached("xeyes");
//    } else if (menuId == MenuIdCloseEyes) {
//        QProcess::startDetached("killall xeyes");
//    }
    QStringList menuitems;
    menuitems << "forecast" << "about" ;
    switch(menuitems.indexOf(menuId)){
    case 0:
        qDebug() << "Menu:Forcast";
        break;
    case 1:
        qDebug() << "Menu:About";
        break;
    }
}

bool MyPlugin::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    Q_UNUSED(event);
    //if (watched == label && event->type() == QEvent::MouseButtonPress) {
     //   QMouseEvent *mouse = static_cast<QMouseEvent*>(event);
       // if(mouse->button()==Qt::RightButton) {
            m_proxyInter->requestContextMenu(this, "");
            return true;
        //}
    //}
    //return false;
}

//更新
void MyPlugin::update()
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
                    labelCity->setText(city);
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
                swtips = weatherinfoObj.value("temp").toString() + "°C\n湿度：" + weatherinfoObj.value("SD").toString() + "\n" + weatherinfoObj.value("WD").toString() + weatherinfoObj.value("WS").toString() + "\nPM2.5：" + weatherinfoObj.value("pm25").toString() + "\n空气质量指数："+ QString::number(weatherinfoObj.value("aqiLevel").toInt()) + "\n刷新：" + currentDateTime.toString("HH:mm:ss");
                temp=weatherinfoObj.value("temp").toString()+"°C";
                labelTemp->setText(weatherinfoObj.value("temp").toString()+"°C");
                labelSD->setText("湿度\n" + weatherinfoObj.value("SD").toString());
                labelWind->setText(weatherinfoObj.value("WD").toString() + "\n" + weatherinfoObj.value("WS").toString());
                labelPM->setText("PM2.5\n" + weatherinfoObj.value("pm25").toString());
                labelAQI->setText("空气质量指数\n" + QString::number(weatherinfoObj.value("aqiLevel").toInt()));
                labelRT->setText("刷新\n" + currentDateTime.toString("HH:mm:ss"));
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
                sw1 = weatherinfoObj.value("weather1").toString();
                QLabel *label = (QLabel *)m_mainWidget;
                label->setText(sw1+"\n"+temp);
                QDateTime date = QDateTime::fromString(weatherinfoObj.value("date_y").toString(), "yyyy年M月d");
                for(int i=1;i<8;i++){
                    labelDate[i-1]->setText(date.addDays(i-1).toString("M-d")+"\n"+date.addDays(i-1).toString("dddd"));
                    labelDate[i-1]->setAlignment(Qt::AlignCenter);
                    URLSTR="http://m.weather.com.cn/weather_img/"+ QString::number(weatherinfoObj.value("img"+QString::number(2*i-1)).toInt()) + ".gif";
                    url.setUrl(URLSTR);
                    reply = manager.get(QNetworkRequest(url));
                    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
                    loop.exec();
                    QPixmap pixmap;
                    pixmap.loadFromData(reply->readAll());
                    labelWImg[i-1]->setPixmap(pixmap);
                    labelWImg[i-1]->setAlignment(Qt::AlignCenter);
                    labelWeather[i-1]->setText(weatherinfoObj.value("weather"+QString::number(i)).toString()+"\n"+weatherinfoObj.value("temp"+QString::number(i)).toString()+"\n"+weatherinfoObj.value("wind"+QString::number(i)).toString());
                    labelWeather[i-1]->setAlignment(Qt::AlignCenter);
                }
            }
        }
    }

    m_tipsLabel->setText(city+"\n"+sw1+"\n"+swtips);

}
