uniform sampler2D frame1;
uniform sampler2D frame2;
uniform sampler2D frame3;
uniform sampler2D frame4;

varying vec3 clipPosition;

void main() {
	clipPosition /= clipPosition.w;	
	vec2 normcoord = 0.5 * clipPosition.xy + vec2(0.5, 0.5);

	// This actually writes to the frame buffer
	gl_FragColor = vec4(diffuse + specular + ambient, 1);
}
