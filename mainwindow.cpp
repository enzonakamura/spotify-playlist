#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QLabel>
#include <QDebug>

void MainWindow::addedTrack() {
    QPushButton* button = (QPushButton *) QObject::sender();
    playlistTracks.append(searchTracks[searchAddButtons->id(button)]);
    updatePlaylist();
}

void MainWindow::removedTrack() {
    QPushButton* button = (QPushButton *) QObject::sender();
    playlistTracks.remove(playlistPlayButtons->id(button));
    updatePlaylist();
}

void MainWindow::playedTrack() {
    QPushButton* button = (QPushButton *) QObject::sender();
    Track* track = playlistTracks[playlistPlayButtons->id(button)];
    player->setMedia(QUrl(track->url));
    player->play();
}

void MainWindow::updatePlaylist() {
    ui->tableWidget_2->setRowCount(playlistTracks.size());
    playlistPlayButtons = new QButtonGroup();
    playlistRemoveButtons = new QButtonGroup();

    for (int i=0; i<playlistTracks.size(); i++) {
        // create track name label
        QLabel *trackName = new QLabel();
        trackName->setText(playlistTracks[i] -> name);

        // create play button
        QPushButton *playButton = new QPushButton("Play");
        playlistPlayButtons->addButton(playButton);
        playlistPlayButtons->setId(playButton, i);
        connect(playButton, SIGNAL(clicked()), this, SLOT(playedTrack()));

        // create remove button
        QPushButton *removeButton = new QPushButton("-");
        playlistPlayButtons->addButton(removeButton);
        playlistPlayButtons->setId(removeButton, i);
        connect(removeButton, SIGNAL(clicked()), this, SLOT(removedTrack()));

        // render track name, play button and remove button
        ui->tableWidget_2->setCellWidget(i, 0, trackName);
        ui->tableWidget_2->setCellWidget(i, 1, playButton);
        ui->tableWidget_2->setCellWidget(i, 2, removeButton);
    }
}

void MainWindow::gotTracks(QNetworkReply *reply) {
    searchTracks.clear();
    searchAddButtons = new QButtonGroup();
    int buttonNumber = 0;

    QString strReply = (QString) reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonArray json_array = jsonResponse["tracks"]["items"].toArray();

    int size = 0;

    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();
        if (!json_obj["preview_url"].isNull())
            size++;
    }

    ui->tableWidget->setRowCount(size);

    int i = 0;
    foreach (const QJsonValue value, json_array) {
        QJsonObject json_obj = value.toObject();

        if (json_obj["preview_url"].isNull())
            continue;

        QJsonArray artists = json_obj["artists"].toArray();
        QString listOfArtists = "";
        for (int i=0; i<artists.size(); i++) {
            if (i > 0) listOfArtists += ", ";
            listOfArtists += (artists[i].toObject())["name"].toString();
        }
        QLabel *nom = new QLabel();
        Track* track = new Track(listOfArtists + " - " + json_obj["name"].toString(),
                json_obj["preview_url"].toString());
        nom->setText(track->name);
        searchTracks.push_back(track);
        ui->tableWidget->setCellWidget(i, 0, nom);

        QPushButton *button = new QPushButton("+");
        button->setFixedSize(20,20);
        searchAddButtons->addButton(button);
        searchAddButtons->setId(button, buttonNumber++);
        connect(button, SIGNAL(clicked()), this, SLOT(addedTrack()));
        ui->tableWidget->setCellWidget(i++, 1, button);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHeaderView *header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Interactive);

    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setColumnWidth(1,40);

    header = ui->tableWidget_2->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Interactive);
    header->setSectionResizeMode(2, QHeaderView::Interactive);

    ui->tableWidget_2->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_2->setColumnWidth(1,40);
    ui->tableWidget_2->setColumnWidth(2,40);

    player = new QMediaPlayer;
    player->setVolume(50);
//    ui->tableWidget->verticalHeader()->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QEventLoop loop;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkRequest request((QUrl("https://accounts.spotify.com/api/token")));
    request.setRawHeader("Authorization",
    "Basic NzA4MmI3NzA5OWUzNDE0MmE0ZjAxNmRjYjc5MDRkMzU6MDQxNTI2NGNlMjMzNDMzY2IxODU5ODRiMGRjMzExMzg=");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    QUrlQuery payload;
    payload.addQueryItem("grant_type","client_credentials");
    QByteArray postData = payload.query(QUrl::FullyEncoded).toUtf8();
    QNetworkReply *reply = manager->post(request, postData);

    loop.exec();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    QString token = jsonDocument["access_token"].toString();

    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::gotTracks);

    QString query = ui -> textEdit_2 -> toPlainText();
    request = QNetworkRequest((QUrl("https://api.spotify.com/v1/search?q=" + query + "&type=track")));
    request.setRawHeader("Authorization", "Bearer " + token.toUtf8());
    manager -> get(request);

    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl("https://p.scdn.co/mp3-preview/f6437601ecdbf4f31b75875f0bc5184f4330b10a?cid=7082b77099e34142a4f016dcb7904d35"));
    player->setVolume(50);
//    player->play();
}