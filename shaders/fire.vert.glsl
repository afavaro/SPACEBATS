attribute vec3 positionIn;
uniform float baseSize;
uniform float alpha;
uniform float viewportWidth;

varying vec4 clipPosition;

void main()
{
	vec4 eyeTemp = gl_ModelViewMatrix * vec4(positionIn, 1);
	
	gl_PointSize = baseSize * viewportWidth / length(eyeTemp.xyz);
	
	gl_Position = gl_ProjectionMatrix * eyeTemp;
	//gl_Position = vec4(positionIn,1);


}