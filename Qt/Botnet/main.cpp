#include "QWidget"
#include "QWindow"
#include "QApplication"
#include "QHBoxLayout"
#include "QPushButton"
#include "iostream"
#include "QLineEdit"
#include "QValidator"
#include "thread"
#include "QSlider"
#include "QLabel"
#include "format"
#include "QString"
#include "../../lib/ConnectorBotnet.cpp"
#include "QImage"
#include "QPixmap"
#include "QFileDialog"
#include "QDataStream"
#include "QByteArray"

class BotnetServer : public QWidget{
private:
    std::unique_ptr<QVBoxLayout> lay;
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::vector<std::jthread> threads;
    std::unique_ptr<QLabel> info;
    std::unique_ptr<QLabel> stat_complete;
    std::unique_ptr<QLabel> stat_speed;
    std::unique_ptr<QLabel> stat_unique_id;
    std::unique_ptr<QLabel> stat_unique_ip;
    std::unique_ptr<QLabel> decrypted;
    std::unique_ptr<ConnectorBotnet> sv;
    std::unique_ptr<QPixmap> px;
    std::unique_ptr<QPushButton> savebtn;

    void save(){
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setNameFilter("Image (*.jpg)");
        dialog.setViewMode(QFileDialog::Detail);
        if (dialog.exec()){
            auto result = sv->getDecrypted();
            auto out = std::ofstream (dialog.selectedFiles().at(0).toStdString());
            out.write(result.data(), result.size());
            out.close();
        }
    }
public:
    BotnetServer(int thread_count, int port) : QWidget(){
        setWindowTitle("Botnet Server");
        setFixedWidth(600);
        setFixedHeight(400);
        sv = std::make_unique<ConnectorBotnet>(port, thread_count);
        px = std::make_unique<QPixmap>(500, 200);
        start_time = std::chrono::system_clock::now();
        lay = std::make_unique<QVBoxLayout>(this);
        lay->setAlignment(Qt::AlignCenter);
        std::string infostr = std::format("Using {} threads for processing requests at {}:{}", thread_count, sv->getPublicIP(), port);
        info = std::make_unique<QLabel>(QString(infostr.c_str()));
        stat_complete = std::make_unique<QLabel>("Completed chunks: ");
        stat_speed = std::make_unique<QLabel>("Speed: ");
        stat_unique_id = std::make_unique<QLabel>("Seen unique IDs: ");
        stat_unique_ip = std::make_unique<QLabel>("Seen unique IPs: ");
        decrypted = std::make_unique<QLabel>();
        decrypted->setFixedWidth(500);
        decrypted->setFixedHeight(200);
        lay->addWidget(info.get());
        lay->addWidget(stat_unique_ip.get());
        lay->addWidget(stat_unique_id.get());
        lay->addWidget(stat_complete.get());
        lay->addWidget(stat_speed.get());
        lay->addWidget(decrypted.get());
        savebtn = std::make_unique<QPushButton>("Save");
        connect(savebtn.get(), &QPushButton::clicked, this, &BotnetServer::save);
        lay->addWidget(savebtn.get());
        threads.emplace_back([this](){
            bool next_done = false;
            while (true){
                std::this_thread::sleep_for(std::chrono::milliseconds (1000 / 60));
                std::chrono::time_point<std::chrono::system_clock> current_time = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = current_time - start_time;
                auto complete = static_cast<int>(checksum::fileChecksum.size() - sv->getIncompleteCount());
                stat_complete->setText(std::format("Completed chunks: {} out of {} [{:.2f}%]", complete, checksum::fileChecksum.size(), static_cast<double>(100) * complete / static_cast<double>(checksum::fileChecksum.size())).c_str());
                double speed = complete / elapsed_seconds.count();
                if (speed < 1){
                    stat_speed->setText(std::format("Speed: {:.2f} s/chunk", 1/speed).c_str());
                }
                else{
                    stat_speed->setText(std::format("Speed: {:.2f} Chunks/s", speed).c_str());
                }
                stat_unique_id->setText(std::format("Seen unique IDs: {}", sv->getUniqueIDs()).c_str());
                stat_unique_ip->setText(std::format("Seen unique IPs: {}", sv->getUniqueAddresses()).c_str());
                auto pic = sv->getDecrypted();
                QImage image(500, 200, QImage::Format_RGB888);
                std::random_device rd;
                std::mt19937 gen(rd());
                while (pic.size() < image.sizeInBytes()){
                    auto mod = sv->getDecrypted();
                    for (int j = 0; j < mod.size(); j++){
                        pic.emplace_back(mod[j]);
                    }
                }
                std::shuffle(pic.begin(), pic.end(), gen);
                memcpy(image.bits(), pic.data(), std::min(pic.size(), uint64_t(image.sizeInBytes())));
                QPixmap pixmap = QPixmap::fromImage(image);
                px = std::make_unique<QPixmap>(pixmap);
                decrypted->setPixmap(*px);
                if (next_done)break;
                if (sv->getIncompleteCount() == 0){
                    savebtn->setEnabled(true);
                    next_done = true;
                }
                else{
                    savebtn->setEnabled(false);
                }

            }
        });
    }
};

class BotnetServerStarter : public QWidget{
private:
    std::unique_ptr<QVBoxLayout> lay;
    std::unique_ptr<QLabel> port_label;
    std::unique_ptr<QLineEdit> port;
    std::unique_ptr<QLabel> thread_label;
    std::unique_ptr<QSlider> thread_count;
    std::unique_ptr<QPushButton> start;

    void check_port(){
        int port_int = port->text().toInt();
        if(port_int > 65535){
            port->setText("65535");
        }
    }

    void start_server(){
        int thread_count_int = thread_count->value();
        int port_int = port->text().toInt();
        this->close();
        auto conf = new BotnetServer(thread_count_int, port_int);
        conf->show();
    }

public:
    BotnetServerStarter() : QWidget(){
        setWindowTitle("Server Configuration");
        setFixedWidth(400);
        setFixedHeight(240);
        lay = std::make_unique<QVBoxLayout>(this);
        port_label = std::make_unique<QLabel>("Input Server Port: ");
        port = std::make_unique<QLineEdit>("4000");
        port->setValidator(new QIntValidator(0, 65535, this));
        connect(port.get(), &QLineEdit::textChanged, this, &BotnetServerStarter::check_port);
        thread_label = std::make_unique<QLabel>("Input Thread Count: ");
        thread_count = std::make_unique<QSlider>(Qt::Horizontal);
        thread_count->setMinimum(1);
        thread_count->setMaximum(128);
        thread_count->setValue(12);
        thread_count->setTickPosition(QSlider::TicksBothSides);
        thread_count->setTickInterval(1);
        lay->addWidget(port_label.get());
        lay->addWidget(port.get());
        lay->addWidget(thread_label.get());
        lay->addWidget(thread_count.get());
        start = std::make_unique<QPushButton>("Start Server");
        connect(start.get(), &QPushButton::clicked, this, &BotnetServerStarter::start_server);
        lay->addWidget(start.get());
}
};

class BotnetClient : public QWidget{
private:
    std::unique_ptr<QVBoxLayout> lay;
    std::chrono::time_point<std::chrono::system_clock> start_time;
    int complete = 0;
    std::vector<std::jthread> threads;
    int exited = 0;
    std::unique_ptr<QLabel> info;
    std::unique_ptr<QLabel> stat_complete;
    std::unique_ptr<QLabel> stat_speed;
    std::mutex mutex;
public:
    BotnetClient(int thread_count, std::string address, int port) : QWidget(){
        setWindowTitle("Botnet Client");
        setFixedWidth(400);
        setFixedHeight(240);
        start_time = std::chrono::system_clock::now();
        lay = std::make_unique<QVBoxLayout>(this);
        lay->setAlignment(Qt::AlignCenter);
        std::string infostr = std::format("Using {} threads for decryption at {}:{}", thread_count, address, port);
        info = std::make_unique<QLabel>(QString(infostr.c_str()));
        lay->addWidget(info.get());
        stat_complete = std::make_unique<QLabel>("Completed chunks: ");
        lay->addWidget(stat_complete.get());
        stat_speed = std::make_unique<QLabel>("Speed: ");
        lay->addWidget(stat_speed.get());
        for (int i = 0; i < thread_count; i++){
            threads.emplace_back([this, address, port](){
                auto cl = std::make_unique<ConnectorBotnet>(address, port);
                while (true){
                    if (!cl->makeRequest()) break;
                    complete++;
                }
                std::scoped_lock lock(mutex);
                exited += 1;
            });
        }
        threads.emplace_back([this](){
            while (true){
                std::this_thread::sleep_for(std::chrono::milliseconds (1000 / 60));
                std::scoped_lock lock(mutex);
                std::chrono::time_point<std::chrono::system_clock> current_time = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = current_time - start_time;
                stat_complete->setText(std::format("Completed chunks: {}", complete).c_str());
                double speed = complete / elapsed_seconds.count();
                if (speed < 1){
                    stat_speed->setText(std::format("Speed: {:.2f} s/chunk", 1/speed).c_str());
                }
                else{
                    stat_speed->setText(std::format("Speed: {:.2f} Chunks/s", speed).c_str());
                }
                if (threads.size() - 1 == exited){
                    break;
                }
            }
        });
    }
};

class BotnetClientStarter : public QWidget{
private:
    std::unique_ptr<QVBoxLayout> lay;
    std::unique_ptr<QLabel> address_label;
    std::unique_ptr<QLineEdit> address;
    std::unique_ptr<QLabel> port_label;
    std::unique_ptr<QLineEdit> port;
    std::unique_ptr<QLabel> thread_label;
    std::unique_ptr<QSlider> thread_count;
    std::unique_ptr<QPushButton> start;

    void check_port(){
        int port_int = port->text().toInt();
        if(port_int > 65535){
            port->setText("65535");
        }
    }

    void start_client(){
        int thread_count_int = thread_count->value();
        std::string address_str = address->text().toStdString();
        int port_int = port->text().toInt();
        this->close();
        auto conf = new BotnetClient(thread_count_int, address_str, port_int);
        conf->show();
    }

public:
    BotnetClientStarter() : QWidget(){
        setWindowTitle("Client Configuration");
        setFixedWidth(400);
        setFixedHeight(240);
        lay = std::make_unique<QVBoxLayout>(this);
        address_label = std::make_unique<QLabel>("Input Server Address:");
        port_label = std::make_unique<QLabel>("Input Server Port:");
        lay->addWidget(address_label.get());
        address = std::make_unique<QLineEdit>("127.0.0.1");
        port = std::make_unique<QLineEdit>("4000");
        port->setValidator(new QIntValidator(0, 65535));
        connect(port.get(), &QLineEdit::textChanged, this, &BotnetClientStarter::check_port);
        thread_count = std::make_unique<QSlider>(Qt::Horizontal);
        thread_count->setMinimum(1);
        thread_count->setMaximum(std::thread::hardware_concurrency());
        thread_count->setValue(std::thread::hardware_concurrency());
        thread_count->setTickPosition(QSlider::TicksBothSides);
        thread_count->setTickInterval(1);
        lay->addWidget(address.get());
        lay->addWidget(port_label.get());
        lay->addWidget(port.get());
        thread_label = std::make_unique<QLabel>("Input Number of Threads:");
        lay->addWidget(thread_label.get());
        lay->addWidget(thread_count.get());
        start = std::make_unique<QPushButton>("Start Client");
        connect(start.get(), &QPushButton::clicked, this, &BotnetClientStarter::start_client);
        lay->addWidget(start.get());
    }
};

class BotnetStarter : public QWidget{
private:
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
    explicit BotnetStarter(QWidget *parent = nullptr) : QWidget(parent){
        setWindowTitle("Role Configuration");
        setFixedWidth(400);
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