#ifndef     PROFILE_DATA_H
#define     PROFILE_DATA_H

#include    "profile-element.h"

#include    <vector>

class ProfileData
{
public:

    ProfileData();

    ~ProfileData();

    void addElement(const profile_element_t &profile_element);

    profile_element_t getElement(double railway_coord);

    bool empty() const;

    void clear();

    profile_element_t &at(size_t i);

    size_t size() const;

private:

    std::vector<profile_element_t>  profile_data;
};

#endif // PROFILE_DATA_H
