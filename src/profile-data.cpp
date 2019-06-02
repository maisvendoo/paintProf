#include    "profile-data.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ProfileData::ProfileData()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ProfileData::~ProfileData()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ProfileData::addElement(const profile_element_t &profile_element)
{
    profile_data.push_back(profile_element);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
profile_element_t ProfileData::getElement(double railway_coord)
{
    if (profile_data.size() == 0)
            return profile_element_t();

        if (railway_coord < (*profile_data.begin()).railway_coord)
            return profile_element_t();

        if (railway_coord >= (*(profile_data.end() - 1)).railway_coord)
            return *(profile_data.end() - 1);

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

bool ProfileData::empty() const
{
    return profile_data.empty();
}

void ProfileData::clear()
{
    profile_data.clear();
}

profile_element_t &ProfileData::at(size_t i)
{
    return profile_data.at(i);
}

size_t ProfileData::size() const
{
    return profile_data.size();
}
