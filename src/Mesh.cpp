#include <GL/gl.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include "Mesh.h"
#include "Material.h"
#include "RenderState.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

/* Show the normal for every vertex in the mesh, for debugging purposes. */
void Mesh::drawNormals(RenderState *s)
{
    (void)s;
}

VertexGroup * Mesh::loadStl(string path, bool computeNormals)
{
    VertexGroup *vg = 0;
    FILE *f = fopen(path.c_str(), "rb");
    char header[80];
    uint32_t triangles = 0;
    vec3 points[4];
    uint16_t attributes;

    // read file header
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s'.\n", path.c_str());
        return 0;
    }
    if((fread(header, sizeof(header), 1, f) < 1) ||
            (fread(&triangles, 4, 1, f) < 1))
    {
        fclose(f);
        return 0;
    }

    // read vertex data
    vg = new VertexGroup(GL_TRIANGLES, 3 * triangles);
    VertexData *v = vg->data;
    for(uint32_t i = 0; i < triangles; i++)
    {
        if((fread(points, sizeof(points), 1, f) < 1) || (fread(&attributes, 2, 1, f) < 1))
        {
            delete vg;
            fclose(f);
            return 0;
        }
        if(computeNormals)
            points[0] = vec3::normal(points[1], points[2], points[3]);
        for(uint32_t j = 0; j < 3; j++, v++)
        {
            v->position = points[j + 1];
            v->normal = points[0];
            v->texCoords = vec2();
        }
    }
    fclose(f);
    return vg;
}

typedef struct
{
    uint32_t vertexIndex;
    uint32_t normalIndex;
    uint32_t texCoordsIndex;
} ObjPoint;

static bool parseObjPoint(char *data, ObjPoint *p)
{
    int vertexIndex = 0;
    int normalIndex = 0;
    int texCoordsIndex = 0;
    if(sscanf(data, "%d/%d/%d", &vertexIndex, &normalIndex, &texCoordsIndex) == 3)
    {
        if(p)
        {
            p->vertexIndex = vertexIndex;
            p->normalIndex = normalIndex;
            p->texCoordsIndex = texCoordsIndex;
        }
        return true;
    }
    else if(sscanf(data, "%d//%d", &vertexIndex, &normalIndex) == 2)
    {
        if(p)
        {
            p->vertexIndex = vertexIndex;
            p->normalIndex = normalIndex;
            p->texCoordsIndex = vertexIndex;
        }
        return true;
    }
    else if(sscanf(data, "%d/%d", &vertexIndex, &texCoordsIndex) == 2)
    {
        if(p)
        {
            p->vertexIndex = vertexIndex;
            p->normalIndex = vertexIndex;
            p->texCoordsIndex = texCoordsIndex;
        }
        return true;
    }
    else if(sscanf(data, "%d", &vertexIndex) == 1)
    {
        if(p)
        {
            p->vertexIndex = vertexIndex;
            p->normalIndex = vertexIndex;
            p->texCoordsIndex = vertexIndex;
        }
        return true;
    }
    return false;
}

VertexGroup * Mesh::loadObj(string path)
{
    FILE *f = fopen(path.c_str(), "r");
    char line[512];
    char point1[32];
    char point2[32];
    char point3[32];
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s'.\n", path.c_str());
        return 0;
    }

    // read the file once to count the number of vertices, normals, texture coords
    uint32_t fileVertexCount = 0;
    uint32_t fileNormalCount = 0;
    uint32_t fileTexCoordsCount = 0;
    uint32_t meshVertexCount = 0;
    while(fgets(line, sizeof(line), f) != 0)
    {
        float v1, v2, v3;
        if(sscanf(line, "v %f %f %f", &v1, &v2, &v3) == 3)
            fileVertexCount++;
        else if(sscanf(line, "vn %f %f %f", &v1, &v2, &v3) == 3)
            fileNormalCount++;
        else if(sscanf(line, "vt %f %f", &v1, &v2) == 2)
            fileTexCoordsCount++;
        else if(sscanf(line, "f %s %s %s", point1, point2, point3) == 3)
            meshVertexCount += 3;
    }
    fseek(f, 0, SEEK_SET);

    // actually read the vertex, ..., data
    ObjPoint points[4];
    vec3 normal;
    bool computeNormals = false;
    vec3 *vertices = new vec3[fileVertexCount];
    vec3 *normals = new vec3[fileNormalCount];
    vec2 *texCoords = new vec2[fileTexCoordsCount];
    vec3 *pv = vertices;
    vec3 *pn = normals;
    vec2 *pt = texCoords;
    VertexGroup *vg = new VertexGroup(GL_TRIANGLES, meshVertexCount);
    VertexData *pd = vg->data;
    while(fgets(line, sizeof(line), f) != 0)
    {
        float v1, v2, v3;
        if(sscanf(line, "v %f %f %f", &v1, &v2, &v3) == 3)
            *pv++ = vec3(v1, v2, v3);
        else if(sscanf(line, "vn %f %f %f", &v1, &v2, &v3) == 3)
            *pn++ = vec3(v1, v2, v3);
        else if(sscanf(line, "vt %f %f", &v1, &v2) == 2)
            *pt++ = vec2(v1, v2);
        else if(sscanf(line, "f %s %s %s", point1, point2, point3) == 3)
        {
            if(parseObjPoint(point1, &points[0]) &&
                parseObjPoint(point2, &points[1]) &&
                parseObjPoint(point3, &points[2]))
            {
                computeNormals = (fileNormalCount == 0);
                if(computeNormals)
                {
                    vec3 v1 = vertices[points[0].vertexIndex - 1];
                    vec3 v2 = vertices[points[1].vertexIndex - 1];
                    vec3 v3 = vertices[points[2].vertexIndex - 1];
                    normal = vec3::normal(v1, v2, v3);
                }
                for(int i = 0; i < 3; i++, pd++)
                {
                    pd->position = vertices[points[i].vertexIndex - 1];
                    if(computeNormals)
                        pd->normal = normal;
                    else
                        pd->normal = normals[points[i].normalIndex - 1];
                    pd->texCoords = texCoords[points[i].texCoordsIndex - 1];
                }
            }
        }
    }
    delete [] vertices;
    delete [] normals;
    delete [] texCoords;
    fclose(f);
    return vg;
}

void Mesh::saveStl(string path) const
{
    int groups = groupCount();
    VertexGroup **vg = (VertexGroup **)new VertexGroup*[groups];
    for(int i = 0; i < groups; i++)
    {
        uint32_t mode = groupMode(i);
        uint32_t count = groupSize(i);
        vg[i] = new VertexGroup(mode, count);
        copyGroupTo(i, vg[i]);
    }
    saveStl(path, vg, groups);
    for(int i = 0; i < groups; i++)
    {
        delete vg[i];
    }
    delete [] vg;
}

void Mesh::saveObj(string path) const
{
    int groups = groupCount();
    VertexGroup **vg = (VertexGroup **)new VertexGroup*[groups];
    for(int i = 0; i < groups; i++)
    {
        uint32_t mode = groupMode(i);
        uint32_t count = groupSize(i);
        vg[i] = new VertexGroup(mode, count);
        copyGroupTo(i, vg[i]);
    }
    saveObj(path, vg, groups);
    for(int i = 0; i < groups; i++)
    {
        delete vg[i];
    }
    delete [] vg;
}

void Mesh::saveStl(string path, VertexGroup **vg, int groups)
{
    if(!vg)
        return;
    FILE *f = fopen(path.c_str(), "wb");
    char header[80];
    uint32_t triangles = 0;
    uint16_t attributes = 0;

    // count triangles
    for(int i = 0; i < groups; i++)
    {
        VertexGroup *g = vg[i];
        if(g->mode == GL_TRIANGLES)
            triangles += (g->count / 3);
    }

    // write file header
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s' for writing.\n", path.c_str());
        return;
    }
    memset(header, 0, sizeof(header));
    fwrite(header, sizeof(header), 1, f);
    fwrite(&triangles, sizeof(uint32_t), 1, f);

    // write vertex data
    for(int i = 0; i < groups; i++)
    {
        VertexGroup *g = vg[i];
        if(g->mode != GL_TRIANGLES)
            continue;
        VertexData *v = g->data;
        for(uint32_t j = 0; (g->count - j) >= 3; j += 3)
        {
            fwrite(&v->normal, sizeof(vec3), 1, f);
            for(uint32_t k = 0; j < 3; k++)
                fwrite(&((v + k)->position), sizeof(vec3), 1, f);
            fwrite(&attributes, sizeof(attributes), 1, f);
        }
    }
    fclose(f);
}

void Mesh::saveObj(string path, VertexGroup **vg, int groups)
{
    if(!vg)
        return;
    FILE *f = fopen(path.c_str(), "w");
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s' for writing.\n", path.c_str());
        return;
    }
    for(int i = 0; i < groups; i++)
    {
        VertexGroup *g = vg[i];
        for(uint32_t j = 0; j < g->count; j++)
        {
            vec3 vertex = g->data[j].position;
            fprintf(f, "v %f %f %f\n", vertex.x, vertex.y, vertex.z);
        }
    }
    for(int i = 0; i < groups; i++)
    {
        VertexGroup *g = vg[i];
        for(uint32_t j = 0; j < g->count; j++)
        {
            vec2 texCoords = g->data[j].texCoords;
            fprintf(f, "vt %f %f\n", texCoords.x, texCoords.y);
        }
    }
    for(int i = 0; i < groups; i++)
    {
        VertexGroup *g = vg[i];
        for(uint32_t j = 0; j < g->count; j++)
        {
            vec3 normal = g->data[j].normal;
            fprintf(f, "vn %f %f %f\n", normal.x, normal.y, normal.z);
        }
    }
    uint32_t offset = 0;
    for(int i = 0; i < groups; i++)
    {
        VertexGroup *g = vg[i];
        switch(g->mode)
        {
        case GL_TRIANGLES:
            saveObjIndicesTri(f, g, offset);
            break;
        case GL_QUADS:
            saveObjIndicesQuad(f, g, offset);
            break;
        case GL_TRIANGLE_STRIP:
            saveObjIndicesTriStrip(f, g, offset);
            break;
        }
    }
    fclose(f);
}

void Mesh::saveObjIndicesTri(FILE *f, VertexGroup *vg, uint32_t &offset)
{
    uint32_t endOffset = offset + vg->count - 2;
    for(uint32_t i = offset; i < endOffset; i += 3)
        saveObjFace(f, i, i + 1, i + 2);
    offset += vg->count;
}

void Mesh::saveObjIndicesQuad(FILE *f, VertexGroup *vg, uint32_t &offset)
{
    uint32_t endOffset = offset + vg->count - 3;
    for(uint32_t i = offset; i < endOffset; i += 4)
    {
        saveObjFace(f, i, i + 1, i + 2);
        saveObjFace(f, i, i + 2, i + 3);
    }
    offset += (vg->count * 6) / 4;
}

void Mesh::saveObjIndicesTriStrip(FILE *f, VertexGroup *vg, uint32_t &offset)
{
    uint32_t endOffset = offset + vg->count - 2;
    for(uint32_t i = offset; i < endOffset; i++)
        saveObjFace(f, i, i + 1, i + 2);
    offset += (vg->count - 2) * 3;
}

void Mesh::saveObjFace(FILE *f, uint32_t ind1, uint32_t ind2, uint32_t ind3)
{
    fprintf(f, "f ");
    saveObjIndice(f, ind1 + 1);
    fprintf(f, " ");
    saveObjIndice(f, ind2 + 1);
    fprintf(f, " ");
    saveObjIndice(f, ind3 + 1);
    fprintf(f, "\n");
}

void Mesh::saveObjIndice(FILE *f, uint32_t ind)
{
    //if(texCoords && normals)
        fprintf(f, "%d/%d/%d", ind, ind, ind);
    //else if(normals)
    //    fprintf(f, "%d//%d", ind, ind);
    //else if(texCoords)
    //    fprintf(f, "%d/%d", ind, ind);
    //else
    //    fprintf(f, "%d", ind);
}
