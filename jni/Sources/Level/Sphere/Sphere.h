#ifndef SPHERE_H_
#define SPHERE_H_

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Object/2D/Object2D.h>
#include <libeng/Graphic/Object/2D/Dynamic2D.h>

#define TEXCOORD_BUFFER_SIZE    (((32 * 72) + 1) * 2)
#define MAX_SPHERE_SCALE        0.9939f // Avoid to see clear pixels around the crystal (when filming clear elements)

using namespace eng;

//////
class Sphere : public Object2D, public Dynamic2D {

public:
    Sphere(bool texCamFull);
    virtual ~Sphere();

    //
    void start(unsigned char texture);

    //////
    virtual void update(const Game* game, const Level* level) { assert(NULL); }

    inline bool checkCollision(const Bounds* bounds, unsigned char boundsId) { assert(NULL); return false; }
    inline bool checkCollision(const Object* object, unsigned char objectId) { assert(NULL); return false; }

    void render(bool resetUniform) const;

};

#endif // SPHERE_H_
