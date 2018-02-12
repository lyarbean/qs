#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QPluginLoader>
#include "core/engineabstract.h"
#include "core/gatewayabstract.h"
#include "core/strategyabstract.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    void setupUi();
    void connectSinalSlot();
    void showPluginInfo();
private:
    QLabel* pluginPath;
    QTextEdit *textEdit;
    QPushButton* button;
};

MainWindow::MainWindow()
{
    setupUi();
    connectSinalSlot();
}


void MainWindow::setupUi()
{
    QVBoxLayout* verticalBox = new QVBoxLayout;
    QHBoxLayout* horizontalBox = new QHBoxLayout;
    pluginPath = new QLabel(this);
    button = new QPushButton(this);
    button->setText(tr("Select"));
    horizontalBox->addWidget(button);
    horizontalBox->addWidget(pluginPath);
    verticalBox->addLayout(horizontalBox);
    textEdit = new QTextEdit(this);
    verticalBox->addWidget(textEdit);
    QWidget* c = new QWidget(this);
    c->setLayout(verticalBox);
    setCentralWidget(c);
}

void MainWindow::connectSinalSlot()
{
    connect(button, &QPushButton::released, this, &MainWindow::showPluginInfo);
}

void MainWindow::showPluginInfo()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Plugin"), QDir::homePath(), tr("Plugins (*.so)"));

    QPluginLoader loader(fileName);
    QObject* plugin = loader.instance();
    qCritical() << (quint64)plugin << fileName;
    if (plugin) {
        pluginPath->setText(fileName);
        auto metaData = loader.metaData().value("MetaData").toObject();
        {
            auto t = qobject_cast<Qs::StrategyAbstract*>(plugin);
            if (t) {
                QString info = QString("Name: %1, Build: %2").arg(metaData.value("Key").toString())
                .arg(metaData.value("Build").toString());
                textEdit->setText(info);
                return;
            }
        }
        {
            auto t = qobject_cast<Qs::GatewayAbstract*>(plugin);
            if (t) {
                QString info = QString("Name: %1, Build: %2").arg(metaData.value("Key").toString())
                .arg(metaData.value("Build").toString());
                textEdit->setText(info);
            }
        }
    }
    else {
        qCritical() << loader.errorString();
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "plugininfoviewer.moc"
