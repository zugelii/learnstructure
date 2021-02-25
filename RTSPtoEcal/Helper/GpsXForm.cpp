#include "GpsXForm.h"
#include <cmath>

#define EARTH_RADIUS        6371315.0 /* [m] */
#define D_PI         (3.14159265358979)

GpsXForm::GpsXForm()
{
  init(0.0, 0.0);
}

void GpsXForm::init(double lat, double lon)
{
  lat_zero = lat;
  lon_zero = lon;
  dx_dlon = EARTH_RADIUS * cos(lat_zero * D_PI  / 180.0) * D_PI / 180.0;
  dy_dlat = EARTH_RADIUS * D_PI / 180.0;
  dlon_dx = 1.0 / dx_dlon;
  dlat_dy = 1.0 / dy_dlat;
}

void GpsXForm::xy2gps(float x, float y, double &lat, double &lon)
{
  lat = lat_zero + ( dlat_dy * y );
  lon = lon_zero + ( dlon_dx * x );
}

void GpsXForm::gps2xy(double lat, double lon, float &x, float &y)
{
  x = (float) ( dx_dlon * (lon - lon_zero));
  y = (float) ( dy_dlat * (lat - lat_zero));
}