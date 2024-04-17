#include <tbb.h>

using namespace std;
using namespace tbb;

#define G 6.673e-11
#define THETA 0.5

enum Quadrant {
    NW = 0,
    NE = 1,
    SW = 2,
    SE = 3
};

typedef struct Particle {
    long double x;
    long double y;
    long double vx;
    long double vy;
    long double mass;
    string      name;
    long double fx;
    long double fy;

    Particle(long double _x, long double _y, long double _vx, long double _vy, long double _mass, string _name) {
        x = _x;
        y = _y;
        vx = _vx;
        vy = _vy;
        mass = _mass;
        name = _name;
        fx = 0;
        fy = 0;
    }
} Particle;

typedef struct Square {
    int              id;
    int              parentId;
    int              level;
    Quadrant         quadrant;
    long double      xMiddle;
    long double      yMiddle;
    long double      length;
    vector<Particle> particles;
    vector<Square>   children;

    Square(int _id, int _parentId, int _level, Quadrant _quadrant, long double _xMiddle, long double _yMiddle, long double _length) {
        id = _id;
        parentId = _parentId;
        level = _level;
        quadrant = _quadrant;
        xMiddle = _xMiddle;
        yMiddle = _yMiddle;
        length = _length;
    }
} Square;

vector<Particle> particles;
vector<Square>   squares;

