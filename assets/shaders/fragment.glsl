#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightPos2; // Secondary light
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightColor2; // Secondary light color
uniform vec3 objectColor;
uniform sampler2D texture1;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    // Primary light
    {
        // Ambient
        float ambientStrength = 0.6; // Increased for much better visibility
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * 1.5; // Boost diffuse

        // Specular
        float specularStrength = 0.5; // Moderate specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;

        // Attenuation
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (1.0 + 0.05 * distance + 0.01 * distance * distance); // Softer attenuation
        diffuse *= attenuation;
        specular *= attenuation;

        result += (ambient + diffuse + specular);
    }

    // Secondary light (fill light)
    {
        // Ambient
        float ambientStrength = 0.4;
        vec3 ambient = ambientStrength * lightColor2;

        // Diffuse
        vec3 lightDir = normalize(lightPos2 - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor2 * 1.2;

        // Specular
        float specularStrength = 0.3;
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
        vec3 specular = specularStrength * spec * lightColor2;

        // Attenuation
        float distance = length(lightPos2 - FragPos);
        float attenuation = 1.0 / (1.0 + 0.05 * distance + 0.01 * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;

        result += (ambient + diffuse + specular);
    }

    result *= objectColor;
    // Handle missing texture gracefully
    vec4 texColor = vec4(1.0); // Default white if no texture
    // Uncomment below when texture is loaded:
    // texColor = texture(texture1, TexCoord);
    FragColor = vec4(result, 1.0) * texColor;
}