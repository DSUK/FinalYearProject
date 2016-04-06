#version 120
//input from the vertex shader
varying vec3 pos;
varying vec3 normal;

//input from the program
//light properties
uniform vec3 amblight;
uniform vec3 diffuselight;
uniform vec3 speclight;
uniform vec3 lightpos;

//surface properties
uniform vec3 samb;
uniform vec3 sdif;
uniform vec3 sspec;
uniform float ref_number;

void main()
{
    //ambient
    vec3 ambientcomp = vec3(amblight*samb);
    //diffuse
    vec3 nnorm = vec3(normalize(normal));                   //normalised surface normal
    vec3 lightvec = vec3(normalize(vec3(lightpos - pos))); //vector from surface to light
    float diffco = max(dot(lightvec,nnorm),0.0);             //dot product to establish diffuse light
    vec3 diffcomp = vec3(diffco*diffuselight*sdif);           //diffuse componmant
    //specular
    vec3 veiwervec = vec3(normalize(-pos));                             //vector from object, to vewer (veiwer is always at (0,0,0) in opengl)
    vec3 reflight = vec3(normalize(reflect(-lightvec,nnorm));          //light reflection vector
    float specco = pow(max(dot(veiwervec,reflight),0),ref_number);      //spec = (veiwervec dot reflection)^n
    vec3 speccomp = vec3(sspec*speclight*specco);
    //attenuate + output
    float dist = length(lightpos - pos);
    float att = 1/(1+ 0.1*dist*dist);
    gl_FragColor = (ambientcomp +  diffcomp + speccomp)*att;
}
