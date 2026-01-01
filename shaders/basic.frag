#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D baseColorTexture;
uniform vec4 baseColorFactor;
uniform bool hasTexture;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 viewPos;

void main() {
    vec4 baseColor = baseColorFactor;
    if (hasTexture) {
        baseColor *= texture(baseColorTexture, TexCoord);
    }

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);

    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * 0.3;

    vec3 result = (ambientColor + diffuse + specular) * baseColor.rgb;

    FragColor = vec4(result, baseColor.a);
}
