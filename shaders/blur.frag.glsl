uniform sampler2D frame1;
uniform sampler2D frame2;
uniform sampler2D frame3;
uniform sampler2D frame4;

varying vec4 clipPosition;

void main() {
	vec2 normcoord = 0.5 * (clipPosition.xy / clipPosition.w) + vec2(0.5, 0.5);


	vec4 val1 = texture2D(frame1, normcoord);
	vec4 val2 = texture2D(frame2, normcoord);
	vec4 val3 = texture2D(frame3, normcoord);
	vec4 val4 = texture2D(frame4, normcoord);

	vec4 result = 0.25*val1 + 0.25*val2 + 0.25*val3 + 0.25*val4;

	// This actually writes to the frame buffer
	gl_FragColor = vec4(result.xyz, 1);
}
