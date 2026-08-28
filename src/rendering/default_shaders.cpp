#include "default_shaders.hpp"

namespace jadeite
{

const char* DefaultShaders::shapeShaderVert = 
R"(#version 300 es
in vec2 aPos;
in vec4 aColor;

out vec4 fragColor;
uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * vec4(aPos, 0.0f, 1.0f);
	fragColor = aColor;
}
)";

const char* DefaultShaders::shapeShaderFrag = 
R"(#version 300 es
precision mediump float;

in vec4 fragColor;
out vec4 color;

void main()
{
	color = fragColor;
}
)";	
	
const char* DefaultShaders::basicShaderVert = 
R"(#version 300 es
in vec2 aPos;
in vec2 aUVs;
in vec4 aColor;

out vec2 fragUVs;
out vec4 fragColor;

uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * vec4(aPos, 0.0f, 1.0f);
	fragUVs = aUVs;
	fragColor = aColor;
}
)";

const char* DefaultShaders::basicShaderFrag = 
R"(#version 300 es
precision mediump float;

in vec2 fragUVs;
in vec4 fragColor;

uniform sampler2D uTexture;
out vec4 color;

void main()
{
	color = texture(uTexture, fragUVs) * fragColor;
}
)";

const char* DefaultShaders::fontShaderVert = 
R"(#version 300 es
in vec2 aPos;
in vec2 aUVs;
in vec4 aColor;

out vec2 fragUVs;
out vec4 fragColor;

uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * vec4(aPos, 0.0f, 1.0f);
	fragUVs = aUVs;
	fragColor = aColor;
}
)";

const char* DefaultShaders::fontShaderFrag = 
R"(#version 300 es
precision mediump float;

in vec2 fragUVs;
in vec4 fragColor;

uniform sampler2D uFontAtlas;
out vec4 color;

void main()
{
	vec4 fontColor = vec4(1, 1, 1, texture(uFontAtlas, fragUVs).a);
	color = fontColor * fragColor;
}
)";


} // jadeite
