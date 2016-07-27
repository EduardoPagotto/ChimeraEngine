#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "Shader.h"
#include "ExceptionChimera.h"

namespace Chimera {

std::string getShaderCode ( const char * file_path ) {

    // Read the Vertex Shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream ( file_path, std::ios::in );
    if ( shaderStream.is_open() ) {
        std::string Line = "";
        while ( getline ( shaderStream, Line ) ) {
            shaderCode += "\n" + Line;
        }

        shaderStream.close();

    } else {
        throw  ExceptionChimera ( ExceptionCode::OPEN, "Impossivel abrir arquivo: " + std::string ( file_path ) );
    }

    return shaderCode;
}

GLuint compileShader ( const std::string &shaderCode, bool _shadeKind ) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint shaderID = -1;
    if (_shadeKind == false)
        shaderID = glCreateShader ( GL_FRAGMENT_SHADER );
    else
        shaderID = glCreateShader ( GL_VERTEX_SHADER );
    
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource ( shaderID, 1, &sourcePointer, NULL );
    glCompileShader ( shaderID );

    // Check Fragment Shader
    glGetShaderiv ( shaderID, GL_COMPILE_STATUS, &Result );
    glGetShaderiv ( shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
    if ( InfoLogLength > 0 ) {
        std::vector<char> shaderErrorMessage ( InfoLogLength + 1 );
        glGetShaderInfoLog ( shaderID, InfoLogLength, NULL, &shaderErrorMessage[0] );

        std::cout << "Check Fragment Shader " << std::string ( &shaderErrorMessage[0] ) << std::endl;
    }

    return shaderID;
}

GLuint linkShader (const GLuint &VertexShaderID, const GLuint &FragmentShaderID ) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint ProgramID = glCreateProgram();
    glAttachShader ( ProgramID, VertexShaderID );
    glAttachShader ( ProgramID, FragmentShaderID );
    glLinkProgram ( ProgramID );

    // Check the program
    glGetProgramiv ( ProgramID, GL_LINK_STATUS, &Result );
    glGetProgramiv ( ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength );
    if ( InfoLogLength > 0 ) {
        std::vector<char> ProgramErrorMessage ( InfoLogLength + 1 );
        glGetProgramInfoLog ( ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0] );

        std::cout << "Check program " << std::string ( &ProgramErrorMessage[0] ) << std::endl;
    }

    return ProgramID;
}

GLuint LoadShaders ( const char * vertex_file_path, const char * fragment_file_path ) {

    std::string VertexShaderCode = getShaderCode ( vertex_file_path );
    std::string FragmentShaderCode = getShaderCode ( fragment_file_path );

    // Compile Vertex Shader
    std::cout << "Compiling Vertex Shader :" << vertex_file_path << std::endl;
    GLuint VertexShaderID = compileShader(VertexShaderCode, true);
    
    // Compile Fragment Shader
    std::cout << "Compiling Fragment Shader : " << std::string ( fragment_file_path ) << std::endl;
    GLuint FragmentShaderID = compileShader(FragmentShaderCode, false);

    // Link the program
    std::cout << "Linking program " << std::endl;
    GLuint ProgramID = linkShader ( VertexShaderID, FragmentShaderID );

    glDeleteShader ( VertexShaderID );
    glDeleteShader ( FragmentShaderID );

    return ProgramID;
}

// GLuint compileVertexShader ( const std::string &VertexShaderCode ) {
// 
//     GLint Result = GL_FALSE;
//     int InfoLogLength;
// 
//     // Create the shaders
//     GLuint VertexShaderID = glCreateShader ( GL_VERTEX_SHADER );
//     char const * VertexSourcePointer = VertexShaderCode.c_str();
//     glShaderSource ( VertexShaderID, 1, &VertexSourcePointer, NULL );
//     glCompileShader ( VertexShaderID );
// 
//     // Check Vertex Shader
//     glGetShaderiv ( VertexShaderID, GL_COMPILE_STATUS, &Result );
//     glGetShaderiv ( VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
//     if ( InfoLogLength > 0 ) {
//         std::vector<char> VertexShaderErrorMessage ( InfoLogLength + 1 );
//         glGetShaderInfoLog ( VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0] );
// 
//         std::cout << "Check Vertex Shader " << std::string ( &VertexShaderErrorMessage[0] ) << std::endl;
//     }
// 
//     return VertexShaderID;
// }
// 
// GLuint compileFragmentShader ( const std::string &FragmentShaderCode ) {
// 
//     GLint Result = GL_FALSE;
//     int InfoLogLength;
// 
//     GLuint FragmentShaderID = glCreateShader ( GL_FRAGMENT_SHADER );
//     char const * FragmentSourcePointer = FragmentShaderCode.c_str();
//     glShaderSource ( FragmentShaderID, 1, &FragmentSourcePointer, NULL );
//     glCompileShader ( FragmentShaderID );
// 
//     // Check Fragment Shader
//     glGetShaderiv ( FragmentShaderID, GL_COMPILE_STATUS, &Result );
//     glGetShaderiv ( FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
//     if ( InfoLogLength > 0 ) {
//         std::vector<char> FragmentShaderErrorMessage ( InfoLogLength + 1 );
//         glGetShaderInfoLog ( FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0] );
// 
//         std::cout << "Check Fragment Shader " << std::string ( &FragmentShaderErrorMessage[0] ) << std::endl;
//     }
// 
//     return FragmentShaderID;
// }

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
