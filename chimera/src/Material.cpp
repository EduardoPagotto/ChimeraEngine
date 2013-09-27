#include "Material.h"

namespace Chimera {

Material::Material(void)
{
	m_pTexture = NULL;
	m_idTex = 0;
}

Material::Material ( const Material &_cpy ) {
    m_effect = _cpy.m_effect;
    if ( _cpy.m_pTexture!=NULL ) {
        m_pTexture = _cpy.m_pTexture;
        m_idTex = _cpy.m_idTex;
    }
}

Material::~Material()
{

}

void Material::exec(void)
{
	link();

    float val = m_effect.getShininess();
    
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_effect.getAmbient().ptr());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_effect.getDiffuse().ptr());
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_effect.getSpecular().ptr());
    glMaterialfv(GL_FRONT, GL_SHININESS, &val);

	glMaterialfv(GL_FRONT, GL_EMISSION, m_effect.getEmissive().ptr());
}

void Material::initTex(void)
{
	if (m_pTexture)
	{
		glGenTextures(1, &m_idTex);
	//	//glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D,m_idTex);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3,image->w, image->h, 0, GL_BGR,GL_UNSIGNED_BYTE, image->pixels);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_pTexture->pImage->w, 
											m_pTexture->pImage->h, 
											GL_RGB,
											GL_UNSIGNED_BYTE,
											m_pTexture->pImage->pixels);

		//TODO:carregar conf do arquivo collada
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

}

int Material::link(void)
{
	if (m_idTex >0 )
		glBindTexture(GL_TEXTURE_2D,m_idTex);

	return m_idTex;
}

}