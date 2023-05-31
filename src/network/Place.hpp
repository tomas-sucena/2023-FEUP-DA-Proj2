#ifndef DA_PROJ2_PLACE_HPP
#define DA_PROJ2_PLACE_HPP

#include "Vertex.hpp"

class Place : public Vertex {
    private:
        double latitude;
        double longitude;

    public:
        /**
         * @brief creates a new Place
         * @param latitude latitude of the Place
         * @param longitude longitude of the Place
        */
        Place(double latitude, double longitude) : latitude(latitude), longitude(longitude) {}

        /**
         * @brief creates a copy of the Station
         * @return pointer to a copy of the Station
         */
        Vertex* clone() const override{
            return new Place(*this);
        }

        /**
         * @brief getter for Place latitude
        */
        double getLatitude() const{
            return this->latitude;
        }

        /**
         * @brief getter for Place longitude
        */
        double getLongitude() const{
            return this->longitude;
        }
};


#endif //DA_PROJ2_PLACE_HPP

