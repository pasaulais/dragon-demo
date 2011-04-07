#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include "RenderStateGL2.h"

RenderStateGL2::RenderStateGL2(QObject *parent) : RenderState(parent)
{
    m_matrixMode = ModelView;
    m_matrix[(int)ModelView].setIdentity();
    m_matrix[(int)Projection].setIdentity();
    m_matrix[(int)Texture].setIdentity();
    m_ambient0 = vec4(1.0, 1.0, 1.0, 1.0);
    m_diffuse0 = vec4(1.0, 1.0, 1.0, 1.0);
    m_specular0 = vec4(1.0, 1.0, 1.0, 1.0);
    m_light0_pos = vec4(0.0, 1.0, 1.0, 0.0);
    m_vertexShader = 0;
    m_pixelShader = 0;
    m_program = 0;
    m_modelViewMatrixLoc = -1;
    m_projMatrixLoc = -1;
    m_texMatrixLoc = -1;
}

RenderStateGL2::~RenderStateGL2()
{
    if(m_vertexShader != 0)
        glDeleteShader(m_vertexShader);
    if(m_pixelShader != 0)
        glDeleteShader(m_pixelShader);
    if(m_program != 0)
        glDeleteProgram(m_program);
}

void RenderStateGL2::drawMesh(Mesh *m)
{
    if(!m)
        return;
    glUniformMatrix4fv(m_modelViewMatrixLoc, 1, GL_FALSE,
                       (const GLfloat *)m_matrix[(int)ModelView].d);
    glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE,
                       (const GLfloat *)m_matrix[(int)Projection].d);
    glUniformMatrix4fv(m_texMatrixLoc, 1, GL_FALSE,
                       (const GLfloat *)m_matrix[(int)Texture].d);
    m->draw(m_output, this, m_meshOutput);
    if(m_drawNormals)
        m->drawNormals(this);
}

void RenderStateGL2::setMatrixMode(RenderStateGL2::MatrixMode newMode)
{
    m_matrixMode = newMode;
}

void RenderStateGL2::loadIdentity()
{
    int i = (int)m_matrixMode;
    m_matrix[i].setIdentity();
}

void RenderStateGL2::multiplyMatrix(const matrix4 &m)
{
    int i = (int)m_matrixMode;
    m_matrix[i] = m_matrix[i] * m;
}

void RenderStateGL2::pushMatrix()
{
    int i = (int)m_matrixMode;
    m_matrixStack[i].append(m_matrix[i]);
}

void RenderStateGL2::popMatrix()
{
    int i = (int)m_matrixMode;
    m_matrix[i] = m_matrixStack[i].takeLast();
}

void RenderStateGL2::translate(float dx, float dy, float dz)
{
    multiplyMatrix(matrix4::translate(dx, dy, dz));
}

void RenderStateGL2::rotate(float angle, float rx, float ry, float rz)
{
    multiplyMatrix(matrix4::rotate(angle, rx, ry, rz));
}

void RenderStateGL2::scale(float sx, float sy, float sz)
{
    multiplyMatrix(matrix4::scale(sx, sy, sz));
}

matrix4 RenderStateGL2::currentMatrix() const
{
    return m_matrix[(int)m_matrixMode];
}

void RenderStateGL2::pushMaterial(const Material &m)
{
    m_materialStack.append(m);
    beginApplyMaterial(m);
}

void RenderStateGL2::popMaterial()
{
    Material m = m_materialStack.takeLast();
    endApplyMaterial(m);
    if(m_materialStack.count() > 0)
        beginApplyMaterial(m_materialStack.last());
}

void RenderStateGL2::beginApplyMaterial(const Material &m)
{
    setUniformValue("u_material_ambient", m.ambient());
    setUniformValue("u_material_diffuse", m.diffuse());
    setUniformValue("u_material_specular", m.specular());
    setUniformValue("u_material_shine", m.shine());
    if(m.texture() != 0)
    {
        glActiveTexture(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m.texture());
        setUniformValue("u_material_texture", 0);
        setUniformValue("u_has_texture", 1);
    }
    else
    {
        setUniformValue("u_has_texture", 0);
    }
}

void RenderStateGL2::endApplyMaterial(const Material &m)
{
    if(m.texture() != 0)
        glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderStateGL2::beginFrame(int w, int h)
{
    glPushAttrib(GL_ENABLE_BIT);
    glUseProgram(m_program);
    initShaders();
    glEnable(GL_DEPTH_TEST);
    //TODO smooth shading, VertexAttrib
    setUniformValue("u_light_ambient", m_ambient0);
    setUniformValue("u_light_diffuse", m_diffuse0);
    setUniformValue("u_light_specular", m_specular0);
    setUniformValue("u_light_pos", m_light0_pos);
    setupViewport(w, h);
    setMatrixMode(ModelView);
    pushMatrix();
    loadIdentity();
    glClearColor(m_bgColor.redF(), m_bgColor.greenF(), m_bgColor.blueF(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderStateGL2::endFrame()
{
    glFlush();
    setMatrixMode(ModelView);
    popMatrix();
    glUseProgram(0);
    glPopAttrib();
}

void RenderStateGL2::setupViewport(int w, int h)
{
    glViewport(0, 0, w, h);
    setMatrixMode(Projection);
    loadIdentity();
    float r = (float)w / (float)h;
    if(m_projection)
        multiplyMatrix(matrix4::perspective(45.0f, r, 0.1f, 100.0f));
    else if (w <= h)
        multiplyMatrix(matrix4::ortho(-1.0, 1.0, -1.0 / r, 1.0 / r, -10.0, 10.0));
    else
        multiplyMatrix(matrix4::ortho(-1.0 * r, 1.0 * r, -1.0, 1.0, -10.0, 10.0));
    setMatrixMode(ModelView);
}

void RenderStateGL2::init()
{
    loadShaders();
}

char * RenderStateGL2::loadShaderSource(const char *path) const
{
    FILE *f = fopen(path, "r");
    if(!f)
        return 0;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    char *code = 0;
    if(size >= 0)
    {
        fseek(f, 0, SEEK_SET);
        code = new char[(size_t)size + 1];
        if((size_t)size > fread(code, 1, (size_t)size, f))
        {
            free(code);
            code = 0;
        }
        else
        {
            code[size] = '\0';
        }
    }
    fclose(f);
    return code;
}

uint RenderStateGL2::loadShader(const char *path, uint type) const
{
    char *code = loadShaderSource(path);
    if(!code)
        return 0;
    uint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar **)&code, 0);
    delete [] code;
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        GLint log_size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
        if(log_size)
        {
            GLchar *log = new GLchar[log_size];
            glGetShaderInfoLog(shader, log_size, 0, log);
            fprintf(stderr, "Error compiling shader: %s\n", log);
            delete [] log;
        }
        else
        {
            fprintf(stderr, "Error compiling shader.\n");
        }
        return 0;
    }
    return shader;
}

bool RenderStateGL2::loadShaders()
{
    uint vertexShader = loadShader("vertex.glsl", GL_VERTEX_SHADER);
    if(vertexShader == 0)
        return false;
    uint pixelShader = loadShader("fragment.glsl", GL_FRAGMENT_SHADER);
    if(pixelShader == 0)
    {
        glDeleteShader(vertexShader);
        return false;
    }
    uint program = glCreateProgram();
    if(program == 0)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(pixelShader);
        return false;
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status)
    {
        GLint log_size;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
        if(log_size)
        {
            GLchar *log = new GLchar[log_size];
            glGetProgramInfoLog(program, log_size, 0, log);
            fprintf(stderr, "Error linking program: %s\n", log);
            delete [] log;
        }
        else
        {
            fprintf(stderr, "Error linking program.\n");
        }
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(pixelShader);
        return 0;
    }
    m_program = program;
    m_vertexShader = vertexShader;
    m_pixelShader = pixelShader;
    m_modelViewMatrixLoc = glGetUniformLocation(program, "u_modelViewMatrix");
    m_projMatrixLoc = glGetUniformLocation(program, "u_projectionMatrix");
    m_texMatrixLoc = glGetUniformLocation(program, "u_textureMatrix");
    return true;
}

void RenderStateGL2::initShaders()
{
}

void RenderStateGL2::setUniformValue(const char *name, const vec4 &v)
{
    int location = glGetUniformLocation(m_program, name);
    glUniform4fv(location, 1, (GLfloat *)&v);
}

void RenderStateGL2::setUniformValue(const char *name, float f)
{
    int location = glGetUniformLocation(m_program, name);
    glUniform1f(location, f);
}

void RenderStateGL2::setUniformValue(const char *name, int i)
{
    int location = glGetUniformLocation(m_program, name);
    glUniform1i(location, i);
}
