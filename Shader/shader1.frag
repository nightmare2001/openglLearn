#version 330 core
out vec4 FragColor;
in vec2 out_uv;
uniform sampler2D _MainTex;
uniform sampler2D _MainTex1;
uniform float _MixValue;
uniform vec3 objectColor;
uniform vec3 lightColor;
void main()
{
   vec4 color1 = texture(_MainTex,out_uv);
   vec4 color2 = texture(_MainTex1,out_uv);
   vec4 texColor = mix(color1,color2,color2.w*_MixValue);
   FragColor = vec4(lightColor*objectColor,1.0);
}