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
         * @brief creates a copy of the Place
         * @return pointer to a copy of the Place
         */
        Vertex* clone() const override{
            return new Place(*this);
        }

        /**
         * @brief returns the latitude of the Place
         * @return latitude of the Place
        */
        double getLatitude() const{
            return latitude;
        }

        /**
         * @brief returns the longitude of the Place
         * @return longitude of the Place
        */
        double getLongitude() const{
            return longitude;
        }
};


#endif //DA_PROJ2_PLACE_HPP

