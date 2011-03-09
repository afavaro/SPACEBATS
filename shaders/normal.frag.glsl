
uniform float viewDepth;

varying vec3 normal;
varying float eyeZ;

void main() {

	vec3 c = 0.5 * normal + vec3(0.5, 0.5, 0.5);
	gl_FragColor = vec4(c, eyeZ / viewDepth);

}
