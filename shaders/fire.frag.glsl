uniform sampler2D fireTex;
uniform float age;

void main() {
	
	gl_FragColor = texture2D(fireTex,gl_TexCoord[0].st);
	//gl_FragColor.w -= age*0.2;
}