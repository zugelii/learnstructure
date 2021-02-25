#ifndef GPSXFORM_H  // defend against multiple includes
#define GPSXFORM_H

class GpsXForm
{
public:
  GpsXForm();
  ~GpsXForm(){};

  void init(double lat, double lon);

  // transforms
  void xy2gps(float x, float y, double &lat, double &lon);
  void gps2xy(double lat, double lon, float &x, float &y);

private:
  double lat_zero;
  double lon_zero;
  double dx_dlon;
  double dy_dlat;
  double dlon_dx;
  double dlat_dy;
};

#endif // defend against multiple includes
