#include "SceneMng.h"

namespace Chimera {

	SceneMng::SceneMng(Node *_pRoot)  {

		pRoot = _pRoot;
		parseEntity(pRoot);
		pCameraAtiva = nullptr;
        pObjeto = nullptr;

        memset(g_mProjection,0, sizeof(float)*16);
        memset(g_mModelView,0,sizeof(float)*16);
        g_LightView.setZero();

	}

	SceneMng::~SceneMng() {

	}

	void SceneMng::parseEntity(Node *_pNode) {

		addEntityToScene(_pNode);

		for (Node *node : _pNode->listChild) {
			parseEntity(node);
		}

	}

	void SceneMng::addEntityToScene(Node *_pNode) {

		switch (_pNode->getKind()) {
		case EntityKind::CAMERA:
			m_vCamera.push_back((Camera*)_pNode);
			break;
		case EntityKind::LIGHT:
			m_vLight.push_back((Light*)_pNode);
			break;
		case EntityKind::OBJECT:
			m_vObject.push_back((Object*)_pNode);
			break;
        case EntityKind::PARTICLE_EMITTER:
            m_vParticle.push_back((ParticleEmitter*)_pNode);
            break;
        case EntityKind::SKYBOX:
            m_vSkyBox.push_back((SkyBox*)_pNode);
            break;
		default:
			break;
		}

	}

	void SceneMng::addChildToScene(Node *_pNode) {

		pRoot->addChild(_pNode);
		addEntityToScene(_pNode);

	}

	Node *SceneMng::getNode(EntityKind _type, const std::string &_nome) {

        Node *retorno = nullptr;

        switch (_type) {
            case EntityKind::CAMERA:
                for(Node *node : m_vCamera) {
                    if (node->getName().compare(_nome) == 0) {
                        return node;
                    }
                }
                break;
             case EntityKind::LIGHT:
                 for(Node *node : m_vLight) {
                     if (node->getName().compare(_nome) == 0) {
                         return node;
                     }
                 }
                 break;
             case EntityKind::OBJECT:
                 for(Node *node : m_vObject) {
                     if (node->getName().compare(_nome) == 0) {
                         return node;
                     }
                 }
                break;
            case EntityKind::PARTICLE_EMITTER:
                for(Node *node : m_vParticle) {
                    if (node->getName().compare(_nome) == 0) {
                        return node;
                    }
                }
                break;
            case EntityKind::SKYBOX:
                for(Node *node : m_vSkyBox) {
                    if (node->getName().compare(_nome) == 0) {
                        return node;
                    }
                }
                break;
            default:
                break;
        }

        return nullptr;

    }

	Node *SceneMng::getNode(EntityKind _type, unsigned index) {

		Node *retorno = nullptr;

		switch (_type) {
		case EntityKind::CAMERA:
			if (m_vCamera.size() > index)
                retorno = m_vCamera[index];
			break;
		case EntityKind::LIGHT:
			if (m_vLight.size() > index)
                retorno = m_vLight[index];
			break;
		case EntityKind::OBJECT:
			if (m_vObject.size() > index)
                retorno = m_vObject[index];
			break;
        case EntityKind::PARTICLE_EMITTER:
            if (m_vParticle.size() > index)
                retorno = m_vParticle[index];
            break;
        case EntityKind::SKYBOX:
            if (m_vSkyBox.size() > index)
                retorno = m_vSkyBox[index];
            break;
		default:
			break;
		}

		return retorno;
	}

	void SceneMng::execLight() {
		for (Light *pLight : m_vLight) {
			pLight->exec();
		}
	}

	void SceneMng::update(DataMsg *dataMsg) {

        if (dataMsg->getKindOp() == KindOp::START) {

            //inicialize primeiro os filhos para garantir textura e efeito em material
            pRoot->update(dataMsg);

            //inicializa objeto local
            init();

        } else {
            pRoot->update(dataMsg);
        }

    }

    //  void SceneMng::draw3d() {
    //
    //      pCameraAtiva->exec();
    //
    //      if (pSkyBox != nullptr)
    //          pSkyBox->render();
    //
    //      Chimera::DataMsg dataMsg(KindOp::DRAW3D, this, pObjeto, nullptr);
    //      update(&dataMsg);
    //
    //         for (Light *pLight : m_vLight) {
    //             pLight->exec();
    //         }
    //
    //      //execLight();
    //
    //  }

    void SceneMng::init()
    {
        //TODO opengldeve esta inicializado!!!!!


//         /////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
//
//         // Here we initialize our multi-texturing functions
//         glActiveTextureARB      = (PFNGLACTIVETEXTUREARBPROC)       wglGetProcAddress("glActiveTextureARB");
//         glMultiTexCoord2fARB    = (PFNGLMULTITEXCOORD2FARBPROC)     wglGetProcAddress("glMultiTexCoord2fARB");

//         // Make sure our multi-texturing extensions were loaded correctly
//         if(!glActiveTextureARB || !glMultiTexCoord2fARB)
//         {
//             // Print an error message and quit.
//             MessageBox(g_hWnd, "Your current setup does not support multitexturing", "Error", MB_OK);
//             PostQuitMessage(0);
//         }
//
//         // Tell OpenGL our light's position
//         glLightfv( GL_LIGHT0, GL_POSITION, g_LightPosition );
//
//         // This turns the background to a dark grey/black.
//         glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//
//         // Turn on our light and enable color along with the light
//         glEnable(GL_LIGHTING);
//         glEnable(GL_LIGHT0);
//         glEnable(GL_COLOR_MATERIAL);


        // Here we allocate memory for our depth texture that will store our light's view.
        // We must set the channels and type for the texture as GL_DEPTH_COMPONENT.
        CreateRenderTexture(g_Texture, SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, SHADOW_ID);

    }

    ///////////////////////////////// STORE LIGHT MATRICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
    /////
    /////   This function positions our view from the light for shadow mapping
    /////
    ///////////////////////////////// STORE LIGHT MATRICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

    void SceneMng::StoreLightMatrices(const btVector3 &posicao) //float g_LightPosition[3])
    {
        // In this function we just set our camera position to the light's position
        // and then store the current modelview matrix.  Lastly, we set our light's
        // frustum (perspective) to set our depth precision.  Ideally, the smaller
        // the frustum is, the more precision our depth map is, which gives a better
        // quality shadow.  Notice that we change the perspective to a 60 degree field
        // of view.  This allows us to view more of the world from our light's position.
        // That way we can have closer near and far planes, giving better depth values.

        // Reset our current light matrices
        memset(g_mModelView, 0, sizeof(float)*16);
        memset(g_mProjection, 0, sizeof(float)*16);

        // Let's push on a new matrix so we don't change the rest of the world
        glPushMatrix();

        // Push on a new matrix to keep our view changes isolated
        glPushMatrix();

        // Reset the current modelview matrix
        glLoadIdentity();

        // This is where we set the light's position and view.
        gluLookAt(posicao.getX(), posicao.getY(), posicao.getZ(),
                  g_LightView.getX(), g_LightView.getY(), g_LightView.getZ(),
                  0, 1, 0);

        // Now that we have the light's view, let's save the current modelview matrix.
        glGetFloatv(GL_MODELVIEW_MATRIX, g_mModelView);

        // Now pop off the current light view's matrix
        glPopMatrix();

        // We could go to prospective mode and change the perspective,
        // then have to restore our camera's perspective and go back to modelview
        // mode.  However, if we just start with a new modelview matrix, then change
        // the perspective and grab the current matrix we save some steps and restorations.

        // Reset the current matrix
        glLoadIdentity();

        // Change our light's perspective to 60 degrees FOV, aspect ratio 1, with a near
        // and far plane of 0.5 and 25.0.  The far plane is only 25 because our world isn't
        // larger than 25.  Depending on how large your world is you will need to increase
        // the far plane and/or add additional lights.
        gluPerspective(60.0f, 1.0f, 0.5f, 25.0f);

        // Even though we want the perspective matrix, we can still grab the
        // current modelview matrix and use that as our projection matrix because
        // we used gluPerspective() on a new matrix, which is the projection matrix.
        glGetFloatv(GL_MODELVIEW_MATRIX, g_mProjection);

        // Go back to the original matrix
        glPopMatrix();
    }

    /////////////////////////////// CREATE RENDER TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
    /////
    /////   This function creates a blank texture to render to
    /////
    /////////////////////////////// CREATE RENDER TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

    void SceneMng::CreateRenderTexture(unsigned int textureArray[], int sizeX, int sizeY, int channels, int type, int textureID)
    {
        // This function is a modified version of what we have used in previous tutorials.
        // We just allowed the option of doing a depth texture.  When doing a depth
        // texture we need to pass in GL_DEPTH_COMPONENT for the type AND the channels.
        // To check for this we just see if the channels are greater than 4.  If so, we
        // allocate a texture for just one bit per pixel, then pass in the channel value
        // for glTexImage2D().

        // Create a pointer to store the blank image data
        unsigned int *pTexture = NULL;

        // Store the current channels to be allocated by default
        int channelsTrue = channels;

        // If the channels are greater than 4 there must be a special flag like
        // GL_DEPTH_COMPONENT, so make sure we only allocate 1 bit per pixel.
        if(channels > 4)
            channelsTrue = 1;

        // Allocate and init memory for the image array and point to it from pTexture
        pTexture = new unsigned int [sizeX * sizeY * channelsTrue];
        memset(pTexture, 0, sizeX * sizeY * channelsTrue * sizeof(unsigned int));

        // Register the texture with OpenGL and bind it to the texture ID
        glGenTextures(1, &textureArray[textureID]);
        glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

        // Create the texture and store it on the video card
        glTexImage2D(GL_TEXTURE_2D, 0, channels, sizeX, sizeY, 0, type, GL_UNSIGNED_INT, pTexture);

        // Set the texture quality
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        // Since we stored the texture space with OpenGL, we can delete the image data
        delete [] pTexture;
    }

    //////////////////////////////// APPLY SHADOW MAP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
    /////
    /////   This function applies the shadow map to our world data
    /////
    //////////////////////////////// APPLY SHADOW MAP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

    void SceneMng::ApplyShadowMap()
    {
        // Now comes the juice of this tutorial.  What this function will do is:
        //
        // 1) Turn on texture mapping
        // 2) Bind our shadow texture that holds the depth values
        // 3) Set our texture mode and function for shadow mapping
        // 4) Turn on and set OpenGL texture generation by GL_EYE_LINEAR
        // 5) Load the light's matrices into the texture matrix
        // 6) Render the world that will be shadowed
        // 7) Restore and turn everything off again
        //

        // Turn on our texture unit for shadow mapping and bind our depth texture
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_Texture[SHADOW_ID]);

        // Here is where we set the mode and function for shadow-mapping in fixed functionality.
        // The mode we use is GL_TEXTURE_COMPARE_MODE_ARB, with GL_COMPARE_R_TO_TEXTURE_ARB, which
        // tells OpenGL that we want to compare the depth value in our world to the current
        // texture's depth value.  We then set the compare function to GL_LEQUAL, which says that
        // we will not shadow the current pixel if the depth value is less than or equal to
        // the texture's depth value.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

        // Let's explain really fast about why we need a "bias" matrix.  Well, when we multiply
        // our world data by the projection and modelview matrix it converts it to clip space,
        // which is a box that is measured in -1 to 1.  Then this is later converted to screen
        // coordinates.  Now, our texture coordinates are clamped from 0 to 1 right?  We are
        // working in the light's clip space and we want to convert the -1 to 1 ratio to a
        // 0 to 1 ratio, which will measure up with our texture coordinates.  To do this we
        // need a "bias" matrix that will convert from -1 to 1 to a 0 to 1 ratio.  We first
        // load the bias matrix into our texture matrix and then multiply the projection and
        // modelview matrix by the bias matrix.  The bias matrix that will do this is below.
        // It's the same thing as doing this to every coordinate: newCoord = (oldCoord + 1) / 2;
        const float mBias[] = {0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0};

            // Now comes the complicated part to grasp at first.  If you have never used the
            // texture generation functionality in OpenGL for sphere mapping or something else,
            // I will give a basic explanation of what it does.  So that we don't have to
            // do the work ourselves of calculating every UV coordinate, we can have the UV
            // coordinates generated by OpenGL for us, given a bunch of different inputs to work from.
            // We can use OpenGL's texture generation functionality to project our camera depth values
            // into the light's clip space.  Since our matrices are 4x4, we need to do our
            // projection in 4D.  Thus, we use texture coordinates S, T, R and Q.

            // We first create planes for each axis (x, y, z, w).  This will be used
            // with our projection of our camera's view to the light's clip space and
            // make sure the texture coordinates don't move when our camera moves, but
            // are the same no matter where our camera is looking.  The equation for
            // doing projected texturing is:
            //
            // mProjectedTexture = mLightProjection * mLightModelview * mCameraInverse;
            //
            // We say the "camera inverse", but really it is the modelview matrix's inverse
            // that has the camera's view applied to it.  So this is what we mean when we say that.
            // Usually we will need to multiply the inverse of our camera matrix by the projection
            // and modelview matrix of the light; however, when using these planes for our camera
            // OpenGL will calculate the inverse of our camera for us.  In the GLSL version of
            // shadow mapping we calculate the camera inverse ourselves.

            // Create the eye planes for generating texture coordinates
            const double xPlane[] = {1.0, 0.0, 0.0, 0.0};
            const double yPlane[] = {0.0, 1.0, 0.0, 0.0};
            const double zPlane[] = {0.0, 0.0, 1.0, 0.0};
            const double wPlane[] = {0.0, 0.0, 0.0, 1.0};

            // Enable texture generation for S, T, R, and Q
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_GEN_R);
            glEnable(GL_TEXTURE_GEN_Q);

            // Set each texture coordinate's texture gen mode to use the camera.
            // This will have OpenGL incorporate the camera's inverse for us.
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
            glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
            glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

            // Set our planes for each coordinate in order to project the texture appropriatly.
            glTexGendv(GL_S, GL_EYE_PLANE, xPlane );
            glTexGendv(GL_T, GL_EYE_PLANE, yPlane );
            glTexGendv(GL_R, GL_EYE_PLANE, zPlane );
            glTexGendv(GL_Q, GL_EYE_PLANE, wPlane );

            // Now we actually do the matrix multiplication.  First we switch to texture
            // mode.  That way we aren't effecting the modelview matrix.  Then we load in
            // the bias matrix, multiply that by the light's projection matrix, then multiply
            // that result by the light's modelview matrix.  This, in conjunction with our
            // camera inverse matrix calculated by OpenGL for us, gives us the right matrix
            // for projecting everything that needs to be projected.  That is, the camera's
            // depth values into the light's clip space, then the generated shadow map onto
            // the geometry of the world.
            glMatrixMode(GL_TEXTURE);
            glLoadMatrixf(mBias);           // The bias matrix to convert to a 0 to 1 ratio
            glMultMatrixf(g_mProjection);   // The light's projection matrix
            glMultMatrixf(g_mModelView);    // The light's modelview matrix
            glMatrixMode(GL_MODELVIEW);         // Switch back to normal modelview mode

            //FIXME
            //RenderWorld();                      // Render the world that needs to be shadowed
            Chimera::DataMsg dataMsg(KindOp::DRAW3D, this, pObjeto, nullptr);
            update(&dataMsg);


            // Now that the world is shadowed and we are done with the texture generation,
            // let's set everything back to normal by resetting the texture matrix.
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);

            // Turn off texture generation for our S, T, R and Q coordinates
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_GEN_R);
            glDisable(GL_TEXTURE_GEN_Q);

            // Turn the first multi-texture pass off
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
    }

    void SceneMng:: RenderSceneA()
    {
        // This is the last function that we need explaining for this tutorial.
        // We have done all the shadow mapping work above this, but we still haven't
        // gone over actually rendering the depth values of the light's view to a
        // texture.  Let's go over each line and see how this is done.  It's simple.

        // Since our light is moving every frame we will need to update the light's
        // matrices.  We really only need to update the modelview matrix every frame,
        // but we do the projection matrix too just to make it a simple function call.
        // If your light never moves, you only need to call this function once.
        //StoreLightMatrices();
        for (Light *pLight : m_vLight) {

            btVector3 posicao = pLight->getPosition();
            StoreLightMatrices(posicao);

        }

        // Clear the screen and depth buffer so we can render from the light's view
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Since our texture map that will store the depth values is a different size
        // than our normal viewport, we need to change the viewport to make it the same
        // size as our texture.
        glViewport(0, 0, (int)SHADOW_WIDTH, (int)SHADOW_HEIGHT);

        // Now we just need to set the matrices for the light before we render.
        // We switch to projection mode and input our light's projection matrix.
        glMatrixMode(GL_PROJECTION);

        // Push on a matrix to make sure we can restore to the old matrix easily
        glPushMatrix();

        // Set the current projection matrix to our light's projection matrix
        glLoadMatrixf(g_mProjection);

        // Load modelview mode to set our light's modelview matrix
        glMatrixMode(GL_MODELVIEW);

        // Load the light's modelview matrix before we render to a texture
        glLoadMatrixf(g_mModelView);

        // Since we don't care about color when rendering the depth values to
        // the shadow-map texture, we disable color writing to increase speed.
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        // This special function fixes artifacts that lead to small errors when
        // calculating the shadow map.  You will need to adjust the scale depending
        // on your scene, but I chose the scale 8.0 for mine.  NVIDIA suggests 1.1
        // or higher.  The 4.0 should stay fairly the same.
        glPolygonOffset(8.0f, 4.0f);

        // This turns of the polygon offset functionality to fix artifacts.
        // Comment this out and run the program to see what artifacts I mean.
        glEnable(GL_POLYGON_OFFSET_FILL);

        // Render the world according to the light's view
        //RenderWorld();
        Chimera::DataMsg dataMsg(KindOp::DRAW3D, this, pObjeto, nullptr);
        update(&dataMsg);

        // Now that the world is rendered, save the depth values to a texture
        glBindTexture(GL_TEXTURE_2D, g_Texture[SHADOW_ID]);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)SHADOW_WIDTH, (int)SHADOW_HEIGHT);

        // We can turn color writing back on since we already stored the depth values
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        // Turn off polygon offsetting
        glDisable(GL_POLYGON_OFFSET_FILL);

        // Restore our normal viewport size to our screen width and height
       // glViewport(0, 0, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);

        // Go back to the projection mode and restore the original matrix
        glMatrixMode(GL_PROJECTION);

        // Restore the original projection matrix
        glPopMatrix();

        // Go back to modelview model to start drawing like normal
        glMatrixMode(GL_MODELVIEW);

        // Clear the color and depth bits and start over from the camera's view
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    }

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
