#version 400


in vec2 TexCoordG;
in vec4 Weight;
in float diffTerm;
out vec4 outputColor;


uniform sampler2D wGrassTexture;
uniform sampler2D snowTexture;
uniform sampler2D rockTexture;
uniform sampler2D waterTexture;

uniform mat4 mvpMatrix;

void main() 
{

	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1);
	vec4 texColorW = (texture(waterTexture, TexCoordG) + ((vec4(0.0,1.0,0.0,0.0)) * 0.2)) * Weight.x;
	vec4 texColorG = texture(wGrassTexture, TexCoordG) * Weight.y;  //Get colour from texture  
	vec4 texColorR = texture(rockTexture, TexCoordG) * Weight.z;
	vec4 texColorS = texture(snowTexture, TexCoordG) * Weight.w;
	
	
	vec4 texColorCombined = texColorW + texColorG + texColorR + texColorS;

	outputColor = diffTerm * texColorCombined * lightColor;
	
}
