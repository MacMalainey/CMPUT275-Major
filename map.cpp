#include "map.h"
#include "assets.h"

#define DEBUG_DRAW true

Path** Map::paths() {return _paths;};

void Map::draw(MCUFRIEND_kbv canvas) {
    int color = genNeonColor();
    #if(DEBUG_DRAW)
        for (uint8_t i = 0; i < _n; i++) {
            Path* p = _paths[i];
            Junction s = p->juncts[0];
            Junction e -> p->juncts[p->numJuncts - 1];
            canvas.drawLine(s.x, s.y, e.x, e.y, color);
        }
    #endif
}

void Map::Map(Path** paths, uint8_t n) {
    _paths = new Path*[n];
    _n = n;
    for (uint8_t i = 0; i < n; i++) {
        _paths[n] = _paths;
    }
}
