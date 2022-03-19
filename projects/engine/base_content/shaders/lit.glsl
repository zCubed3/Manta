#version 330

#ifdef VERT
    layout(location = 0) in vec3 _vertex;
    layout(location = 1) in vec3 _normal;
    layout(location = 2) in vec2 _uv;

    uniform mat4 MANTA_MVP;
    uniform mat4 MANTA_M;
    uniform mat4 MANTA_M_IT;

    out vec3 world_pos;
    out vec3 world_normal;

    void main() {
        gl_Position = MANTA_MVP * vec4(_vertex, 1.0);

        world_pos = (MANTA_M * vec4(_vertex, 1.0)).xyz;
        world_normal = (MANTA_M_IT * vec4(_normal, 0.0)).xyz;
    }
#endif

#ifdef FRAG
    out vec4 col;

    uniform vec3 MANTA_CAM_POS;
    uniform vec4 MANTA_SINTIME;
    uniform vec4 MANTA_COSTIME;
    uniform vec4 MANTA_TANTIME;

    in vec3 world_pos;
    in vec3 world_normal;

    const float PI = 3.14159265359;

    float BlinnPhongSpecular(vec3 N, vec3 V, vec3 L, float power) {
        vec3 H = normalize(V + L);
        return pow(max(0.0, max(0.0, dot(N, H))), power);
    }

    vec3 FresnelSchlick(float cosTheta, vec3 F0) {
        return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
    }

    float DistributionGGX(vec3 N, vec3 H, float roughness) {
        float a      = roughness*roughness;
        float a2     = a*a;
        float NdotH  = max(dot(N, H), 0.0);
        float NdotH2 = NdotH*NdotH;

        float num   = a2;
        float denom = (NdotH2 * (a2 - 1.0) + 1.0);
        denom = PI * denom * denom;

        return num / denom;
    }

    float GeometrySchlickGGX(float NdotV, float roughness) {
        float r = (roughness + 1.0);
        float k = (r*r) / 8.0;

        float num   = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return num / denom;
    }

    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2  = GeometrySchlickGGX(NdotV, roughness);
        float ggx1  = GeometrySchlickGGX(NdotL, roughness);

        return ggx1 * ggx2;
    }

    void main() {
        vec3 normal = normalize(world_normal);
        vec3 view = normalize(MANTA_CAM_POS - world_pos);
        vec3 light = normalize(vec3(0.5, 1, 0.5));
        vec3 halfway = normalize(view + light);

        vec3 color = vec3(1, 1, 1);
        color = normal;

        float metallic = abs(MANTA_SINTIME.x);
        float roughness = 0.1;

        vec3 F0 = vec3(0.04);
        F0 = mix(F0, color, metallic);

        vec3 F = FresnelSchlick(max(dot(halfway, view), 0.0), F0);
        float NDF = DistributionGGX(normal, halfway, roughness);
        float G = GeometrySmith(normal, view, light, roughness);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, view), 0.0) * max(dot(normal, light), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;

        kD *= 1.0 - metallic;

        vec3 diffuse = color * max(0.0, dot(normal, light));
        float NDotL = max(dot(normal, light), 0.0);

        vec3 final_color = ((kD * color / PI + specular) * NDotL);

        col = vec4(diffuse + specular, 1.0);
        col.rgb = max(vec3(0.0), min(vec3(1.0), col.rgb));
    }
#endif