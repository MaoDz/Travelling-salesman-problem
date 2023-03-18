#include <math.h>
#include "haversine.h"

double toRad(double angleD){
	return (angleD * M_PI / 180.0);
}

double haversin(double val){
	return pow(sin(val / 2), 2);
}

double calculDistance(double nLat1, double nLon1, double nLat2, double nLon2){
	double dLat  = toRad(nLat2 - nLat1);
	double dLong = toRad(nLon2 - nLon1);
	double startLat = toRad(nLat1);
	double endLat = toRad(nLat2);
	double a = haversin(dLat) + cos(startLat) * cos(endLat) * haversin(dLong);
	double c = 2 * atan2(sqrt(a),sqrt(1 - a));
	return RAYON_TERRE * c; 
}

