uniform sampler2D  tex ;
uniform vec4 lightPos ;
uniform vec4 lightLambert ;
uniform vec4 lightAmbient ;
uniform float m ;
uniform float ka ;
uniform float kd ;
uniform float ks ;
varying vec3 normal ;
varying vec4 lightVertex ;
varying vec4 color ;
varying vec4 worldCoord ;
void main()
{
    vec2 poissonDisk[4] ;
    poissonDisk[0] = vec2( -0.94201624, -0.39906216 ) ;
    poissonDisk[1] = vec2( 0.94558609, -0.76890725 ) ;
    poissonDisk[2] = vec2( -0.094184101, -0.92938870 );
    poissonDisk[3] = vec2( 0.34495938, 0.29387760 ) ;
    vec3 l = normalize(lightPos - worldCoord).xyz ;
    vec3 n = normalize(normal) ;
    vec3 v = normalize(-worldCoord).xyz ;
    float d = max(dot(n , l) , 0.0) ;
    float s = max(dot(normalize(v + l) , n) , 0.0)  ;
    float cosTheta = dot(n , l) ;
    float bias = m*tan(acos(cosTheta));
    float shadow = 1.0 ;
    //使用poissonDisk采样优化锯齿
    for (int i = 0 ; i < 4; ++i)
    {
        if ( texture2D( tex, lightVertex.xy + poissonDisk[i]/700.0 ).z  <  lightVertex.z-bias ){
            shadow -=0.2;
        }
    }
    //不使用优化
   /*float depth = texture2D(tex , lightVertex.xy).z ;

    if (depth + bias < lightVertex.z  )
    {
        shadow = 0.2 ;
    }*/
   // shadow = 0.5 ;
    vec4 finalColor = vec4(0.0 , 0.0 , 0.0 , 1.0) ;
    //finalColor *= shadow ;
    finalColor = (finalColor + ks * s * lightLambert + ka*lightAmbient  + kd * d * lightLambert)*shadow;
    gl_FragColor = finalColor ;
}