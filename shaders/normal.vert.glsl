
attribute vec3 positionIn;
attribute vec3 normalIn;

varying vec3 normal;
varying float eyeZ;

void main() {

	gl_Position = gl_ModelViewProjectionMatrix * vec4(positionIn, 1);

	eyeZ = gl_Position.z;

	normal = gl_NormalMatrix * normalIn;

}
