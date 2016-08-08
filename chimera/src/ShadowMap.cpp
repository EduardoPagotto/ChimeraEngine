#include "Video.h"
#include "ShadowMap.h"

// These store our width and height for the shadow texture.  The higher the
// texture size the better quality shadow.  Must be power of two for most video cards.
# define SHADOW_WIDTH 512
# define SHADOW_HEIGHT 512

namespace Chimera {

ShadowMap::ShadowMap() {
    memset ( g_mProjection, 0, sizeof ( float ) * 16 );
    memset ( g_mModelView, 0, sizeof ( float ) * 16 );
    g_LightView = glm::vec3(0.0,0.0,0.0);

    pTexture = new Texture ( "ShadowMap-01" ,"");
}

ShadowMap::~ShadowMap() {

}

void ShadowMap::init ( Node *_pScene ) {
    pScene = _pScene;

    // Here we allocate memory for our depth texture that will store our light's view.
    // We must set the channels and type for the texture as GL_DEPTH_COMPONENT.
    pTexture->createRenderTexture ( SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT );
}

///////////////////////////////// STORE LIGHT MATRICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function positions our view from the light for shadow mapping
/////
///////////////////////////////// STORE LIGHT MATRICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
void ShadowMap::StoreLightMatrices ( const glm::vec3 &posicao ) {
    // In this function we just set our camera position to the light's position
    // and then store the current modelview matrix.  Lastly, we set our light's
    // frustum (perspective) to set our depth precision.  Ideally, the smaller
    // the frustum is, the more precision our depth map is, which gives a better
    // quality shadow.  Notice that we change the perspective to a 60 degree field
    // of view.  This allows us to view more of the world from our light's position.
    // That way we can have closer near and far planes, giving better depth values.

    // Reset our current light matrices
    memset ( g_mModelView, 0, sizeof ( float ) * 16 );
    memset ( g_mProjection, 0, sizeof ( float ) * 16 );

    // Let's push on a new matrix so we don't change the rest of the world
    glPushMatrix();

    // Push on a new matrix to keep our view changes isolated
    glPushMatrix();

    // Reset the current modelview matrix
    glLoadIdentity();

    // This is where we set the light's position and view.
    gluLookAt ( posicao.x, posicao.y, posicao.z,
                g_LightView.x, g_LightView.y, g_LightView.z,
                0, 1, 0 );

    // Now that we have the light's view, let's save the current modelview matrix.
    glGetFloatv ( GL_MODELVIEW_MATRIX, g_mModelView );

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
    gluPerspective ( 90.0f, 1.0f, 0.5f, 25.0f );

    // Even though we want the perspective matrix, we can still grab the
    // current modelview matrix and use that as our projection matrix because
    // we used gluPerspective() on a new matrix, which is the projection matrix.
    glGetFloatv ( GL_MODELVIEW_MATRIX, g_mProjection );

    // Go back to the original matrix
    glPopMatrix();
}

void ShadowMap::initApplyShadow() {
	pTexture->beginTexCreated();

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
	const float mBias[] = { 0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};

	// Now we actually do the matrix multiplication.  First we switch to texture
	// mode.  That way we aren't effecting the modelview matrix.  Then we load in
	// the bias matrix, multiply that by the light's projection matrix, then multiply
	// that result by the light's modelview matrix.  This, in conjunction with our
	// camera inverse matrix calculated by OpenGL for us, gives us the right matrix
	// for projecting everything that needs to be projected.  That is, the camera's
	// depth values into the light's clip space, then the generated shadow map onto
	// the geometry of the world.
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(mBias);                        // The bias matrix to convert to a 0 to 1 ratio
	glMultMatrixf(g_mProjection);                // The light's projection matrix
	glMultMatrixf(g_mModelView);                 // The light's modelview matrix
	glMatrixMode(GL_MODELVIEW);                  // Switch back to normal modelview mode

	//FIXME Implementar o root do GE
	// Render the world that needs to be shadowed

}

void ShadowMap::endApplyShadow() {

	// Now that the world is shadowed and we are done with the texture generation,
	// let's set everything back to normal by resetting the texture matrix.
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	pTexture->endTexCreated();
}

//////////////////////////////// APPLY SHADOW MAP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function applies the shadow map to our world data
/////
//////////////////////////////// APPLY SHADOW MAP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//void ShadowMap::ApplyShadowMap ( void *pObjeto ) {
//     pTexture->beginTexCreated();
// 
//     // Let's explain really fast about why we need a "bias" matrix.  Well, when we multiply
//     // our world data by the projection and modelview matrix it converts it to clip space,
//     // which is a box that is measured in -1 to 1.  Then this is later converted to screen
//     // coordinates.  Now, our texture coordinates are clamped from 0 to 1 right?  We are
//     // working in the light's clip space and we want to convert the -1 to 1 ratio to a
//     // 0 to 1 ratio, which will measure up with our texture coordinates.  To do this we
//     // need a "bias" matrix that will convert from -1 to 1 to a 0 to 1 ratio.  We first
//     // load the bias matrix into our texture matrix and then multiply the projection and
//     // modelview matrix by the bias matrix.  The bias matrix that will do this is below.
//     // It's the same thing as doing this to every coordinate: newCoord = (oldCoord + 1) / 2;
//     const float mBias[] = { 0.5, 0.0, 0.0, 0.0,
//                             0.0, 0.5, 0.0, 0.0,
//                             0.0, 0.0, 0.5, 0.0,
//                             0.5, 0.5, 0.5, 1.0
//                           };
// 
//     // Now we actually do the matrix multiplication.  First we switch to texture
//     // mode.  That way we aren't effecting the modelview matrix.  Then we load in
//     // the bias matrix, multiply that by the light's projection matrix, then multiply
//     // that result by the light's modelview matrix.  This, in conjunction with our
//     // camera inverse matrix calculated by OpenGL for us, gives us the right matrix
//     // for projecting everything that needs to be projected.  That is, the camera's
//     // depth values into the light's clip space, then the generated shadow map onto
//     // the geometry of the world.
//     glMatrixMode ( GL_TEXTURE );
//     glLoadMatrixf ( mBias );                        // The bias matrix to convert to a 0 to 1 ratio
//     glMultMatrixf ( g_mProjection );                // The light's projection matrix
//     glMultMatrixf ( g_mModelView );                 // The light's modelview matrix
//     glMatrixMode ( GL_MODELVIEW );                  // Switch back to normal modelview mode
// 
//     //FIXME Implementar o root do GE
//     // Render the world that needs to be shadowed
//     DataMsg dataMsg ( KindOp::DRAW_NO_TEX, this, pObjeto, nullptr );
//     pScene->update ( &dataMsg );
// 
//     // Now that the world is shadowed and we are done with the texture generation,
//     // let's set everything back to normal by resetting the texture matrix.
//     glMatrixMode ( GL_TEXTURE );
//     glLoadIdentity();
//     glMatrixMode ( GL_MODELVIEW );
// 
//     pTexture->endTexCreated();
//}

void ShadowMap::initSceneShadow() {

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
}

void ShadowMap::endSceneShadow() {

	// Now that the world is rendered, save the depth values to a texture
	pTexture->apply();
	//glBindTexture(GL_TEXTURE_2D, g_Texture[SHADOW_ID]);
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

//void ShadowMap::RenderSceneA ( void *pObjeto ) {
//
//     // Clear the screen and depth buffer so we can render from the light's view
//     glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//     glLoadIdentity();
// 
//     // Since our texture map that will store the depth values is a different size
//     // than our normal viewport, we need to change the viewport to make it the same
//     // size as our texture.
//     glViewport ( 0, 0, ( int ) SHADOW_WIDTH, ( int ) SHADOW_HEIGHT );
// 
//     // Now we just need to set the matrices for the light before we render.
//     // We switch to projection mode and input our light's projection matrix.
//     glMatrixMode ( GL_PROJECTION );
// 
//     // Push on a matrix to make sure we can restore to the old matrix easily
//     glPushMatrix();
// 
//     // Set the current projection matrix to our light's projection matrix
//     glLoadMatrixf ( g_mProjection );
// 
//     // Load modelview mode to set our light's modelview matrix
//     glMatrixMode ( GL_MODELVIEW );
// 
//     // Load the light's modelview matrix before we render to a texture
//     glLoadMatrixf ( g_mModelView );
// 
//     // Since we don't care about color when rendering the depth values to
//     // the shadow-map texture, we disable color writing to increase speed.
//     glColorMask ( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
// 
//     // This special function fixes artifacts that lead to small errors when
//     // calculating the shadow map.  You will need to adjust the scale depending
//     // on your scene, but I chose the scale 8.0 for mine.  NVIDIA suggests 1.1
//     // or higher.  The 4.0 should stay fairly the same.
//     glPolygonOffset ( 8.0f, 4.0f );
// 
//     // This turns of the polygon offset functionality to fix artifacts.
//     // Comment this out and run the program to see what artifacts I mean.
//     glEnable ( GL_POLYGON_OFFSET_FILL );
// 
//     //FIXME Implementar o root do GE
//     // Render the world according to the light's view
//     //RenderWorld();
//     DataMsg dataMsg ( KindOp::DRAW_NO_TEX, this, pObjeto, nullptr );
//     pScene->update ( &dataMsg );
// 
//     // Now that the world is rendered, save the depth values to a texture
//     pTexture->apply();
//     //glBindTexture(GL_TEXTURE_2D, g_Texture[SHADOW_ID]);
//     glCopyTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, 0, 0, ( int ) SHADOW_WIDTH, ( int ) SHADOW_HEIGHT );
// 
//     // We can turn color writing back on since we already stored the depth values
//     glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
// 
//     // Turn off polygon offsetting
//     glDisable ( GL_POLYGON_OFFSET_FILL );
// 
//     // Restore our normal viewport size to our screen width and height
//     // glViewport(0, 0, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);
// 
//     // Go back to the projection mode and restore the original matrix
//     glMatrixMode ( GL_PROJECTION );
// 
//     // Restore the original projection matrix
//     glPopMatrix();
// 
//     // Go back to modelview model to start drawing like normal
//     glMatrixMode ( GL_MODELVIEW );
// 
//     // Clear the color and depth bits and start over from the camera's view
//     glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//     glLoadIdentity();
//}

}