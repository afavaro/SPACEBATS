uniform sampler2D fireTex;

void main() {

	gl_FragColor = texture2D(fireTex,gl_TexCoord[0].st);
	
}