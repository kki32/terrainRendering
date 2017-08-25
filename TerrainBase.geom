#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;
uniform mat4 mvpMatrix;

in vec2 TexCoordGin[];
out vec2 TexCoordG;
out float diffTerm;

out vec4 Weight;


void main()
{
	float H = 8;
	float bottom = ((((3*H)/4) - (H/2))*2 / 10);

	
	float GrassRockPos =  bottom + (H/2);
	float bottom2 = ((H - ((3*H)/4))*2 / 10);
	
	float RockSnowPos = bottom2 + ((3*H)/4);
	float y;
	

	
	
	int i;
/*
	vec2 TexCoordGin[3] = vec2[3](vec2(0,0),
	vec2(1,0),
	vec2(0.5,1));
*/	
	
		y = (gl_in[0].gl_Position.y +  gl_in[1].gl_Position.y + gl_in[2].gl_Position.y)/3.0;
		y = max(gl_in[0].gl_Position.y, gl_in[1].gl_Position.y);
		y = max(gl_in[2].gl_Position.y, y);
	
		if(y < H/4){
			Weight = vec4(1,0,0,0);
		} else if(y >= H/4 && y < H/2){
			Weight = vec4(0,1,0,0);
		} else if(y >= H/2 && y < GrassRockPos){
			Weight = vec4(0,((GrassRockPos - y)/(bottom)), 1- (GrassRockPos - y)/(bottom),0);
		} else if(y >= GrassRockPos && y < ((3*H)/4)){
			Weight = vec4(0,0,1,0);
		} else if(y >= ((3*H)/4) && y < RockSnowPos){
			Weight = vec4(0,0, ((RockSnowPos - y)/(bottom2)), 1- (RockSnowPos - y)/(bottom2));
		} else if(y >= RockSnowPos && y <= H){
			Weight = vec4(0,0,0,1);
		}
			
vec3 v1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 v2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 n = cross(v1, v2);
		n = normalize(n);
		vec3 l = vec3(100, 300, 0);
		l = normalize(l);
	for(i=0; i<gl_in.length(); i++)
	{
		
		
		diffTerm = max(0.0, dot(n, l));
		
		gl_Position = mvpMatrix * gl_in[i].gl_Position;
		TexCoordG = TexCoordGin[i];
	
	

		EmitVertex();
	}
		
	EndPrimitive();

	
}
