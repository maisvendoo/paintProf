#include    "profile.h"

#include    <QFile>
#include    <QTextStream>
#include    <QPainter>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Profile::Profile(int widht, int height) : QWidget()
  , beginCoord(0.0)
  , range(10.0)
  , maxHeight(100.0)
{
    this->resize(widht, height);
    initY = height / 2;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Profile::~Profile()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Profile::clean()
{
    profile_data.clear();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Profile::load(QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly))
        return;

    int id = 1;

    while (!file.atEnd())
    {
        QString line = file.readLine();

        if (line.isEmpty())
            continue;

        QTextStream stream(&line);
        profile_element_t prof_elem;

        stream >> prof_elem.railway_coord >> prof_elem.inclination >> prof_elem.curvature;
        prof_elem.id = id;
        ++id;

        profile_data.push_back(prof_elem);
    }

    for (size_t i = 0; i < profile_data.size() - 1; ++i)
    {
        profile_data[i].length = profile_data[i+1].railway_coord - profile_data[i].railway_coord;
    }

    int zu = 0;
    ++zu;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Profile::setBeginCoord(double coord)
{
    beginCoord = coord;
    this->update();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Profile::paintEvent(QPaintEvent *event)
{
    if (profile_data.empty())
        return;

    initY = this->height() / 2;

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter);
    painter.end();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Profile::paint(QPainter &painter)
{
    painter.setClipRect(QRect(0, 0, this->width(), this->height()));

    painter.fillRect(QRect(0, 0, this->width(), this->height()), Qt::white);

    double sx = static_cast<double>(this->width()) / range;
    double sy = static_cast<double>(this->height() - initY) / maxHeight;

    double x = beginCoord;
    double dx = 0.01;
    double y = 10.0;
    int old_id = 0;
    double y_min = 0;
    int km = static_cast<int>(x);

    int inc_h = this->height() / 4;

    painter.setPen(QPen(Qt::black, 2));

    std::vector<QPoint> km_line;
    std::vector<graph_profile_element_t> g_elems;

    while (x < beginCoord + range)
    {
        if (y < y_min)
            y = y_min;

        if (static_cast<int>(x) == km)
        {
            QPoint p(static_cast<int>(sx * (x - beginCoord)), this->height() - initY - static_cast<int>(sy * y));
            km_line.push_back(p);
            ++km;
        }

        profile_element_t prof_elem = getElement(x);

        if (prof_elem.id != old_id)
        {
            graph_profile_element_t ge;
            ge.beginX = static_cast<int>(sx * (prof_elem.railway_coord - beginCoord));
            ge.endX = static_cast<int>(sx * (prof_elem.railway_coord - beginCoord + prof_elem.length));

            if (prof_elem.inclination > 0)
                ge.dir = 1;
            else
                ge.dir = -1;

            if (qAbs(prof_elem.inclination) < 0.001)
                ge.dir = 0;

            old_id = prof_elem.id;

            g_elems.push_back(ge);
        }

        QPoint p0(static_cast<int>(sx * (x - beginCoord)), this->height() - initY - static_cast<int>(sy * y));

        y += prof_elem.inclination * dx;
        x += dx;

        QPoint p1(static_cast<int>(sx * (x - beginCoord)), this->height() - initY - static_cast<int>(sy * y));

        painter.drawLine(p0, p1);
    }

    int Y_line = this->height() - initY - static_cast<int>(sy * (y_min - 10.0));

    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(0, Y_line, this->width(), Y_line);

    for (auto km : km_line)
    {
        painter.drawLine(km, QPoint(km.x(), Y_line));
    }

    for (auto ge : g_elems)
    {
        QRect rect(ge.beginX, Y_line, ge.endX - ge.beginX, inc_h);
        painter.drawRect(rect);

        switch (ge.dir)
        {
        case 0:
            {

                QLine line(ge.beginX, Y_line + inc_h / 2, ge.endX, Y_line + inc_h / 2);
                painter.drawLine(line);

                break;
            }

        case 1:
            {

                QLine line(ge.beginX, Y_line + inc_h, ge.endX, Y_line);
                painter.drawLine(line);

                break;
            }

        case -1:
            {

                QLine line(ge.beginX, Y_line, ge.endX, Y_line + inc_h);
                painter.drawLine(line);

                break;
            }
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
profile_element_t Profile::getElement(double railway_coord)
{
    if (profile_data.size() == 0)
            return profile_element_t();

        if (railway_coord < (*profile_data.begin()).railway_coord)
            return profile_element_t();

        if (railway_coord >= (*(profile_data.end() - 1)).railway_coord)
            return profile_element_t();

        profile_element_t profile_element;

        size_t left_idx = 0;
        size_t right_idx = profile_data.size() - 1;
        size_t idx = (left_idx + right_idx) / 2;

        while (idx != left_idx)
        {
            profile_element = profile_data.at(idx);

            if (railway_coord <= profile_element.railway_coord)
                right_idx = idx;
            else
                left_idx = idx;

            idx = (left_idx + right_idx) / 2;
        }

        profile_element = profile_data.at(idx);

        return profile_element;
}

