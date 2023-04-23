#version 330 core
out vec4 fragmentColour;     //color sent to fragment on screen

in vec3 fragmentPosition;
in vec3 normalTransposed;              //color received from the pipeline (and vertex shader)
in vec2 UV;         // for textures

uniform sampler2D textureSampler;

uniform float ambientStrength = 0.3;
uniform float lightStrength = 0.5;
uniform vec3 lightPosition = vec3(0.0f, 3.0f, 0.0f); // (0.0f, 3.0f, 0.0f), (-7.5f, 2.0f, 0.0f)
uniform vec3 lightColour = vec3(0.8, 0.8, 0.3);
uniform vec3 objectColour = vec3(0.7, 0.7, 0.7);

uniform vec3 cameraPosition;
uniform float specularStrength = 0.5;
uniform int specularExponent = 64;

void main() {
        // ambient
        vec3 ambient = ambientStrength * lightColour * objectColour;

        // diffuse
        vec3 normalCorrected = normalize(normalTransposed);
        vec3 lightDirection = normalize(lightPosition - fragmentPosition);
        float angleFactor = max(dot(normalCorrected, lightDirection), 0.0);
        vec3 diffuse = angleFactor * objectColour * lightColour * lightStrength;

        // specular
        vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
        float spec = 0.0;
        if (angleFactor > 0.0) {
                vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
                spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
        }
        vec3 specular = spec * lightColour * specularStrength;

        vec3 result = ambient + diffuse + specular;
        fragmentColour = vec4(result, 1.0)/* * texture(textureSampler, UV)*/;
}
