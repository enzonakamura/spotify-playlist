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
    void pauseToPlayButton(QPushButton* button);
    void playToPauseButton(QPushButton* button);
    bool isTrackLoaded(Track* track);

private slots:
    void gotTracks(QNetworkReply*);
    void on_searchButton_clicked();
    void addedTrack();
    void playedTrack();
    void pausedTrack();
    void removedTrack();
    void on_openButton_clicked();
    void on_saveButton_clicked();
    void updatePlaylistIfEndOfMedia();
    void on_volume_valueChanged(int value);
    void on_searchBar_returnPressed();
};
#endif // MAINWINDOW_H
