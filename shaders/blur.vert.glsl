attribute vec2 positionIn;
attribute vec2 texCoordIn;

varying vec4 clipPosition;

void main() {

	// Transform the vertex to get the eye-space position of the vertex
	vec4 eyeTemp = gl_ModelViewMatrix * vec4(positionIn, 0, 1);
	vec3 eyePosition = eyeTemp.xyz;

	// Transform again to get the clip-space position.  The gl_Position
	// variable tells OpenGL where the vertex should go.
	gl_Position = gl_ProjectionMatrix * eyeTemp;
	clipPosition = gl_Position;
}
