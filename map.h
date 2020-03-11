#include <MCUFRIEND_kbv.h>

typedef char Pellets; // Typedef to make compiler happy until code is made
typedef char Player;

class Path; // Forward declare for the paths class

class Map {
    private:
    Path** _paths;
    uint8_t _n;

    public:
    Path** paths();
    void draw(MCUFRIEND_kbv canvas);

    Map(Path** paths, uint16_t n);
};

class Junction {
    private:
    int _x;
    int _y;

    Path** _paths;
    uint8_t _n;

    public:
    uint8_t numPaths();
    Path** paths();

    int x();
    int y();
};

// Unlike normal graphs where "paths" aren't usually actual objects, we use the paths as containers for
// other objects as well as storing any junctions that lie on the path
class Path {
    private:
    Pellets** _pellets;
    Player** _players;

    Junction** _junctions; // Ordered left to right or top to bottom
    uint8_t _n;

    public:
    Junction** juncts();
    uint8_t numJuncts();

    // TODO: add code to get access to players and pellets
};
