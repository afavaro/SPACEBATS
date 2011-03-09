uniform sampler2D frame1;
uniform sampler2D frame2;
uniform sampler2D frame3;
uniform sampler2D frame4;

varying vec3 position;

void main() {


	// This actually writes to the frame buffer
	gl_FragColor = vec4(diffuse + specular + ambient, 1);
}
