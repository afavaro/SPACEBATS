
uniform sampler2D texture;

varying vec4 clipPosition;

void main() {
	
	vec2 texcoord = 0.5 * (clipPosition.xy / clipPosition.w) + vec2(0.5, 0.5);

	gl_FragColor = texture2D(texture, texcoord);

}
