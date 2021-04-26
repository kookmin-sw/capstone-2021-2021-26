#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
out vec3 nearPoint;
out vec3 farPoint;
out mat4 fragView;
out mat4 fragProj;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 UnprojectPoint(float x, float y, float z, mat4 mview, mat4 mprojection) {
    vec4 unprojectedPoint =  mview * mprojection * vec4(x, y, z, 0.5);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}


void main()
{
    vec3 p = aPos;
    nearPoint = UnprojectPoint(p.x, p.y, 1.0, view, projection); // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection); // unprojecting on the far plane
	
    FragPos	= vec3(model * vec4(p, 1.0));
	Normal	= normalize(vec3(model * vec4(aNormal, 1.0)));
	TexCoord= aTexCoord;
	
    fragView = view;
    fragProj = projection;
	
    gl_Position = projection * view * model * vec4(p, 1.0);
}
