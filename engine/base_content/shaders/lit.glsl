#version 330

#ifdef VERT
    layout(location = 0) in vec3 _vertex;
    layout(location = 1) in vec3 _normal;
    layout(location = 2) in vec2 _uv;

    uniform mat4 SILICA_MVP;
    uniform mat4 SILICA_M;
    uniform mat4 SILICA_M_IT;

    out vec3 world_pos;
    out vec3 world_normal;

    void main() {
        gl_Position = SILICA_MVP * vec4(_vertex, 1.0);

        world_pos = (SILICA_M * vec4(_vertex, 1.0)).xyz;
        world_normal = (SILICA_M_IT * vec4(_normal, 1.0)).xyz;
    }
#endif

#ifdef FRAG
    out vec4 col;

    uniform vec3 SILICA_CAM_POS;

    in vec3 world_pos;
    in vec3 world_normal;

    void main() {
        vec3 normal = normalize(world_normal);
        vec3 view = normalize(SILICA_CAM_POS - world_pos);
        vec3 light = normalize(vec3(0, 1, 0));
        vec3 halfway = normalize(view + light);

        vec3 color = vec3(1, 1, 1);

        float NDotV = max(0.0, dot(normal, view));
        float NDotL = max(0.0, dot(normal, light));
        float NDotH = max(0.0, dot(normal, halfway));

        vec3 diffuse = color * NDotL;
        vec3 specular = color * pow(NDotH, 512);

        col = vec4(diffuse + specular, 1.0);
    }
#endif