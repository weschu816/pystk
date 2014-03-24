uniform vec3 windDir;
uniform mat4 ViewProjectionMatrix;
uniform mat4 ViewMatrix;

in vec3 Origin;
in vec3 Orientation;

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
in vec4 Color;

out vec3 nor;
out vec2 uv;


void main()
{
    mat4 ModelMatrix = mat4(1.);
    ModelMatrix[3].xyz += Origin;
    mat4 TransposeInverseModelView = transpose(inverse(ViewMatrix * ModelMatrix));
    gl_Position = ViewProjectionMatrix *  ModelMatrix * vec4(Position + windDir * Color.r, 1.);
    nor = (TransposeInverseModelView * vec4(Normal, 0.)).xyz;
    uv = Texcoord;
}
