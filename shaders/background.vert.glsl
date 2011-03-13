
attribute vec2 positionIn;

varying vec4 clipPosition;

void main() {
	
	clipPosition = gl_ModelViewProjectionMatrix * vec4(positionIn.xy, 0, 1);

	gl_Position = clipPosition;

}
