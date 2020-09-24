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
#include <QButtonGroup>
#include "track.h"

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
    QVector<Track*> searchTracks;
    QButtonGroup* searchAddButtons;

    QVector<Track*> playlistTracks;
    QButtonGroup* playlistPlayButtons;
    QButtonGroup* playlistRemoveButtons;

    QMediaPlayer *player;
    void updatePlaylist();

private slots:
    void gotTracks(QNetworkReply*);
    void on_pushButton_clicked();
    void addedTrack();
    void playedTrack();
    void removedTrack();
    void on_loadButton_clicked();
    void on_saveButton_clicked();
};
#endif // MAINWINDOW_H
