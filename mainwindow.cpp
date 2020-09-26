#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

void MainWindow::addedTrack() {
    QPushButton* button = (QPushButton *) QObject::sender();

    foreach (Track* track, playlistTracks)
        if (track->name == searchTracks[searchAddButtons->id(button)]->name)
            return;

    playlistTracks.append(searchTracks[searchAddButtons->id(button)]);
    updatePlaylist();
}

void MainWindow::removedTrack() {
    QPushButton* button = (QPushButton *) QObject::sender();

    int id = playlistRemoveButtons->id(button);

    if (isTrackLoaded(playlistTracks[id])) {
        if (searchTracks.size() > 0)
            foreach (QAbstractButton* playButton, searchPlayButtons->buttons()) {
                int idButtonOnSearch = searchPlayButtons->id(playButton);

                if (isTrackLoaded(searchTracks[idButtonOnSearch]))
                    pauseToPlayButtonFromSearch((QPushButton*) playButton);
            }

        player->setMedia(0);
    }

    playlistTracks.remove(playlistRemoveButtons->id(button));

    updatePlaylist();
}

// changes a button from Pause to Play
void MainWindow::pauseToPlayButton(QPushButton* button) {
    button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    disconnect(button, SIGNAL(clicked()), this, SLOT(pausedTrack()));
    connect(button, SIGNAL(clicked()), this, SLOT(playedTrack()));
}

void MainWindow::pauseToPlayButtonFromSearch(QPushButton* button) {
    button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    disconnect(button, SIGNAL(clicked()), this, SLOT(pausedTrackFromSearch()));
    connect(button, SIGNAL(clicked()), this, SLOT(playedTrackFromSearch()));
}

// changes a button from Play to Pause
void MainWindow::playToPauseButton(QPushButton* button) {
    button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    disconnect(button, SIGNAL(clicked()), this, SLOT(playedTrack()));
    connect(button, SIGNAL(clicked()), this, SLOT(pausedTrack()));
}

void MainWindow::playToPauseButtonFromSearch(QPushButton* button) {
    button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    disconnect(button, SIGNAL(clicked()), this, SLOT(playedTrackFromSearch()));
    connect(button, SIGNAL(clicked()), this, SLOT(pausedTrackFromSearch()));
}

void MainWindow::playedTrack() {
    QPushButton* button = (QPushButton *) QObject::sender();
    playToPauseButton(button);

    Track* track = playlistTracks[playlistPlayButtons->id(button)];

    if (player->mediaStatus() == player->NoMedia) {
        player->setMedia(QUrl(track->url));

        if (searchTracks.size() > 0) {
            foreach (QAbstractButton* playButton, searchPlayButtons->buttons()) {
                int id = searchPlayButtons->id(playButton);
                if (searchTracks[id]->name == track->name)
                    playToPauseButtonFromSearch((QPushButton*) playButton);
            }
        }
    } else {
        if (!isTrackLoaded(track)) {
            player->setMedia(QUrl(track->url));

            // reset other icons
            foreach (QAbstractButton* playButton, playlistPlayButtons->buttons())
                if ((QPushButton*) playButton != button)
                    pauseToPlayButton((QPushButton*) playButton);

            if (searchTracks.size() > 0) {
                foreach (QAbstractButton* playButton, searchPlayButtons->buttons()) {
                    int id = searchPlayButtons->id(playButton);

                    if (track->name == searchTracks[id]->name)
                        playToPauseButtonFromSearch((QPushButton*) playButton);
                    else
                        pauseToPlayButtonFromSearch((QPushButton*) playButton);
                }
            }
        } else {
            if (searchTracks.size() > 0) {
                foreach (QAbstractButton* playButton, searchPlayButtons->buttons()) {
                    int id = searchPlayButtons->id(playButton);
                    if (searchTracks[id]->name == track->name)
                        playToPauseButtonFromSearch((QPushButton*) playButton);
                }
            }
        }
    }

    player->play();
}

void MainWindow::playedTrackFromSearch() {
    QPushButton* button = (QPushButton *) QObject::sender();
    playToPauseButtonFromSearch(button);

    Track* track = searchTracks[searchPlayButtons->id(button)];

    if (player->mediaStatus() == player->NoMedia) {
        player->setMedia(QUrl(track->url));

        if (playlistTracks.size() > 0) {
            foreach (QAbstractButton* playButton, playlistPlayButtons->buttons()) {
                int id = playlistPlayButtons->id(playButton);
                if (playlistTracks[id]->name == track->name)
                    playToPauseButton((QPushButton*) playButton);
            }
        }
    } else {
        if (!isTrackLoaded(track)) {
            player->setMedia(QUrl(track->url));

            // reset other icons
            foreach (QAbstractButton* playButton, searchPlayButtons->buttons()) {
                if ((QPushButton*) playButton != button)
                    pauseToPlayButtonFromSearch((QPushButton*) playButton);
            }

            if (playlistTracks.size() > 0) {
                foreach (QAbstractButton* playButton, playlistPlayButtons->buttons()) {
                    int id = playlistPlayButtons->id(playButton);

                    if (track->name == playlistTracks[id]->name)
                        playToPauseButton((QPushButton*) playButton);
                    else
                        pauseToPlayButton((QPushButton*) playButton);
                }
            }
        } else {
            if (playlistTracks.size() > 0) {
                foreach (QAbstractButton* playButton, playlistPlayButtons->buttons()) {
                    int id = playlistPlayButtons->id(playButton);
                    if (playlistTracks[id]->name == track->name)
                        playToPauseButton((QPushButton*) playButton);
                }
            }
        }
    }

    player->play();
}

bool MainWindow::isTrackLoaded(Track* track) {
    return player->mediaStatus() != QMediaPlayer::NoMedia
            && player->mediaStatus() != QMediaPlayer::EndOfMedia
            && player->media() == QMediaContent(QUrl(track->url));
}

void MainWindow::pausedTrack() {
    QPushButton* button = (QPushButton *) QObject::sender();
    player->pause();
    pauseToPlayButton(button);

    Track* track = playlistTracks[playlistPlayButtons->id(button)];

    if (searchTracks.size() > 0) {
        foreach (QAbstractButton* playButton, searchPlayButtons->buttons()) {
            int id = searchPlayButtons->id(playButton);
            if (searchTracks[id]->name == track->name)
                pauseToPlayButtonFromSearch((QPushButton*) playButton);
        }
    }
}

void MainWindow::pausedTrackFromSearch() {
    QPushButton* button = (QPushButton *) QObject::sender();
    player->pause();
    pauseToPlayButtonFromSearch(button);

    Track* track = searchTracks[searchPlayButtons->id(button)];

    if (playlistTracks.size() > 0) {
        foreach (QAbstractButton* playButton, playlistPlayButtons->buttons()) {
            int id = playlistPlayButtons->id(playButton);
            if (playlistTracks[id]->name == track->name)
                pauseToPlayButton((QPushButton*) playButton);
        }
    }
}

void MainWindow::updatePlaylistIfEndOfMedia() {
    if (player->mediaStatus() == player->EndOfMedia) {
        for (int i=0; i<searchTracks.size(); i++)
            if (player->media() == QMediaContent(QUrl(searchTracks[i]->url))) {
                pauseToPlayButtonFromSearch((QPushButton*) searchPlayButtons->button(i));
                break;
            }

        if (ui->autoplay->checkState() != 0)
            for (int i=0; i<playlistTracks.size(); i++)
                if (player->media() == QMediaContent(QUrl(playlistTracks[i]->url))
                    && i != playlistTracks.size() - 1) {
                    player->setMedia(QUrl(playlistTracks[i+1]->url));

                    if (searchTracks.size() > 0)
                        foreach (QAbstractButton* playButton, searchPlayButtons->buttons()) {
                            int idButtonOnSearch = searchPlayButtons->id(playButton);

                            if (searchTracks[idButtonOnSearch]->name ==
                                    playlistTracks[i+1]->name) {
                                playToPauseButtonFromSearch((QPushButton*) playButton);
                                break;
                            }
                        }

                    player->play();
                    break;
                }

        updatePlaylist();
    }
}

void MainWindow::updatePlaylist() {
    ui->playlistTable->setRowCount(playlistTracks.size());
    playlistPlayButtons = new QButtonGroup();
    playlistRemoveButtons = new QButtonGroup();

    for (int i=0; i<playlistTracks.size(); i++) {
        // create track name label
        QLabel *trackName = new QLabel();
        trackName->setText(playlistTracks[i] -> name);

        // create play button
        QPushButton *playButton = new QPushButton();
        playlistPlayButtons->addButton(playButton);
        playlistPlayButtons->setId(playButton, i);

        if (isTrackLoaded(playlistTracks[i])) {
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            connect(playButton, SIGNAL(clicked()), this, SLOT(pausedTrack()));
        } else {
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            connect(playButton, SIGNAL(clicked()), this, SLOT(playedTrack()));
        }

        // create remove button
        QPushButton *removeButton = new QPushButton();
        playlistRemoveButtons->addButton(removeButton);
        playlistRemoveButtons->setId(removeButton, i);
        removeButton->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
        connect(removeButton, SIGNAL(clicked()), this, SLOT(removedTrack()));

        // render track name, play button and remove button
        ui->playlistTable->setCellWidget(i, 0, trackName);
        ui->playlistTable->setCellWidget(i, 1, playButton);
        ui->playlistTable->setCellWidget(i, 2, removeButton);
    }
}

void MainWindow::gotTracks(QNetworkReply *reply) {
    searchTracks.clear();
    searchAddButtons = new QButtonGroup();
    searchPlayButtons = new QButtonGroup();
    int buttonNumber = 0;

    QString strReply = (QString) reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonArray json_array = jsonResponse["tracks"]["items"].toArray();

    int numberOfTracks = 0;

    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();
        if (!json_obj["preview_url"].isNull())
            numberOfTracks++;
    }

    if (numberOfTracks == 0) {
        ui->searchTable->setRowCount(1);
        ui->searchTable->setCellWidget(0, 0, new QLabel("No results found for \""
                                                + ui->searchBar->text() + "\""));
        return;
    }

    ui->searchTable->setRowCount(numberOfTracks);

    int i = 0;
    foreach (QJsonValue js, json_array) {
        QJsonObject json_obj = js.toObject();

        if (json_obj["preview_url"].isNull())
            continue;

        QJsonArray artists = json_obj["artists"].toArray();
        QString listOfArtists = "";
        for (int j=0; j<artists.size(); j++) {
            if (j > 0) listOfArtists += ", ";
            listOfArtists += (artists[j].toObject())["name"].toString();
        }
        QLabel *trackName = new QLabel();
        Track* track = new Track(
                    listOfArtists + " - " + json_obj["name"].toString(),
                    json_obj["preview_url"].toString());
        trackName->setText(track->name);
        searchTracks.push_back(track);
        ui->searchTable->setCellWidget(i, 0, trackName);

        QPushButton *playButton = new QPushButton();

        if (!isTrackLoaded(track)) {
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            connect(playButton, SIGNAL(clicked()), this, SLOT(playedTrackFromSearch()));
        } else {
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            connect(playButton, SIGNAL(clicked()), this, SLOT(pausedTrackFromSearch()));
        }

        searchPlayButtons->addButton(playButton);
        searchPlayButtons->setId(playButton, buttonNumber);
        ui->searchTable->setCellWidget(i, 1, playButton);

        QPushButton *button = new QPushButton("+");
        button->setToolTip("Add track to playlist");
        button->setStyleSheet("font-weight: bold");
//        button->setFixedSize(25,25);
        searchAddButtons->addButton(button);
        searchAddButtons->setId(button, buttonNumber++);
        connect(button, SIGNAL(clicked()), this, SLOT(addedTrack()));
        ui->searchTable->setCellWidget(i++, 2, button);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->saveButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->openButton->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));

    QHeaderView *header = ui->searchTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Interactive);
    header->setSectionResizeMode(2, QHeaderView::Interactive);

    ui->searchTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->searchTable->setColumnWidth(1,40);
    ui->searchTable->setColumnWidth(2,40);

    header = ui->playlistTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Interactive);
    header->setSectionResizeMode(2, QHeaderView::Interactive);

    ui->playlistTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->playlistTable->setColumnWidth(1,40);
    ui->playlistTable->setColumnWidth(2,40);

    player = new QMediaPlayer;
    player->setVolume(50);
    ui->searchTable->verticalHeader()->setVisible(false);
    ui->playlistTable->verticalHeader()->setVisible(false);

    connect(player,
            SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this,
            SLOT(updatePlaylistIfEndOfMedia()));

    ui->volumeIcon->setPixmap(style()->standardIcon(QStyle::SP_MediaVolume).pixmap(20));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchButton_clicked()
{
    bool trackPlayingNotOnPlaylist = true;

    if (playlistTracks.size() > 0)
        foreach (QAbstractButton* playButton, playlistPlayButtons->buttons()) {
            int idButtonOnSearch = playlistPlayButtons->id(playButton);
            if (isTrackLoaded(playlistTracks[idButtonOnSearch]))
                trackPlayingNotOnPlaylist = false;
        }

    if (trackPlayingNotOnPlaylist)
        player->setMedia(0);

    updatePlaylist();

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

    // Display "Loading..."
    ui->searchTable->clearContents();
    ui->searchTable->setRowCount(1);
    ui->searchTable->setCellWidget(0, 0, new QLabel("Loading..."));

    // Loop to wait for the reply
    loop.exec();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    QString token = jsonDocument["access_token"].toString();

    connect(manager,
            &QNetworkAccessManager::finished,
            this,
            &MainWindow::gotTracks);

    request = QNetworkRequest((QUrl("https://api.spotify.com/v1/search?q="
                                    + ui->searchBar->text()
                                    + "&type=track&limit=50")));
    request.setRawHeader("Authorization", "Bearer " + token.toUtf8());
    manager -> get(request);
}

void MainWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Open Spotify Playlist"), "",
            tr("All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this,
                                     tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        QTextStream out(&file);

        foreach (Track* track, playlistTracks)
            out << track->name << "\n" << track->url << "\n";

        file.close();
    }
}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Spotify Playlist"), "",
            tr("All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        return;
    }

    QTextStream in(&file);

    playlistTracks.clear();

    while (!in.atEnd())
        playlistTracks.append(new Track(in.readLine(), in.readLine()));

    updatePlaylist();
}

void MainWindow::on_volume_valueChanged(int value)
{
    player->setVolume(ui->volume->value());
    QIcon icon;
    if (value == 0)
        icon = style()->standardIcon(QStyle::SP_MediaVolumeMuted);
    else
        icon = style()->standardIcon(QStyle::SP_MediaVolume);
    ui->volumeIcon->setPixmap(icon.pixmap(20));
}

void MainWindow::on_searchBar_returnPressed()
{
    on_searchButton_clicked();
}
