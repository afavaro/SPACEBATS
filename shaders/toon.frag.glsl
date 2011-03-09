
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform vec2 pixelSize;

uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 Ka;
uniform float alpha;

varying vec2 texcoord;
varying vec3 normal;
varying vec3 eyePosition;
varying vec4 clipPosition;

vec4 gradient(vec2 p, vec4 pcolor) {
	vec4 grad = vec4(0, 0, 0, 0);

	vec2 a = p + vec2(-pixelSize.x, pixelSize.y);
	grad += abs(texture2D(normalMap, a) - pcolor);

	vec2 b = p + vec2(0, pixelSize.y);
	grad += 2.0 * abs(texture2D(normalMap, b) - pcolor);

	vec2 c = p + pixelSize;
	grad += abs(texture2D(normalMap, c) - pcolor);

	vec2 d = p - vec2(pixelSize.x, 0);
	grad += 2.0 * abs(texture2D(normalMap, d) - pcolor);

	vec2 e = p + vec2(pixelSize.x, 0);
	grad += 2.0 * abs(texture2D(normalMap, e) - pcolor);

	vec2 f = p - pixelSize;
	grad += abs(texture2D(normalMap, f) - pcolor);

	vec2 g = p - vec2(0, pixelSize.y);
	grad += 2.0 * abs(texture2D(normalMap, g) - pcolor);

	vec2 h = p + vec2(pixelSize.x, -pixelSize.y);
	grad += abs(texture2D(normalMap, h) - pcolor);

	return grad / 8.0;
}

float outline(vec2 p) {
	vec4 gmax = gradient(p, texture2D(normalMap, p));
	vec4 gmin = gmax;

	vec2 q = p + vec2(-pixelSize.x, pixelSize.y);
	vec4 g = gradient(q, texture2D(normalMap, q));
	gmax.x = max(gmax.x, g.x); gmin.x = min(gmin.x, g.x);
	gmax.y = max(gmax.y, g.y); gmin.y = min(gmin.y, g.y);
	gmax.z = max(gmax.z, g.z); gmin.z = min(gmin.z, g.z);
	gmax.w = max(gmax.w, g.w); gmin.w = min(gmin.w, g.w);

	q = p + vec2(0, pixelSize.y);
	g = gradient(q, texture2D(normalMap, q));
	gmax.x = max(gmax.x, g.x); gmin.x = min(gmin.x, g.x);
	gmax.y = max(gmax.y, g.y); gmin.y = min(gmin.y, g.y);
	gmax.z = max(gmax.z, g.z); gmin.z = min(gmin.z, g.z);
	gmax.w = max(gmax.w, g.w); gmin.w = min(gmin.w, g.w);

	q = p + pixelSize;
	g = gradient(q, texture2D(normalMap, q));
	gmax.x = max(gmax.x, g.x); gmin.x = min(gmin.x, g.x);
	gmax.y = max(gmax.y, g.y); gmin.y = min(gmin.y, g.y);
	gmax.z = max(gmax.z, g.z); gmin.z = min(gmin.z, g.z);
	gmax.w = max(gmax.w, g.w); gmin.w = min(gmin.w, g.w);

	q = p - vec2(pixelSize.x, 0);
	g = gradient(q, texture2D(normalMap, q));
	gmax.x = max(gmax.x, g.x); gmin.x = min(gmin.x, g.x);
	gmax.y = max(gmax.y, g.y); gmin.y = min(gmin.y, g.y);
	gmax.z = max(gmax.z, g.z); gmin.z = min(gmin.z, g.z);
	gmax.w = max(gmax.w, g.w); gmin.w = min(gmin.w, g.w);

	q = p + vec2(pixelSize.x, 0);
	g = gradient(q, texture2D(normalMap, q));
	gmax.x = max(gmax.x, g.x); gmin.x = min(gmin.x, g.x);
	gmax.y = max(gmax.y, g.y); gmin.y = min(gmin.y, g.y);
	gmax.z = max(gmax.z, g.z); gmin.z = min(gmin.z, g.z);
	gmax.w = max(gmax.w, g.w); gmin.w = min(gmin.w, g.w);

	q = p - pixelSize;
	g = gradient(q, texture2D(normalMap, q));
	gmax.x = max(gmax.x, g.x); gmin.x = min(gmin.x, g.x);
	gmax.y = max(gmax.y, g.y); gmin.y = min(gmin.y, g.y);
	gmax.z = max(gmax.z, g.z); gmin.z = min(gmin.z, g.z);
	gmax.w = max(gmax.w, g.w); gmin.w = min(gmin.w, g.w);

	q = p - vec2(0, pixelSize.y);
	g = gradient(q, texture2D(normalMap, q));
	gmax.x = max(gmax.x, g.x); gmin.x = min(gmin.x, g.x);
	gmax.y = max(gmax.y, g.y); gmin.y = min(gmin.y, g.y);
	gmax.z = max(gmax.z, g.z); gmin.z = min(gmin.z, g.z);
	gmax.w = max(gmax.w, g.w); gmin.w = min(gmin.w, g.w);

	q = p + vec2(pixelSize.x, -pixelSize.y);
	g = gradient(q, texture2D(normalMap, q));
	gmax.x = max(gmax.x, g.x); gmin.x = min(gmin.x, g.x);
	gmax.y = max(gmax.y, g.y); gmin.y = min(gmin.y, g.y);
	gmax.z = max(gmax.z, g.z); gmin.z = min(gmin.z, g.z);
	gmax.w = max(gmax.w, g.w); gmin.w = min(gmin.w, g.w);

	float dn = length((gmax.xyz - gmin.xyz) / 0.1);
	float dz = (gmax.w - gmin.w) / 0.005;
	return max(min(dn * dn, 1.0), min(dz * dz, 1.0));
}

void main() {

	vec3 N = normalize(normal);
	vec3 L = normalize(gl_LightSource[0].position.xyz);
	vec3 V = normalize(-eyePosition);

	float Rd;
	if (dot(L, N) > 0) Rd = 1.0;
	else Rd = 0.0;
	vec3 Td = texture2D(diffuseMap, texcoord).rgb;

	vec3 diffuse = Rd * Kd * Td * gl_LightSource[0].diffuse.rgb;
	vec3 ambient = Ka * Td * gl_LightSource[0].ambient.rgb;

	vec3 R = reflect(-L, N);
	float Rs = pow(max(0.0, dot(V, R)), alpha);
	if (Rs > 0.1) Rs = 1.0;
	else Rs = 0.0;
	vec3 Ts = texture2D(specularMap, texcoord).rgb;
	vec3 specular = Rs * Ks * Ts * gl_LightSource[0].specular.rgb;

	clipPosition /= clipPosition.w;	
	vec2 normcoord = 0.5 * clipPosition.xy + vec2(0.5, 0.5);
	float border = 1.0 - outline(normcoord);

	gl_FragColor = border * vec4(diffuse + ambient, 1);

}
