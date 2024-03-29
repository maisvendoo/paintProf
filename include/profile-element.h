#ifndef     PROFILE_ELEMENT_H
#define     PROFILE_ELEMENT_H

struct profile_element_t
{
    int     id;
    double  railway_coord;
    double  length;
    double  inclination;
    double  curvature;

    profile_element_t()    
        : id(1)
        , railway_coord(0.0)
        , length(1.0)
        , inclination(0.0)
        , curvature(0.0)
    {

    }
};

#endif // PROFILE_ELEMENT_H
