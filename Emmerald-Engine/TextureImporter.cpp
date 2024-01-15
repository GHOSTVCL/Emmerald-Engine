#include "Application.h"
#include "TextureImporter.h"
#include "CompTexture.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "DevIL/libx86/DevIL.lib")
#pragma comment(lib, "DevIL/libx86/ILU.lib")
#pragma comment(lib, "DevIL/libx86/ILUT.lib")


Texture* TextureImporter::ImportTexture(std::string textfile, GameObject* GoToTex)
{
    ILenum imageToTextID;
    ILboolean done;

    ilGenImages(1, &imageToTextID);
    ilBindImage(imageToTextID);

    done = ilLoadImage(textfile.c_str());

    if (done == IL_TRUE) {
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

        GLuint _texture = 0;

        GLuint width = ilGetInteger(IL_IMAGE_WIDTH);
        GLuint height = ilGetInteger(IL_IMAGE_HEIGHT);
        ILubyte* textdata = ilGetData();

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, textdata);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
     
        ilDeleteImages(1, &imageToTextID);
        if (GoToTex != nullptr)
        if (GoToTex->GetComponent<CompMesh>()->mesh != NULL)
        {
            GoToTex->GetComponent<CompMesh>()->mesh->textid = new Texture(_texture, width, height);
            GoToTex->GetComponent<CompTexture>()->SetTexture(new Texture(_texture, width, height));
            GoToTex->GetComponent<CompTexture>()->path = textfile;
        }
        
        return new Texture(_texture, width, height);
    }
}

