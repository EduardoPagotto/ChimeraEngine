========================================================================
    STATIC LIBRARY : chimeraLib Project Overview
========================================================================

AppWizard has created this chimeraLib library project for you.

No source files were created as part of your project.


chimeraLib.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

chimeraLib.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

Includes:
../../chimera/include
C:\Libs\SDL2-2.0.4\include
C:\Libs\SDL2_image-2.0.1\include
C:\Libs\SDL2_ttf-2.0.14\include
C:\Libs\bullet3\src
C:\libs\tinyxml2
C:\Libs\OculusSDK\LibOVR\Include
C:\Libs\glew-1.13.0\include

Ao compilar com suporte ao Rift
mudar de:
NO_OVR_SET_TO_USE

para:
OVR_SET_TO_USE


mapa Joystick Xbox360
A Button			0	
B Button			1
X Button			2
Y Button			3
Left Bumper			4
Right Bumper		5
Back Button			6
Start Button		7
Left Stick Click	8
Right Stick Click	9
D-Pad Up			hat 0x01h	
D-Pad Down		    hat 0x04h
D-Pad Left		    hat 0x02h
D-Pad Right		    hat 0x08h
Xbox Button         -

Referencias Site:
http://www.kickjs.org/example/shader_editor/shader_editor.html : template para usar
http://www.lighthouse3d.com/tutorials/glsl-tutorial/attribute-variables/ : como carregar
https://cognitivewaves.wordpress.com/opengl-vbo-shader-vao/ : a resposta aqui!!!!! matrix de model e projecao
http://learnopengl.com/#!Getting-started/Camera : Mudanca radical no tratamento de camera e objeto para uso de GLSL 

	//  Model::Draw(ICamera camera) {
	//     GLuint matrixID = glGetUniformLocation(programID, "mvp");
	//     GLuint positionID = glGetAttribLocation(programID, "position_modelspace");
	//     GLuint uvID = glGetAttribLocation(programID, "uv");
	//     GLuint normalID = glGetAttribLocation(programID, "normal_modelspace");
	//     GLuint tangentID = glGetAttribLocation(programID, "tangent_modelspace");
	//     GLuint bitangentID = glGetAttribLocation(programID, "bitangent_modelspace");
	//
	//     glm::mat4 projection = camera->GetProjectionMatrix();
	//     glm::mat4 view = camera->GetViewMatrix();
	//     glm::mat4 model = glm::mat4(1.0f);
	//     glm::mat4 mvp = projection * view * model;
	//
	//     glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	//


 ///
// void teste() {
//     float pitch;
//     float yaw;
//     float roll;
//
//     glm::vec3 position(0,0,0);
//
//     //Euler to Quarterion pitch, yaw, roll angul em radianos
//     glm::vec3 myEuler (pitch, yaw, roll);//yaw, pitch, roll ?????
//     glm::quat myQuat (myEuler);
//
//     glm::mat4 matRot = glm::toMat4(myQuat); //matriz rotacao
//     glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), position); //matriz translacao
//     glm::mat4 final = matRot * matTrans; //primeiro translada depois rotaciona, ordem é importante!!!
// }
///