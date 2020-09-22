#ifndef TRACK_H
#define TRACK_H

#include <QString>

class Track
{
public:
    Track(QString name, QString url);
    QString name;
    QString url;
};

#endif // TRACK_H
