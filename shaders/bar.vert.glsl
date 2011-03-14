
attribute vec2 positionIn;
attribute vec4 colorIn;

varying vec4 color;

void main() {
	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(positionIn.xy, 0, 1);

	color = colorIn;

}
