#version 330 core

const int MAX_POINT_LIGHTS = 10; 
const int MAX_SPOT_LIGHTS = 10;
const float PI = 3.141592653589793238462643383;

in DATA
{
	vec2 texcoord;
	vec3 normal;
	vec3 worldpos;
} shader_in;

out vec4 FragColor;

struct BaseLight
{
	vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
	float Const;
	float Lin;
	float Quad;
};

struct PointLight
{
	BaseLight Base;
	vec3 Position;
	Attenuation Att;
};

struct SpotLight
{
	PointLight Point;
	vec3 Direction;
	float Cutoff;
};

uniform sampler2D texture;
uniform DirectionalLight dirlight;
uniform vec3 viewpos;
uniform float matSpecularIntensity;
uniform int matSpecularPower;
uniform int numPointLights;                                                                
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numSpotLights;                                                                
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];      


vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)                   
{                                                                                           
    vec4 Ambient  = vec4(Light.Color * Light.AmbientIntensity, 1.0f);
    vec4 Diffuse  = vec4(0.f);                                                  
    vec4 Specular = vec4(0.f);   
	                                               
    float DiffuseFactor = dot(Normal, -LightDirection);                                     
                                                                                                                                                                                      
    if (DiffuseFactor > 0) 
	{                                                                
        Diffuse = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0f);    
                                                                                            
        vec3 VertexToEye = normalize(viewpos - shader_in.worldpos);                             
        vec3 LightReflect = reflect(LightDirection, Normal);                     
        float SpecularFactor = dot(VertexToEye, LightReflect);                                      
        if (SpecularFactor > 0) 
		{                                                           
            SpecularFactor = pow(SpecularFactor, matSpecularPower);
            Specular = vec4(Light.Color * matSpecularIntensity * SpecularFactor, 1.0f);
        }                                                                                   
    }                                                                                       
                                                                                            
    return (Ambient + Diffuse + Specular);                                   
}     

vec4 CalcDirectionalLight(vec3 Normal)                                                      
{                                                                                           
    return CalcLightInternal(dirlight.Base, dirlight.Direction, Normal); 
} 

vec4 CalcPointLight(PointLight l, vec3 Normal)                                                 
{                                                                                           
    vec3 LightDirection = shader_in.worldpos - l.Position;
    float Distance = length(LightDirection);                                                
    LightDirection = normalize(LightDirection);                                             
                                                                                            
    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal);       
    float Attenuation =  l.Att.Const +                               
                         l.Att.Lin * Distance +                      
                         l.Att.Quad * Distance * Distance;               
                                                                                            
    return Color / Attenuation;                                                             
}        

vec4 CalcSpotLight(SpotLight l, vec3 Normal)  
{
	vec3 LightToPixel = normalize(shader_in.worldpos - l.Point.Position);
	float SpotFactor = dot(LightToPixel, l.Direction);

	if (SpotFactor > l.Cutoff)
	{
		vec4 Color = CalcPointLight(l.Point, Normal);
		float coeff = (1.0f - (1.0f - SpotFactor) / (1.0f - l.Cutoff));
		//coeff = 0.5 * ( sin((coeff - 0.5f) * PI) + 1.f);
		return Color * coeff;
	}
	else
	{
		return vec4(0.f);
	}
}  

void main()
{
	vec3 Normal = normalize(shader_in.normal);

	vec4 TotalLight = CalcDirectionalLight(Normal);                                         

	for (int i = 0 ; i < numPointLights ; i++) 
	{                                           
        TotalLight += CalcPointLight(pointLights[i], Normal);                                            
    }

	for (int i = 0 ; i < numSpotLights ; i++) 
	{     
        TotalLight += CalcSpotLight(spotLights[i], Normal);                                                                                      
    }    

    FragColor = texture2D(texture, shader_in.texcoord.xy) * TotalLight;
	
}