#include <GL/gl.h>
#include <cmath>
#include <cstdio>
#include "MeshGL1.h"
#include "Material.h"
#include "RenderState.h"

MeshGL1::MeshGL1(QObject *parent) : Mesh(parent)
{
}

MeshGL1::~MeshGL1()
{
}

QVector<vec3> & MeshGL1::vertices()
{
    return m_vertices;
}

const QVector<vec3> & MeshGL1::vertices() const
{
    return m_vertices;
}

void MeshGL1::setVertices(GLfloat *vertices, int n)
{
    if(!vertices)
        return;
    m_vertices.resize(n);
    for(int i = 0; i < n; i++, vertices += 3)
        m_vertices[i] = vec3(vertices[0], vertices[1], vertices[2]);
}

QVector<vec3> & MeshGL1::normals()
{
    return m_normals;
}

const QVector<vec3> & MeshGL1::normals() const
{
    return m_normals;
}

void MeshGL1::setNormals(float *normals, int n)
{
    if(!normals)
        return;
    m_normals.resize(n);
    for(int i = 0; i < n; i++, normals += 3)
        m_normals[i] = vec3(normals[0], normals[1], normals[2]);
}

QVector<vec2> & MeshGL1::texCoords()
{
    return m_texCoords;
}

const QVector<vec2> & MeshGL1::texCoords() const
{
    return m_texCoords;
}

void MeshGL1::setTexCoords(float *texCoords, int n)
{
    if(!texCoords)
        return;
    m_texCoords.resize(n);
    for(int i = 0; i < n; i++, texCoords += 2)
        m_texCoords[i] = vec2(texCoords[0], texCoords[1]);
}

QVector<uint> & MeshGL1::indices()
{
    return m_indices;
}

const QVector<uint> & MeshGL1::indices() const
{
    return m_indices;
}

void MeshGL1::setIndices(uint *indices, int n)
{
    if(!indices)
        return;
    m_indices.resize(n);
    for(int i = 0; i < n; i++, indices++)
        m_indices[i] = indices[0];
}

const QVector<FaceGL1> & MeshGL1::faces() const
{
    return m_faces;
}

void MeshGL1::addVertices(VertexGroup *vg)
{
    uint destOffset = m_vertices.count();
    uint newSize = destOffset + vg->count;
    m_vertices.resize(newSize);
    m_normals.resize(newSize);
    m_texCoords.resize(newSize);
    m_indices.resize(newSize);

    VertexData *v = vg->data;
    for(int i = destOffset; i < newSize; i++, v++)
    {
        m_vertices[i] = v->position;
        m_normals[i] = v->normal;
        m_texCoords[i] = v->texCoords;
        m_indices[i] = i;
    }
    addFace(vg->mode, vg->count, destOffset);
}

void MeshGL1::addFace(uint mode, int vertexCount, int offset, bool draw)
{
    FaceGL1 f;
    f.mode = mode;
    f.count = vertexCount;
    f.offset = offset;
    f.draw = draw;
    m_faces.append(f);
}

/* Generate texture coordinates for 4 vertices-faced meshes */
void MeshGL1::generateTextureCoords()
{
    int indiceCount = m_indices.count();
    if((indiceCount == 0) || (m_vertices.count() < indiceCount))
        return;
    m_texCoords.resize(indiceCount);
    foreach(FaceGL1 f, m_faces)
    {
        if(f.count != 4)
            return;
        for(GLuint j = 0; j < 4; j++)
        {
            GLuint ind = m_indices[f.offset + j];
            vec2 *tc = &m_texCoords[ind];
            tc->x = (((j == 0) || (j == 3)) ? 0.0 : 1.0);
            tc->y = (((j == 0) || (j == 1)) ? 0.0 : 1.0);
        }
    }
}

void MeshGL1::computeNormals()
{
    int indiceCount = m_indices.count();
    if((indiceCount == 0) || (m_vertices.count() < indiceCount))
        return;

    vec3 u;
    m_normals.resize(indiceCount);
    foreach(FaceGL1 f, m_faces)
    {
        // use the face's first three vertices to compute its normal vector
        if(f.count < 3)
            continue;
        GLuint ind1 = m_indices[f.offset + 0];
        GLuint ind2 = m_indices[f.offset + 1];
        GLuint ind3 = m_indices[f.offset + 2];
        u = vec3::normal(m_vertices[ind1], m_vertices[ind2], m_vertices[ind3]);

        // assign it to every vertex in the face
        for(int i = 0; i < f.count; i++)
            m_normals[f.offset + i] = u;
    }
}

void MeshGL1::draw(MeshGL1::OutputMode mode, RenderState *s, Mesh *output)
{
    switch(mode)
    {
    default:
    case RenderToScreen:
        drawVertexList();
        break;
    case RenderToMesh:
        drawToMesh(output, s);
        break;
    }
}

void MeshGL1::drawVertexList()
{
    if(m_normals.count() > 0)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, m_normals.data());
    }
    if(m_texCoords.count() > 0)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords.data());
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_vertices.data());
    foreach(FaceGL1 f, m_faces)
    {
        if(f.draw)
        {
            if(m_indices.count() > 0)
                glDrawElements(f.mode, f.count, GL_UNSIGNED_INT, (m_indices.data() + f.offset));
            else
                glDrawArrays(f.mode, f.offset, f.count);
        }
    }
    if(m_texCoords.count() > 0)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if(m_normals.count() > 0)
        glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void MeshGL1::drawToMesh(Mesh *out, RenderState *s)
{
    if(!out || !s)
        return;
    foreach(FaceGL1 f, m_faces)
    {
        if(!f.draw)
            continue;
        VertexGroup *vg = drawFaceToMeshCopy(s, f);
        out->addVertices(vg);
        delete vg;
    }
}

VertexGroup * MeshGL1::drawFaceToMeshCopy(RenderState *s, FaceGL1 f)
{
    VertexGroup *vg = new VertexGroup(f.mode, f.count);
    VertexData *v = vg->data;
    matrix4 m = s->currentMatrix();
    for(int i = 0; i < f.count; i++, v++)
    {
        uint srcIndex = m_indices.value(f.offset + i);
        v->position = m.map(m_vertices.value(srcIndex));
        v->normal = m.mapNormal(m_normals.value(srcIndex));
        v->texCoords = m_texCoords.value(srcIndex);
    }
    return vg;
}

/* Show the normal for every vertex in the mesh, for debugging purposes. */
void MeshGL1::drawNormals(RenderState *s)
{
    static Material mat(vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), 0.0);
    if(m_normals.count() == 0)
        return;
    s->pushMaterial(mat);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    foreach(FaceGL1 face, m_faces)
    {
        if(!face.draw)
            continue;
        for(int p = 0; p < face.count; p++)
        {
            uint ind = (m_indices.count() > 0) ? m_indices[face.offset + p] : face.offset + p;
            vec3 v = m_vertices[ind];
            vec3 v2 = m_normals[ind] + v;
            glVertex3fv((GLfloat *)&v);
            glVertex3fv((GLfloat *)&v2);
        }
    }
    glEnd();
    glLineWidth(1.0);
    s->popMaterial();
}

MeshGL1 * MeshGL1::loadStl(const char *path, QObject *parent, bool compute_normals)
{
    MeshGL1 *m = 0;
    FILE *f = fopen(path, "rb");
    char header[80];
    uint32_t triangles = 0;
    vec3 points[4];
    uint16_t attributes;

    // read file header
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s'.\n", path);
        return 0;
    }
    if((fread(header, sizeof(header), 1, f) < 1) ||
            (fread(&triangles, 4, 1, f) < 1))
    {
        fclose(f);
        return 0;
    }

    // read vertex data
    m = new MeshGL1(parent);
    QVector<vec3> &vertices = m->vertices();
    QVector<vec3> &normals = m->normals();
    vertices.resize(3 * triangles);
    normals.resize(3 * triangles);
    for(uint32_t i = 0; i < triangles; i++)
    {
        if((fread(points, sizeof(points), 1, f) < 1) ||
            (fread(&attributes, 2, 1, f) < 1))
        {
            delete m;
            fclose(f);
            return 0;
        }
        if(compute_normals)
            points[0] = vec3::normal(points[1], points[2], points[3]);
        for(uint32_t v = 0; v < 3; v++)
        {
            uint32_t pos = (i * 3) + v;
            vertices[pos] = points[v + 1];
            normals[pos] = points[0];
        }
    }
    m->addFace(GL_TRIANGLES, triangles * 3, 0);
    fclose(f);
    return m;
}

void MeshGL1::saveStl(QString path)
{
    FILE *f = fopen(path.toUtf8().data(), "wb");
    char header[80];
    uint32_t triangles = 0;
    uint16_t attributes = 0;

    // count triangles
    foreach(FaceGL1 f, m_faces)
    {
        if(f.mode == GL_TRIANGLES)
            triangles += (f.count / 3);
    }

    // write file header
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s' for writing.\n", path.toUtf8().data());
        return;
    }
    memset(header, 0, sizeof(header));
    fwrite(header, sizeof(header), 1, f);
    fwrite(&triangles, sizeof(uint32_t), 1, f);

    // write vertex data
    QVector<vec3> lastVertices;
    QVector<vec3> lastNormals;
    foreach(FaceGL1 face, m_faces)
    {
        if(face.mode != GL_TRIANGLES)
            continue;
        for(int i = 0; i < face.count; i++)
        {
            uint srcIndex = m_indices.value(face.offset + i);
            lastVertices.append(m_vertices.value(srcIndex));
            lastNormals.append(m_normals.value(srcIndex));
            if(lastVertices.count() == 3)
            {
                vec3 normal = lastNormals[0];
                fwrite(&normal, 3 * sizeof(float), 1, f);
                foreach(vec3 vertex, lastVertices)
                    fwrite(&vertex, 3 * sizeof(float), 1, f);
                fwrite(&attributes, sizeof(attributes), 1, f);
                lastVertices.clear();
                lastNormals.clear();
            }
        }
    }
    fclose(f);
}

typedef struct
{
    uint vertexIndex;
    uint normalIndex;
    uint texCoordsIndex;
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

MeshGL1 * MeshGL1::loadObj(const char *path, QObject *parent)
{
    FILE *f = fopen(path, "r");
    char line[512];
    char point1[32];
    char point2[32];
    char point3[32];
    char point4[32];
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s'.\n", path);
        return 0;
    }

    MeshGL1 *m = new MeshGL1(parent);
    QVector<vec3> vertices;
    QVector<vec3> normals;
    QVector<vec2> texCoords;
    QVector<vec3> & meshVertices = m->vertices();
    QVector<vec3> & meshNormals = m->normals();
    QVector<vec2> & meshTexCoords = m->texCoords();
    QVector<uint> & meshIndices = m->indices();
    ObjPoint points[4];
    vec3 normal;
    uint indiceCount = 0;
    int maxPoints = 0;
    bool computeNormals = false;
    while(fgets(line, sizeof(line), f) != 0)
    {
        float v1, v2, v3;
        int pointCount = 0;
        if(sscanf(line, "v %f %f %f", &v1, &v2, &v3) == 3)
            vertices.append(vec3(v1, v2, v3));
        else if(sscanf(line, "vn %f %f %f", &v1, &v2, &v3) == 3)
            normals.append(vec3(v1, v2, v3));
        else if(sscanf(line, "vt %f %f", &v1, &v2) == 2)
            texCoords.append(vec2(v1, v2));
        else if(sscanf(line, "f %s %s %s %s", point1, point2, point3, point4) == 4)
        {
            if(parseObjPoint(point1, &points[0]) &&
                parseObjPoint(point2, &points[1]) &&
                parseObjPoint(point3, &points[2]) &&
                parseObjPoint(point4, &points[3]))
                pointCount = 4;
        }
        else if(sscanf(line, "f %s %s %s", point1, point2, point3) == 3)
        {
            if(parseObjPoint(point1, &points[0]) &&
                parseObjPoint(point2, &points[1]) &&
                parseObjPoint(point3, &points[2]))
                pointCount = 3;
        }
        if(pointCount > 0)
        {
            computeNormals = ((normals.count() == 0) && (pointCount >= 3));
            if(computeNormals)
            {
                vec3 v1 = vertices.value(points[0].vertexIndex - 1);
                vec3 v2 = vertices.value(points[1].vertexIndex - 1);
                vec3 v3 = vertices.value(points[2].vertexIndex - 1);
                normal = vec3::normal(v1, v2, v3);
            }
            for(int i = 0; i < pointCount; i++, indiceCount++)
            {
                meshVertices.append(vertices.value(points[i].vertexIndex - 1));
                if(computeNormals)
                    meshNormals.append(normal);
                else
                    meshNormals.append(normals.value(points[i].normalIndex - 1));
                meshTexCoords.append(texCoords.value(points[i].texCoordsIndex - 1));
                meshIndices.append(indiceCount);
            }
        }
        maxPoints = std::max(maxPoints, pointCount);
    }
    if((indiceCount > 0) && (maxPoints == 3))
        m->addFace(GL_TRIANGLES, indiceCount, 0);
    else if((indiceCount > 0) && (maxPoints == 4))
        m->addFace(GL_QUADS, indiceCount, 0);
    fclose(f);
    return m;
}

void MeshGL1::saveObj(QString path)
{
    FILE *f = fopen(path.toUtf8().data(), "w");

    // write file header
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s' for writing.\n", path.toUtf8().data());
        return;
    }
    foreach(vec3 vertex, m_vertices)
        fprintf(f, "v %f %f %f\n", vertex.x, vertex.y, vertex.z);
    foreach(vec3 normal, m_normals)
        fprintf(f, "vn %f %f %f\n", normal.x, normal.y, normal.z);
    foreach(vec2 texCoords, m_texCoords)
        fprintf(f, "vt %f %f\n", texCoords.x, texCoords.y);
    foreach(FaceGL1 face, m_faces)
    {
        switch(face.mode)
        {
        case GL_TRIANGLES:
            saveObjIndicesTri(f, face);
            break;
        case GL_QUADS:
            saveObjIndicesQuad(f, face);
            break;
        case GL_TRIANGLE_STRIP:
            saveObjIndicesTriStrip(f, face);
            break;
        }
    }
    fclose(f);
}

void MeshGL1::saveObjIndicesTri(FILE *f, FaceGL1 face)
{
    bool normals = m_normals.count() > 0;
    bool texCoords = m_texCoords.count() > 0;
    for(int i = 0; i < face.count; i += 3)
    {
        uint ind1 = m_indices[face.offset + i + 0];
        uint ind2 = m_indices[face.offset + i + 1];
        uint ind3 = m_indices[face.offset + i + 2];
        saveObjFace(f, ind1, ind2, ind3, normals, texCoords);
    }
}

void MeshGL1::saveObjIndicesQuad(FILE *f, FaceGL1 face)
{
    bool normals = m_normals.count() > 0;
    bool texCoords = m_texCoords.count() > 0;
    QVector<uint> indices;
    for(int i = 0; i < face.count; i++)
    {
        indices.append(m_indices[face.offset + i]);
        if(indices.count() == 4)
        {
            saveObjFace(f, indices[0], indices[1], indices[2], normals, texCoords);
            saveObjFace(f, indices[0], indices[2], indices[3], normals, texCoords);
            indices.clear();
        }
    }
}

void MeshGL1::saveObjIndicesTriStrip(FILE *f, FaceGL1 face)
{
    bool normals = m_normals.count() > 0;
    bool texCoords = m_texCoords.count() > 0;
    QVector<uint> indices;
    for(int i = 0; i < face.count; i++)
    {
        indices.append(m_indices[face.offset + i]);
        if(indices.count() == 3)
        {
            saveObjFace(f, indices[0], indices[1], indices[2], normals, texCoords);
            indices.remove(0);
        }
    }
}

void MeshGL1::saveObjFace(FILE *f, uint ind1, uint ind2, uint ind3, bool normals, bool texCoords)
{
    fprintf(f, "f ");
    saveObjIndice(f, ind1 + 1, normals, texCoords);
    fprintf(f, " ");
    saveObjIndice(f, ind2 + 1, normals, texCoords);
    fprintf(f, " ");
    saveObjIndice(f, ind3 + 1, normals, texCoords);
    fprintf(f, "\n");
}

void MeshGL1::saveObjIndice(FILE *f, uint ind, bool normals, bool texCoords)
{
    if(texCoords && normals)
        fprintf(f, "%d/%d/%d", ind, ind, ind);
    else if(normals)
        fprintf(f, "%d//%d", ind, ind);
    else if(texCoords)
        fprintf(f, "%d/%d", ind, ind);
    else
        fprintf(f, "%d", ind);
}
