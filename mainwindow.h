#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtMultimedia/QMediaPlayer>
#include <QStringListModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtCore>
#include <QtGui>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QString> searchTracks;
    QVector<QString> playlistTracks;
private slots:
    void gotTracks(QNetworkReply*);
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
