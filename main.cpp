#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengles2.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

struct AppContext
{
    SDL_Window *window;
    SDL_GLContext glContext;
    SDL_AppResult appQuit = SDL_APP_CONTINUE;
    GLuint program;
    GLuint textureID;
    float angle = 0.0f;
};

const char *vertexShaderSource =
        "attribute vec2 aPosition;\n"
        "attribute vec2 aTexCoord;\n"  // Coordenadas de textura
        "uniform mat3 uTransform;\n"
        "varying vec2 vTexCoord;\n"    // Variable para pasar las coordenadas al fragment shader
        "void main()\n"
        "{\n"
        "    vec3 pos = uTransform * vec3(aPosition, 1.0);\n"
        "    gl_Position = vec4(pos.xy, 0.0, 1.0);\n"
        "    vTexCoord = aTexCoord;\n"  // Pasamos las coordenadas de textura al fragment shader
        "}\n";

const char *fragmentShaderSource =
        "precision mediump float;\n"
        "uniform sampler2D uTexture;\n"  // Uniform para la textura
        "varying vec2 vTexCoord;\n"      // Coordenada de textura que viene del vertex shader
        "void main()\n"
        "{\n"
        "    gl_FragColor = texture2D(uTexture, vTexCoord);\n"  // Muestreamos la textura
        "}\n";

// Helper function for creating shaders
GLuint createShader(const char *shaderSource, int shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        glDeleteShader(shader); // Don't leak the shader
        std::cout << &(errorLog[0]) << std::endl;
        std::cout << shaderSource << std::endl;
    }
    return shader;
}

// Helper function for creating a shader program
GLuint createShaderProgram()
{
    GLuint program = glCreateProgram();
    GLuint vShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glUseProgram(program);

    return program;
}

// Load a square to the video card
void initVertexBuffers(GLuint program) {
    // Vértices de un cuadrado con coordenadas de textura (UV)
    float vertPositions[] = {
            // Posición (x, y)    // Coordenadas de textura (u, v)
            -0.5f, -0.5f, 0.0f, 0.0f,  // Esquina inferior izquierda
            0.5f, -0.5f, 1.0f, 0.0f,   // Esquina inferior derecha
            0.5f, 0.5f, 1.0f, 1.0f,    // Esquina superior derecha

            -0.5f, -0.5f, 0.0f, 0.0f,  // Esquina inferior izquierda
            0.5f, 0.5f, 1.0f, 1.0f,    // Esquina superior derecha
            -0.5f, 0.5f, 0.0f, 1.0f    // Esquina superior izquierda
    };

    GLuint vertPosBuffer;
    glGenBuffers(1, &vertPosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertPositions), vertPositions, GL_STATIC_DRAW);

    GLint aPositionLocation = glGetAttribLocation(program, "aPosition");
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(aPositionLocation);

    GLint aTexCoordLocation = glGetAttribLocation(program, "aTexCoord");
    glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(aTexCoordLocation);
}

// Función para crear una matriz de rotación 2D
void createRotationMatrix(float angle, float *matrix) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    matrix[0] = cosA;  matrix[1] = -sinA; matrix[2] = 0.0f;
    matrix[3] = sinA;  matrix[4] = cosA;  matrix[5] = 0.0f;
    matrix[6] = 0.0f;  matrix[7] = 0.0f;  matrix[8] = 1.0f;
}

GLuint loadTexture(const char *imagePath) {
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Cargar la imagen a la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Configurar el filtrado de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Liberar la superficie después de cargarla
    SDL_DestroySurface(surface);

    return texture;
}

SDL_AppResult SDL_Fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    // Inicializar SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_Fail();
    }

    // Inicializar OpenGL
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    // Crear ventana
    SDL_Window *window = SDL_CreateWindow("Rotating Square", 352, 430, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        return SDL_Fail();
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        return SDL_Fail();
    }

    glClearColor(0.188f, 0.22f, 0.255f, 1.f);

    // Crear el programa de shaders y cargar los vértices
    GLuint program = createShaderProgram();
    initVertexBuffers(program);

    // Cargar la textura
    SDL_Surface *surface = SDL_LoadBMP("image.bmp");
    SDL_Surface *convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ABGR8888);
    SDL_DestroySurface(surface);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurface->w, convertedSurface->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, convertedSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_DestroySurface(convertedSurface);


    //GLuint textureID = loadTexture("crate.png");
    if (!textureID) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());  // Error si no se carga la imagen
        //td::cout << "Error loading texture: " << std::endl;  // También imprimimos un mensaje en la consola
        //return ;  // Retorna 0 si hay un error al cargar la textura

    }

    // Inicializar el estado de la aplicación
    AppContext *app = new AppContext();
    app->window = window;
    app->glContext = glContext;
    app->program = program;
    app->textureID = textureID;  // Guardar el ID de la textura
    *appstate = app;

    SDL_ShowWindow(window);
    SDL_Log("Application started successfully!");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    auto *app = (AppContext *)appstate;

    switch (event->type)
    {
        case SDL_EVENT_QUIT:
        {
            app->appQuit = SDL_APP_SUCCESS;
            break;
        }
        default:
        {
            break;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto *app = (AppContext *)appstate;

    // Limpiar el buffer de color
    glClear(GL_COLOR_BUFFER_BIT);

    // Actualizar el ángulo de rotación
    app->angle += 0.01f; // Incrementar el ángulo
    if (app->angle > 2 * M_PI) {
        app->angle -= 2 * M_PI; // Mantener el ángulo en un rango razonable
    }

    // Crear la matriz de rotación
    float rotationMatrix[9];
    createRotationMatrix(app->angle, rotationMatrix);

    // Pasar la matriz de rotación al shader
    GLint transformLocation = glGetUniformLocation(app->program, "uTransform");
    glUniformMatrix3fv(transformLocation, 1, GL_FALSE, rotationMatrix);

    // Activar la textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app->textureID);  // Asumiendo que app->textureID es el ID de la textura

    // Pasar la textura al shader
    GLint texLocation = glGetUniformLocation(app->program, "uTexture");
    glUniform1i(texLocation, 0);  // Usar la textura activada en GL_TEXTURE0

    // Dibujar el cuadrado
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Intercambiar los buffers
    SDL_GL_SwapWindow(app->window);

    return app->appQuit;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    auto *app = (AppContext *)appstate;
    if (app)
    {
        SDL_GL_DestroyContext(app->glContext);
        SDL_DestroyWindow(app->window);
        delete app;
    }

    SDL_Quit();
    SDL_Log("Application quit successfully!");
}
