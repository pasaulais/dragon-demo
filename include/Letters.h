#ifndef INITIALS_LETTERS_H
#define INITIALS_LETTERS_H

class Mesh;
class DragonScene;

class Letters
{
public:
    static bool meshLoaded();
    static bool initMeshes();
    static void drawP(DragonScene *s);
    static void drawA(DragonScene *s);
    static void drawS(DragonScene *s);
    static void drawArcS(DragonScene *s);
    static void drawCube(DragonScene *s);

    static Mesh *arc_c;
    static Mesh *arc_s;
    static Mesh *disk;
    static Mesh *cube;
    static Mesh *sheared_a;
};

#endif
