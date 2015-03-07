// Copyright (c) 2009-2015, Pierre-Andre Saulais <pasaulais@free.fr>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
