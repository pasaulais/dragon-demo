#include "Platform.h"

void checkGlError(const char* op)
{
    for(GLint error = glGetError(); error; error = glGetError())
        LOGI("after %s() glError (0x%x)\n", op, error);
}

#ifdef JNI_WRAPPER

char *loadFileData(std::string path)
{
    FILE *f = fopen(path.c_str(), "r");
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
            delete [] code;
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

#else

#include <QFile>

QString resolvePath(QString path)
{
    // try to load from resources if the file is not found
    if(QFile::exists(path))
        return path;
    else
        return QString(":/%1").arg(path);
}

char *loadFileData(std::string path)
{
    QString realPath = resolvePath(QString::fromStdString(path));
    QFile f(realPath);
    if(!f.open(QFile::ReadOnly))
    {
        fprintf(stderr, "Could not open file '%s' for reading.\n", realPath.toLatin1().constData());
        return 0;
    }
    QByteArray data = f.readAll();
    char *code = new char[data.length() + 1];
    memcpy(code, data.constData(), data.length());
    code[data.length()] = '\0';
    return code;
}

void freeFileData(char *data)
{
    delete [] data;
}

#endif
