// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <engine/renderer.h>

#include <SDL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


void static* library;
extern SDL_Window* sdl_window;
SDL_GLContext static gl_context;
FT_Library static freetype_library;

#pragma region OpenGL Loader

// OpenGL Types
using GLboolean   = std::uint8_t;
using GLchar      = char;
using GLushort    = std::uint16_t;
using GLsizei     = std::int32_t;
using GLint       = std::int32_t;
using GLfloat     = float;
using GLenum      = std::uint32_t;
using GLuint      = std::uint32_t;
using GLbitfield  = std::uint32_t;
using GLclampf    = float;
using GLvoid      = void;
using GLsizeiptr  = std::int64_t;
using GLintptr    = std::int64_t;

// OpenGL Constants
#define GL_FALSE                        0
#define GL_TRUE                         1

#define GL_RGBA					        0x1908
#define GL_RGBA8				        0x8058

// Data types
#define GL_FLOAT                        0x1406
#define GL_UNSIGNED_INT				    0x1405
#define GL_UNSIGNED_BYTE			    0x1401
#define GL_UNSIGNED_SHORT			    0x1403

// Color
#define GL_ALPHA				        0x1906

// Blending
#define GL_BLEND                        0x0BE2
#define GL_SRC_ALPHA                    0x0302
#define GL_ONE_MINUS_SRC_ALPHA          0x0303

// Buffer and Clear
#define GL_COLOR_BUFFER_BIT             0x00004000

// Buffer Handling
#define GL_FRAMEBUFFER                  0x8D40
#define GL_ARRAY_BUFFER                 0x8892
#define GL_ELEMENT_ARRAY_BUFFER         0x8893
#define GL_COLOR_ATTACHMENT0            0x8CE0
#define GL_FRAMEBUFFER_COMPLETE         0x8CD5

// Drawing
#define GL_STATIC_DRAW                  0x88E4
#define GL_STREAM_DRAW                  0x88E0

// Primitives
#define GL_TRIANGLES				    0x0004

// Texture Mapping and Filtering
#define GL_REPEAT				        0x2901
#define GL_NEAREST				        0x2600
#define GL_TEXTURE_2D				    0x0DE1
#define GL_CLAMP_TO_EDGE			    0x812F
#define GL_TEXTURE_WRAP_S			    0x2802
#define GL_TEXTURE_WRAP_T			    0x2803
#define GL_TEXTURE_MAG_FILTER			0x2800
#define GL_TEXTURE_MIN_FILTER			0x2801
#define GL_NEAREST_MIPMAP_NEAREST		0x2700

// Shader Compilation and Program Linking
#define GL_LINK_STATUS                  0x8B82
#define GL_COMPILE_STATUS               0x8B81
#define GL_INFO_LOG_LENGTH              0x8B84
#define GL_FRAGMENT_SHADER              0x8B30
#define GL_VERTEX_SHADER                0x8B31

// OpenGL Errors
#define GL_NO_ERROR                     0
#define GL_INVALID_ENUM                 0x0500
#define GL_INVALID_VALUE                0x0501
#define GL_INVALID_OPERATION            0x0502
#define GL_STACK_OVERFLOW               0x0503
#define GL_STACK_UNDERFLOW              0x0504
#define GL_OUT_OF_MEMORY                0x0505

auto static gl_error_string(GLenum error) -> char const* {
    switch (error) {
        case GL_NO_ERROR: return "No error";
        case GL_INVALID_ENUM: return "Invalid enum";
        case GL_INVALID_VALUE: return "Invalid value";
        case GL_INVALID_OPERATION: return "Invalid operation";
        case GL_STACK_OVERFLOW: return "Stack overflow";
        case GL_STACK_UNDERFLOW: return "Stack underflow";
        case GL_OUT_OF_MEMORY: return "Out of memory";
        default: return "Unknown error";
    }
}

#ifndef NDEBUG
#define GL_CALL(expr) \
    do { \
        while (glGetError() != GL_NO_ERROR); \
        expr; \
        GLenum error = glGetError(); \
        if (error != GL_NO_ERROR) \
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "OpenGL error at %s:%d: %s", __FILE__, __LINE__, gl_error_string(error)); \
    } while (0)
#else
#define GL_CALL(expr) expr
#endif

// OpenGL Extensions
#define GL_EXTENSION_LIST \
GL_EXTENSION(GLenum, GetError, void) \
/* Capability control */ \
GL_EXTENSION(void, Enable, GLenum cap) \
GL_EXTENSION(void, Disable, GLenum cap) \
GL_EXTENSION(void, BlendFunc, GLenum sfactor, GLenum dfactor) \
/* Rendering Setup */ \
GL_EXTENSION(void, Viewport, GLint x, GLint y, GLsizei width, GLsizei height) \
/* Buffer and Clearing */ \
GL_EXTENSION(void, Clear, GLbitfield mask) \
GL_EXTENSION(void, ClearColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) \
/* Framebuffer */ \
GL_EXTENSION(void, GenFramebuffers, GLsizei n, GLuint *ids) \
GL_EXTENSION(void, BindFramebuffer, GLenum target, GLuint framebuffer)        \
GL_EXTENSION(void, FramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
GL_EXTENSION(GLenum, CheckFramebufferStatus, GLenum target) \
/* Vertex array handling */ \
GL_EXTENSION(void, GenVertexArrays, GLsizei n, GLuint *arrays) \
GL_EXTENSION(void, DeleteVertexArrays, GLsizei n, const GLuint *arrays) \
/* Buffer handling */ \
GL_EXTENSION(void, GenBuffers, GLsizei n, GLuint *buffers) \
GL_EXTENSION(void, DeleteBuffers, GLsizei n, const GLuint *buffers) \
/* Buffer binding and data management */ \
GL_EXTENSION(void, BindBuffer, GLenum target, GLuint buffer) \
GL_EXTENSION(void, BufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data) \
GL_EXTENSION(void, BufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage) \
/* Vertex attribute array handling */  \
GL_EXTENSION(void, BindVertexArray, GLuint array) \
GL_EXTENSION(void, EnableVertexAttribArray, GLuint index) \
GL_EXTENSION(void, VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) \
/* Texture handling */ \
GL_EXTENSION(void, GenTextures, GLsizei n, GLuint * textures) \
GL_EXTENSION(void, DeleteTextures, GLsizei n, const GLuint *textures) \
GL_EXTENSION(void, BindTexture, GLenum target, GLuint texture) \
GL_EXTENSION(void, TexParameteri, GLenum target, GLenum pname, GLint param) \
GL_EXTENSION(void, TexImage2D, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) \
GL_EXTENSION(void, GenerateMipmap, GLenum target) \
/* Rendering with vertex indices */ \
GL_EXTENSION(void, DrawElements, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) \
/* Shader Compilation */ \
GL_EXTENSION(GLuint, CreateShader, GLenum shaderType) \
GL_EXTENSION(void, ShaderSource, GLuint shader, GLsizei count, const GLchar** string, const GLint* length) \
GL_EXTENSION(void, CompileShader, GLuint shader) \
GL_EXTENSION(void, GetShaderiv, GLuint shader, GLenum pname, GLint* params) \
GL_EXTENSION(void, GetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog) \
GL_EXTENSION(void, DeleteShader, GLuint shader) \
/* Program Creation and Linking */ \
GL_EXTENSION(GLuint, CreateProgram) \
GL_EXTENSION(void, AttachShader, GLuint program, GLuint shader) \
GL_EXTENSION(void, DetachShader, GLuint program, GLuint shader) \
GL_EXTENSION(void, LinkProgram, GLuint program) \
GL_EXTENSION(void, BindAttribLocation, GLuint program, GLuint index, const GLchar* name) \
GL_EXTENSION(void, GetProgramiv, GLuint program, GLenum pname, GLint* params) \
GL_EXTENSION(void, GetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog) \
GL_EXTENSION(void, DeleteProgram, GLuint program) \
GL_EXTENSION(void, UseProgram, GLuint program) \
/* Shader Uniforms */ \
GL_EXTENSION(GLint, GetUniformLocation, GLuint program, const GLchar *name) \
GL_EXTENSION(void, UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \

#define GL_EXTENSION(ret, name, ...) using PFN_gl##name = ret (__VA_ARGS__); PFN_gl##name static* gl##name;
GL_EXTENSION_LIST
#undef GL_EXTENSION

#pragma endregion OpenGL Loader


auto static create_context() -> bool {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    gl_context = SDL_GL_CreateContext(sdl_window);
    if (!gl_context) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s", SDL_GetError());
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    SDL_GL_LoadLibrary(nullptr);

    #define GL_EXTENSION(ret, name, ...) \
    gl##name = reinterpret_cast<PFN_gl##name*>(SDL_GL_GetProcAddress("gl"#name)); \
    if (gl##name == nullptr) { \
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Failed to load GL extension: %s", "gl"#name); \
        return false; \
    }
    GL_EXTENSION_LIST
    #undef GL_EXTENSION

    return true;
}


#pragma region Renderer Structs
struct vertex_t { xc::vector2_t position, texcoord; };
#pragma endregion Renderer Structs


#pragma region Renderer Constants
auto static constexpr MAX_BATCHES = 128u;
auto static constexpr INDICES_PER_QUAD = 6u;
auto static constexpr VERTICES_PER_QUAD = 4u;
auto static constexpr MAX_INDICES = INDICES_PER_QUAD * MAX_BATCHES;
auto static constexpr MAX_VERTICES = VERTICES_PER_QUAD * MAX_BATCHES;
#pragma endregion Renderer Constants


#pragma region Renderer Locals
vertex_t vertices[MAX_VERTICES];
std::uint32_t num_vertices;

std::uint32_t vbo, ibo, vao, fbo, shader;
std::uint32_t bound_texture, fbo_texture, batch_count;

FT_Face static default_face;

xc::vector2_t static scene_camera;
#pragma endregion Renderer Locals


auto static set_view_bounds(float width, float height) -> void {
    xc::renderer::bind_shader(shader);
    xc::renderer::set_shader_uniform(shader, "projection", xc::orthographic(0.f, width, 0.f, height, -1.f, 1.f));
}

auto static flush() -> void {
    if (num_vertices == 0u) return;

    GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
    GL_CALL(glDrawElements(GL_TRIANGLES, batch_count * INDICES_PER_QUAD, GL_UNSIGNED_SHORT, nullptr));

    num_vertices = 0u;
    batch_count = 0u;
}


namespace xc::renderer {
    auto initialize(std::uint32_t width, std::uint32_t height) -> bool {
        if (!create_context()) return false;

        // Load Freetype and default font face
        if (FT_Init_FreeType(&freetype_library)) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Could not load the Freetype Library");
            return false;
        }

        if (FT_New_Face(freetype_library, "../assets/5px.ttf", 0, &default_face)) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Could not load default font face");
            return false;
        }
        FT_Set_Pixel_Sizes(default_face, 0, 5);

        // Load Shader
        shader = create_shader("../assets/shaders/vertex.vsh", "../assets/shaders/fragment.fsh");

        // Setup modes and projection
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        set_view_bounds(static_cast<float>(width), static_cast<float>(height));

        // Create quad Geometry
        std::uint16_t indices[MAX_INDICES];
        for (auto i = 0u; i < MAX_BATCHES; ++i) {
            indices[i * 6u + 0u] = i * VERTICES_PER_QUAD + 0u;
            indices[i * 6u + 1u] = i * VERTICES_PER_QUAD + 1u;
            indices[i * 6u + 2u] = i * VERTICES_PER_QUAD + 2u;
            indices[i * 6u + 3u] = i * VERTICES_PER_QUAD + 2u;
            indices[i * 6u + 4u] = i * VERTICES_PER_QUAD + 1u;
            indices[i * 6u + 5u] = i * VERTICES_PER_QUAD + 3u;
        }


        GL_CALL(glGenVertexArrays(1, &vao));
        GL_CALL(glBindVertexArray(vao));

        GL_CALL(glGenBuffers(1, &vbo));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), nullptr, GL_STREAM_DRAW));

        GL_CALL(glGenBuffers(1, &ibo));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

        auto position_attribute = 0u;
        GL_CALL(glEnableVertexAttribArray(position_attribute));
        GL_CALL(glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), reinterpret_cast<void*>(0)));

        auto texcoord_attribute = 1u;
        GL_CALL(glEnableVertexAttribArray(texcoord_attribute));
        GL_CALL(glVertexAttribPointer(texcoord_attribute, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), reinterpret_cast<void*>(2 * sizeof(float))));



        /*
        // Generate and bind FBO
        GL_CALL(glGenFramebuffers(1, &fbo));
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        // Generate and attach a texture to the FBO
        GL_CALL(glGenTextures(1, &fbo_texture));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, fbo_texture));
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0));


         // Check FBO status
         if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
             SDL_LogError(SDL_LOG_CATEGORY_RENDER, "FBO Error");
         }

         glBindFramebuffer(GL_FRAMEBUFFER, 0);
         */

        return true;
    }

    auto uninitialize() -> void {
        FT_Done_Face(default_face);
        FT_Done_FreeType(freetype_library);
        destroy_shader(shader);
        glDisable(GL_BLEND);
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        // delete framebuffer
    }

    auto begin() -> void {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, 64,  64);
    }

    auto end() -> void {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        glViewport(0, 0, 640, 640);

        draw_texture({fbo_texture, 64, 64}, {0u, 0u, 64u, 64u}, {0.f, 0.f}, 0.f, 1.f);
    }

    auto tick() -> void {
        swap();
    }

    auto clear(color_t color) -> void {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    auto swap() -> void {
        SDL_GL_SwapWindow(sdl_window);
    }

    auto set_camera(vector2_t camera) -> void {
        scene_camera = camera;
    }

    auto create_shader(char const* vertex_shader_path, char const* fragment_shader_path) -> std::uint32_t {
        auto compile_shader = [](GLenum type, char const* shader_path) {
            auto* file = fopen(shader_path, "r");
            if (!file) {
                SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error: Unable to open shader_program file: %s\n", shader_path);
                return 0u;
            }

            fseek(file, 0, SEEK_END);
            auto file_size = ftell(file);
            fseek(file, 0, SEEK_SET);

            auto* shader_source = reinterpret_cast<char*>(malloc(file_size + 1));
            if (!shader_source) {
                SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error: Unable to allocate memory for shader_program source.\n");
                fclose(file);
                return 0u;
            }

            fread(shader_source, 1, file_size, file);
            shader_source[file_size] = '\0';

            fclose(file);

            auto shader = glCreateShader(type);
            auto const_shader_source = reinterpret_cast<char const*>(shader_source);
            glShaderSource(shader, 1, &const_shader_source, nullptr);
            glCompileShader(shader);
            free(shader_source);

            auto compile_status = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
            if (compile_status == GL_FALSE) {
                GLint info_log_length;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
                auto* info_log = new GLchar[info_log_length];
                glGetShaderInfoLog(shader, info_log_length, nullptr, info_log);
                printf("%s", info_log);
                delete[] info_log;

                glDeleteShader(shader);
                return 0u;
            }

            return shader;
        };

        auto shader_program = glCreateProgram();
        auto vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_path);
        auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_path);

        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);

        auto link_status = 0;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
        if (link_status == GL_FALSE) {
            GLint info_log_length;
            glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_log_length);
            auto* info_log = new GLchar[info_log_length];
            glGetProgramInfoLog(shader_program, info_log_length, nullptr, info_log);
            printf("%s", info_log);
            delete[] info_log;

            glDetachShader(shader_program, vertex_shader);
            glDetachShader(shader_program, fragment_shader);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            glDeleteProgram(shader_program);
        }

        glDetachShader(shader_program, vertex_shader);
        glDetachShader(shader_program, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return shader_program;
    }

    auto destroy_shader(std::uint32_t shader) -> void {
        glDeleteProgram(shader);
    }

    auto bind_shader(std::uint32_t shader) -> void {
        GL_CALL(glUseProgram(shader));
    }

    auto set_shader_uniform(std::uint32_t shader, char const* name, matrix4_t value) -> void {
        GL_CALL(glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, reinterpret_cast<float*>(&value)));
    }

    auto create_texture(char const* path) -> texture_t {
        int width, height, components;
        auto* image = stbi_load(path, &width, &height, &components, STBI_rgb_alpha);
        if (!image) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", stbi_failure_reason());
            return {};
        }

        auto texture = texture_t{0u, static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        GL_CALL(glGenTextures(1, &texture.id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.id));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image));
        GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        stbi_image_free(image);

        return texture;
    }

    auto destroy_texture(texture_t texture) -> void {
        glDeleteTextures(1, &texture.id);
    }

    auto bind_texture(texture_t texture) -> void {
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    auto draw_texture(texture_t texture, rectangle_t region, vector2_t position, float angle, float scale) -> void {
        if (bound_texture != texture.id) {
            flush();
            bind_texture(texture);
            bound_texture = texture.id;
        }

        position = position - (vector2_t(region.w, region.h) / 2.f);
        matrix3_t mvp = identity_t{};

        // Scale
        mvp = mvp * scaling_matrix(vector2_t{scale, scale});

        // Rotate
        mvp = mvp * translation_matrix(vector2_t{-(float)region.w / 2.f, -(float)region.h / 2.f}); // translate to origin
        mvp = mvp * rotation_matrix(angle * DEG2RAD);
        mvp = mvp * translation_matrix(vector2_t{(float)region.w / 2.f, (float)region.h / 2.f}); // translate back

        // Translate
        mvp = mvp * translation_matrix(vector2_t{position.x, position.y});

        // Camera
        mvp = mvp * translation_matrix(scene_camera);

        auto left_uv = (float)region.x / (float)texture.w;
        auto right_uv = (float)(region.x + region.w) / (float)texture.w;
        auto top_uv = (float)region.y / (float)texture.h;
        auto bottom_uv = (float)(region.y + region.h) / (float)texture.h;

        auto x = mvp * vector3_t{0.f, 0.f, 1.f};
        vertices[num_vertices++] = vertex_t{{x.x, x.y}, {left_uv, top_uv}};

        x = mvp * vector3_t{static_cast<float>(region.w), 0.f, 1.f};
        vertices[num_vertices++] = vertex_t{{x.x, x.y}, {right_uv, top_uv}};

        x = mvp * vector3_t{0.f, static_cast<float>(region.h), 1.f};
        vertices[num_vertices++] = vertex_t{{x.x, x.y}, {left_uv, bottom_uv}};

        x = mvp * vector3_t{static_cast<float>(region.w), static_cast<float>(region.h), 1.f};
        vertices[num_vertices++] = vertex_t{{x.x, x.y}, {right_uv, bottom_uv}};

        batch_count += 1u;
    }



    auto create_font(char const* path) -> font_t {
        auto font = font_t{};
        return font;
    }

    auto draw_text(char const* text, font_t font, vector2_t position) -> void {

    }
} // namespace xc::renderer
