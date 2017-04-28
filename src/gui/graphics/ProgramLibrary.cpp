#include "ProgramLibrary.h"
#include <sstream>
#include "Material.h"

void gp::graphics::ProgramLibrary::createShadowProgram()
{    
  std::string shadowVertexShader = R"glsl(
#version 330
layout(location=0) in vec3 Position;
uniform mat4 depthMVP;
void main() {
  gl_Position = depthMVP * vec4(Position, 1.0);
}
)glsl";

  std::string shadowFragmentShader = R"glsl(
#version 330
out float fragdepth;
void main() {
  fragdepth = gl_FragCoord.z;
}
)glsl";
  
  shadow.compileAndLink(shadowVertexShader, shadowFragmentShader);
}

void gp::graphics::ProgramLibrary::createLinesProgram()
{    
  std::string vertexShader = R"glsl(
#version 330
layout(location=0) in vec3 Position;
uniform Camera {
  mat4 P;
  mat4 V;
};
void main() {
  gl_Position = P*V*vec4(Position, 1.0);
}
)glsl";

  std::string fragmentShader = R"glsl(
#version 330
out vec4 fragColor;
void main() {
  fragColor = vec4(1.0);
}
)glsl";
  
  lines.compileAndLink(vertexShader, fragmentShader);
}

void gp::graphics::ProgramLibrary::createShadingProgram(Program& program, size_t interpolationType, size_t textures)
{
  std::stringstream vertexShader;
  vertexShader << "#version 330\n";
  if (interpolationType == Flat) {
    vertexShader << "#define FLAT\n";
  }
  vertexShader << R"glsl(
layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
uniform Camera {
  mat4 P;
  mat4 V;
};
layout(std140) uniform SpotLight {
  vec3 LightPosition;
  float CutoffAngle;
  vec3 SpotDirection;
  float AmbientIntensity;
};
uniform mat4 MV;
uniform mat3 NormalMatrix;
uniform mat4 depthMVP;
#ifdef FLAT
  flat out vec3 vNormal;
#else
  out vec3 vNormal;
#endif
out vec3 vEye;
out vec3 vLightDir;
out vec4 vShadowCoord;
out vec3 vCubeMap;
void main() {
  vec4 pos = MV * vec4(Position, 1.0);
  vNormal = NormalMatrix * Normal;
  vEye = vec3(-pos);
  vLightDir = LightPosition - vec3(pos);
  vShadowCoord = depthMVP * vec4(Position, 1.0);
  vCubeMap = Position;
  gl_Position = P * pos;
}
)glsl";

  std::stringstream fragmentShader;
  fragmentShader << "#version 330\n";
  fragmentShader << "#define MAX_TEXTURES " << Material::NUMBER_OF_TEXTURE_TYPES << "\n";
  if (interpolationType == Flat) {
    fragmentShader << "#define FLAT\n";
  }
  if (textures & (1 << Material::Diffuse)) {
    fragmentShader << "#define DIFFUSETEXTURE\n";
  }
  if (textures & (1 << Material::NormalMap)) {
    fragmentShader << "#define NORMALMAP\n";
  }
  fragmentShader << R"glsl(
#ifdef FLAT
  flat in vec3 vNormal;
#else
  in vec3 vNormal;
#endif
in vec3 vEye;
in vec3 vLightDir;
in vec4 vShadowCoord;
in vec3 vCubeMap;
layout(std140) uniform Material {
  vec3 DiffuseColor;
  float Shininess;
  vec3 SpecularColor;
};
layout(std140) uniform SpotLight {
  vec3 LightPosition;
  float CutoffAngle;
  vec3 SpotDirection;
  float AmbientIntensity;
};
uniform sampler2DShadow ShadowMap;
uniform samplerCube Textures[MAX_TEXTURES];
out vec4 fragColor;

float ShadowCalculation(vec4 fragPosLightSpace) {
  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // Transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;
  return texture(ShadowMap, projCoords);
}
vec2 xyzToCubeUV(vec3 pos)
{
  vec2 uv;
  float ma;
  vec3 absPos = abs(pos);
  if (absPos.x >= absPos.y && absPos.x >= absPos.z) {
    ma = absPos.x;
    uv.s = -pos.z * sign(pos.x);
    uv.t = -pos.y;
  } else if (absPos.y >= absPos.z) {
    ma = absPos.y;
    uv.s = pos.x;
    uv.t = pos.z * sign(pos.y);
  } else {
    ma = absPos.z;
    uv.s = pos.x * sign(pos.z);
    uv.t = -pos.y;
  }
  uv = 0.5 * uv / ma + 0.5;
  return uv;
}
// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx(p);
    vec3 dp2 = dFdy(p);
    vec2 duv1 = dFdx(uv);
    vec2 duv2 = dFdy(uv);
 
    // solve the linear system
    vec3 dp2perp = cross(dp2, N);
    vec3 dp1perp = cross(N, dp1);
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt(max(dot(T,T), dot(B,B)));
    return mat3(T * invmax, B * invmax, N);
}
void main() {
  float visibility = 1.0;
  float intensity = 0.0;
  float specularIntensity = 0.0;
  vec3 l = normalize(vLightDir);
  vec3 s = normalize(-SpotDirection);
#ifdef DIFFUSETEXTURE
  vec3 diffuseColor = texture(Textures[0], vCubeMap).rgb;
#else
  vec3 diffuseColor = DiffuseColor;
#endif

  if (dot(l, s) > CutoffAngle) {
  vec3 n = normalize(vNormal);
#ifdef NORMALMAP
    vec3 pos = -vEye;
    vec3 N = 2.0 * texture(Textures[1], vCubeMap).rgb - vec3(1.0);
    n = normalize(cotangent_frame(n, pos, xyzToCubeUV(vCubeMap)) * N);
#endif
    float cosTheta = dot(n,l);
    intensity = max(cosTheta, 0.0);
    if (intensity > 0) {
      //float falloff = 20.0 / dot(vLightDir, vLightDir);
      visibility = ShadowCalculation(vShadowCoord);
      intensity = intensity;
      vec3 e = normalize(vEye);
      vec3 h = normalize(l + e);
      specularIntensity = pow( max(dot(n,h), 0.0), Shininess);
    }
  }
  
  fragColor.rgb = AmbientIntensity * diffuseColor + visibility * (diffuseColor * intensity + SpecularColor * specularIntensity);
  fragColor.a = 1.0;
}
)glsl";
  
  program.compileAndLink(vertexShader.str(), fragmentShader.str());
}

gp::graphics::ProgramLibrary::ProgramLibrary()
{
  createShadowProgram();
  createLinesProgram();
}

gp::graphics::Program* gp::graphics::ProgramLibrary::shading(enum NormalInterpolation interpolationType, size_t textures)
{
  auto program = m_shading[interpolationType].find(textures);
  if (m_shading[interpolationType].find(textures) == m_shading[interpolationType].end()) {
    for (unsigned i = 0; i < NUMBER_OF_INTERPOLATION_TYPES; ++i) {
      createShadingProgram(m_shading[i][textures], i, textures);
    }
    program = m_shading[interpolationType].find(textures);
  }
  return &program->second;
}
