#include "QWidget"
#include "QWindow"
#include "QApplication"
#include "QHBoxLayout"
#include "QPushButton"
#include "QPointer"
#include "iostream"

class BotnetServerStarter : public QWidget{
protected:
    std::unique_ptr<QHBoxLayout> lay;
    std::unique_ptr<QPushButton> svb;
public:
    BotnetServerStarter() : QWidget(){
        setWindowTitle("Server Configuration");
        setFixedWidth(360);
        setFixedHeight(240);
        lay = std::make_unique<QHBoxLayout>(this);
        svb = std::make_unique<QPushButton>("Server");
        lay->addWidget(svb.get());
}
};

class BotnetClientStarter : public QWidget{
protected:
    std::unique_ptr<QHBoxLayout> lay;
    std::unique_ptr<QPushButton> clb;
public:
    BotnetClientStarter() : QWidget(){
        setWindowTitle("Client Configuration");
        setFixedWidth(360);
        setFixedHeight(240);
        lay = std::make_unique<QHBoxLayout>(this);
        clb = std::make_unique<QPushButton>("Client");
        lay->addWidget(clb.get());
    }
};

class BotnetStarter : public QWidget{
protected:
    std::unique_ptr<QHBoxLayout> lay;
    std::unique_ptr<QPushButton> svb;
    std::unique_ptr<QPushButton> clb;

    void configure_server(){
        this->close();
        auto conf = new BotnetServerStarter();
        conf->show();
    }

    void configure_client(){
        this->close();
        auto conf = new BotnetClientStarter();
        conf->show();
    }
public:
    BotnetStarter(QWidget *parent = nullptr) : QWidget(parent){
        setWindowTitle("Role Configuration");
        setFixedWidth(360);
        lay = std::make_unique<QHBoxLayout>(this);
        svb = std::make_unique<QPushButton>("Server");
        connect(svb.get(), &QPushButton::clicked, this, &BotnetStarter::configure_server);
        clb = std::make_unique<QPushButton>("Client");
        connect(clb.get(), &QPushButton::clicked, this, &BotnetStarter::configure_client);
        lay->addWidget(svb.get());
        lay->addWidget(clb.get());
}
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BotnetStarter window;
    window.show();
    return QApplication::exec();
}