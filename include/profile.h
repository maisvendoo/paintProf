#ifndef     PROFILE_H
#define     PROFILE_H

#include    <QWidget>

#include    "profile-element.h"

class QPainter;

struct graph_profile_element_t
{
    int beginX;
    int endX;
    int dir;

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

private:

    double  beginCoord;
    double  range;
    double  maxHeight;
    int     initY;

    std::vector<profile_element_t> profile_data;

    void paintEvent(QPaintEvent *event) override;

    void paint(QPainter &painter);

    profile_element_t getElement(double railway_coord);
};

#endif // PROFILE_H
