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

        profile_data.addElement(prof_elem);
    }

    for (size_t i = 0; i < profile_data.size() - 1; ++i)
    {
        profile_data.at(i).length = profile_data.at(i+1).railway_coord - profile_data.at(i).railway_coord;
    }

    create_sprofile(profile_data, sprofile_data);

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

void Profile::setInitKm(int init_km)
{
    this->init_km = init_km;
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

    int inc_h = this->height() / 16;

    painter.setPen(QPen(Qt::black, 3));

    std::vector<QPoint> km_line;
    std::vector<graph_profile_element_t> g_elems;

    double end_coord = beginCoord + range;
    auto last = profile_data.at(profile_data.size() - 1);

    if (end_coord > last.railway_coord + last.length)
        end_coord = last.railway_coord + last.length;

    while (x <= end_coord)
    {
        if (y < y_min)
            y = y_min;

        if (static_cast<int>(x) == km)
        {
            QPoint p(static_cast<int>(sx * (x - beginCoord)), this->height() - initY - static_cast<int>(sy * y));
            km_line.push_back(p);
            ++km;
        }

        profile_element_t prof_elem = profile_data.getElement(x);
        profile_element_t sprof_elem = sprofile_data.getElement(x);

        if (sprof_elem.id != old_id)
        {
            graph_profile_element_t ge;
            ge.beginX = static_cast<int>(sx * (sprof_elem.railway_coord - beginCoord));
            ge.endX = static_cast<int>(sx * (sprof_elem.railway_coord - beginCoord + sprof_elem.length));

            if (sprof_elem.inclination > 0)
                ge.dir = 1;
            else
                ge.dir = -1;

            if (qAbs(sprof_elem.inclination) < 0.001)
                ge.dir = 0;

            ge.inc = sprof_elem.inclination;
            ge.length = sprof_elem.length;

            old_id = sprof_elem.id;

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

    //
    for (auto it = km_line.begin() + 1; it != km_line.end(); ++it)
    {
        auto km = *it;
        painter.drawLine(km, QPoint(km.x(), Y_line));
    }

    //
    for (auto ge : g_elems)
    {
        QRect rect(ge.beginX, Y_line, ge.endX - ge.beginX, inc_h);
        painter.drawRect(rect);

        switch (ge.dir)
        {
        case 0:
            {

                QLine line(ge.beginX, Y_line + inc_h, ge.endX, Y_line + inc_h);
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

    //
    for (auto it = km_line.begin() + 1; it != km_line.end(); ++it)
    {
        QLine line((*it).x(), Y_line + inc_h, (*it).x(), Y_line + 2 * inc_h);
        painter.drawLine(line);
    }

    painter.drawLine(0, Y_line + 2 * inc_h, this->width(), Y_line + 2 * inc_h);

    QFont font = painter.font();
    int base_size = inc_h / 4;
    font.setPixelSize(base_size);
    painter.setFont(font);

    //
    for (auto ge : g_elems)
    {
        if (ge.length * 1000.0 < 250.0)
            continue;

        QRect rect_i;
        QRect rect_l;

        int w = ge.endX - ge.beginX;
        int X = ge.beginX;
        int Y = Y_line;

        if (ge.inc >= 0.0)
        {
            rect_i = QRect(X, Y, w / 2, inc_h / 2);
            rect_l = QRect(X + w / 2, Y + inc_h / 2, w / 2, inc_h / 2);
        }
        else
        {
            rect_l = QRect(X, Y + inc_h / 2, w / 2, inc_h / 2);
            rect_i = QRect(X + w / 2, Y, w / 2, inc_h / 2);
        }

        painter.drawText(rect_i, Qt::AlignCenter, QString("%1").arg(ge.inc, 4, 'f', 1));
        painter.drawText(rect_l, Qt::AlignCenter, QString("%1").arg(qRound(ge.length * 1000)));
    }

    font.setPixelSize(2 * base_size);
    painter.setFont(font);

    //
    int w = km_line[1].x() - km_line[0].x();

    for (size_t i = 0; i < km_line.size(); ++i)
    {
        int X = km_line[i].x();
        int Y = Y_line + inc_h;

        int h = inc_h;

        painter.drawText(QRect(X, Y, w, h), Qt::AlignCenter, QString("%1").arg(init_km + i + static_cast<int>(beginCoord)));
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Profile::create_sprofile(ProfileData &profile, ProfileData &sprofile)
{
    double sum = 0;
    double Sc = 0;
    std::vector<profile_element_t> tmp_data;
    int id = 1;

    for (size_t i = 0; i < profile.size(); ++i)
    {
        profile_element_t p_elem = profile.at(i);

        sum += p_elem.inclination * p_elem.length;
        Sc += p_elem.length;

        double ic = sum / Sc;

        tmp_data.push_back(p_elem);

        for (auto elem : tmp_data)
        {
            if ( (elem.length > 1.0 / (abs(ic - elem.inclination))) ||
                 (elem.inclination * ic < 0) )
            {
                i--;

                auto last = *(tmp_data.end() - 1);
                sum -= last.inclination * last.length;
                Sc -= last.length;

                tmp_data.erase(tmp_data.end() - 1);

                profile_element_t s_elem;
                s_elem.railway_coord = (*tmp_data.begin()).railway_coord;
                s_elem.length = Sc;
                s_elem.inclination = sum / Sc;
                s_elem.id = id;
                ++id;

                sprofile.addElement(s_elem);

                sum = 0;
                Sc = 0;
                tmp_data.clear();

                break;
            }
        }

        if (!tmp_data.empty() && (i == profile_data.size() - 1))
        {
            profile_element_t s_elem;
            s_elem.railway_coord = (*tmp_data.begin()).railway_coord;
            s_elem.length = Sc;
            s_elem.inclination = sum / Sc;
            s_elem.id = id;
            ++id;

            sprofile.addElement(s_elem);
        }
    }
}

