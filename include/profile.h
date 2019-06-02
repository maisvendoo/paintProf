#ifndef     PROFILE_H
#define     PROFILE_H

#include    <QWidget>

#include    "profile-data.h"

class QPainter;

struct graph_profile_element_t
{
    int beginX;
    int endX;
    int dir;
    double inc;
    double length;

    graph_profile_element_t()
        : beginX(0)
        , endX(0)
        , dir(0)
    {

    }
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class Profile : public QWidget
{
    Q_OBJECT

public:

    Profile(int width = 200, int height = 100);

    ~Profile() override;

    void clean();

public slots:

    void load(QString path);

    void setBeginCoord(double coord);

    void setInitKm(int init_km);

    void save(QString path);

private:

    double      beginCoord;
    double      range;
    double      maxHeight;
    int         initY;
    int         init_km;

    ProfileData profile_data;

    ProfileData sprofile_data;

    void paintEvent(QPaintEvent *event) override;

    void paint(QPainter &painter);

    profile_element_t getElement(double railway_coord);

    void create_sprofile(ProfileData &profile, ProfileData &sprofile);

    void saveSVG(QString path);
};

#endif // PROFILE_H
