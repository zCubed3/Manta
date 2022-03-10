#version 330

#ifdef VERT
    layout(location = 0) in vec3 _vertex;
    layout(location = 1) in vec3 _normal;
    layout(location = 2) in vec2 _uv;

    uniform mat4 SILICA_MVP;

    out vec3 normal;

    void main() {
        gl_Position = SILICA_MVP * vec4(_vertex, 1.0);
        normal = _normal;
    }
#endif

#ifdef FRAG
    out vec4 col;

    in vec3 normal;

    void main() {
        col = vec4(normalize(normal), 1.0);
    }
#endif