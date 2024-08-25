// utils.hpp

#define MIN(_x, _y) ((_x) < (_y) ? (_x) : (_y))
#define MAX(_x, _y) ((_x) > (_y) ? (_x) : (_y))

#define MININDEX2(_x, _y) ((_x) < (_y) ? 0 : 1)
#define MAXINDEX2(_x, _y) ((_x) > (_y) ? 0 : 1)

#define MININDEX3(_x, _y, _z) ((_x) < (_y) ? ((_x) < (_z) ? 0 : 2) : ((_y) < (_z) ? 1 : 2))
#define MAXINDEX3(_x, _y, _z) ((_x) > (_y) ? ((_x) > (_z) ? 0 : 2) : ((_y) > (_z) ? 1 : 2))

#define PI 3.141592653589793

#define TODEGREE(_theta) ((_theta) * 180 / PI)
#define TORADIAN(_theta) ((_theta) * PI / 180)

#define ABS(_a) ((_a) < 0 ? (-_a) : (_a))

#define CLAMP(_t, _a, _b) (MIN(MAX(_t, _a), _b))

#define LERP(_t, _a, _b) ((1 - (_t)) * (_a) + (_t) * (_b))

#define EQUAL2(_a, _b, _n) (ABS((_a) - (_b)) < pow(10, -(_n)))
#define EQUAL3(_a, _b, _c, _n) (EQUAL2(_a, _b, _n) && EQUAL2(_a, _c, _n) && EQUAL2(_b, _c, _n))
#define EQUALTOZERO(_a, _n) (ABS(_a) < pow(10, -(_n)))
