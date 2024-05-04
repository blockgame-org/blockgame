#include <epoxy/gl.h>

#include <engine.h>

#include <Graphics/camera.h>
#include <Basic/file.h>
#include <Basic/math.h>
#include <Input/mouse.h>
#include <Graphics/shader.h>
#include <Graphics/texture2d.h>
#include <Basic/utility.h>
#include <Graphics/vertex.h>
#include <Graphics/window.h>

#include <Graphics/sdl_utility.h>

bgEngine *BG_ENGINE = NULL;

void destroyEngine_(void) {
    free(BG_ENGINE);
    BG_ENGINE = NULL;
}

int bg_initEngine(void) {
    assertSDL_("Failed to initialize SDL2", 0 == SDL_Init(SDL_INIT_VIDEO));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                        SDL_GL_CONTEXT_PROFILE_CORE));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8));
    assertSDL_("Failed to set GL attribute",
               0 == SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16));

    // TODO: Temporary dimensions
    bg_initWindow((bgVec2u){1080, 720});
    bg_initMouse();

    BG_ENGINE = bg_calloc(1, sizeof(bgEngine));
    BG_ENGINE->running = 0;

    atexit(destroyEngine_);
    atexit(SDL_Quit);

    return 0;
}

int bgEngine_run(void) {
    BG_ENGINE->running = 1;

    //
    // Setup shader program
    //

    bgProgram prog;
    bgShader vs;
    bgShader fs;
    char *vsContent;
    char *fsContent;

    bg_readFile(&vsContent, "res/shaders/main.vert");
    bg_readFile(&fsContent, "res/shaders/main.frag");

    bg_vertexShader(&vs, "res/shaders/main.vert", vsContent, strlen(vsContent));
    bg_fragmentShader(&fs, "res/shaders/main.frag", fsContent,
                      strlen(fsContent));

    bg_program(&prog, (bgShader[2]){vs, fs}, 2, NULL, 0);

    bgShader_free(&vs);
    bgShader_free(&fs);

    // NOTE: We dont really need this since their locations
    // are hardcoded in <vertex.h>
    int m = bgProgram_getUniformId(&prog, "model");
    int v = bgProgram_getUniformId(&prog, "view");
    int p = bgProgram_getUniformId(&prog, "projection");

    //
    // Load textures
    //

    int tex = bgProgram_getUniformId(&prog, "tex");
    bgTexture2D texture;
    bg_readTexture2D(&texture, "res/textures/grass.png", GL_REPEAT, GL_NEAREST,
                     0);
    bgTexture2D_use(&texture);
    bgProgram_setUniform1i(&prog, tex, texture.id);

    //
    // Setup VBO and VAO
    //

    unsigned int vbo;
    unsigned int vao;
    unsigned int ibo;

    bgVertex vertices[] = {
        (bgVertex){
            .position = {-1., -1., 1.}, .normal = {0., 0., 0.}, .uv = {0., 0.}},
        (bgVertex){
            .position = {1., -1., 1.}, .normal = {0., 0., 0.}, .uv = {1., 0.}},
        (bgVertex){
            .position = {-1., 1., 1.}, .normal = {0., 0., 0.}, .uv = {0., 1.}},
        (bgVertex){
            .position = {1., 1., 1.}, .normal = {0., 0., 0.}, .uv = {1., 1.}},
        (bgVertex){.position = {-1., -1., -1.},
                   .normal = {0., 0., 0.},
                   .uv = {0., 0.}},
        (bgVertex){
            .position = {1., -1., -1.}, .normal = {0., 0., 0.}, .uv = {1., 0.}},
        (bgVertex){
            .position = {-1., 1., -1.}, .normal = {0., 0., 0.}, .uv = {0., 1.}},
        (bgVertex){
            .position = {1., 1., -1.}, .normal = {0., 0., 0.}, .uv = {1., 1.}},
    };

    uint32_t indices[] = {0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1};

    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, sizeof(vertices), vertices,
                         GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &vao);
    bgVertex_configureVaoVbo(vao, vbo, 0);

    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

    //
    // Setup rendering
    //

    bgCamera camera;
    bg_camera(&camera, 1080. / 720.);

    glViewport(0, 0, 1080, 720);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    bgProgram_use(&prog);

    //
    // Setup input
    //

    bgMouse_setRelativeMode(1);

    //
    // Engine loop
    //

    unsigned long nowTicks = SDL_GetTicks64();
    unsigned long lastTicks = 0;

    while (BG_ENGINE->running) {
        lastTicks = nowTicks;
        nowTicks = SDL_GetTicks64();

        // float dt = (nowTicks - lastTicks) / 1000.;

        //
        // Handle events
        //

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                BG_ENGINE->running = false;
                break;
            case SDL_KEYDOWN:
                switch (ev.key.keysym.sym) {
                case SDLK_ESCAPE:
                    BG_ENGINE->running = false;
                    break;
                case SDLK_a:
                    bgCamera_move(&camera, BG_CAMERA_LEFT);
                    break;
                case SDLK_d:
                    bgCamera_move(&camera, BG_CAMERA_RIGHT);
                    break;
                case SDLK_w:
                    bgCamera_move(&camera, BG_CAMERA_FORWARD);
                    break;
                case SDLK_s:
                    bgCamera_move(&camera, BG_CAMERA_BACKWARD);
                    break;
                }
                break;
            default:
                break;
            }
        }

        //
        // Update MVP
        //

        bgCamera_mouse(&camera);
        // bgCamera_step(&camera, dt);

        bgMat4 model;
        bgMat4 view;
        bgMat4 projection;

        bgMat4_translate(model, (bgVec3f){0., 0., -2.});
        bgMat4_scale(model, model, 1.);

        bgCamera_view(&camera, view);
        bgCamera_projection(&camera, projection);

        bgProgram_setUniformMat4(&prog, m, model);
        bgProgram_setUniformMat4(&prog, v, view);
        bgProgram_setUniformMat4(&prog, p, projection);

        //
        // Render scene
        //

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLE_STRIP, sizeof(indices) / sizeof(uint32_t),
                       GL_UNSIGNED_INT, NULL);

        bgWindow_swap();
    }

    return 0;
}