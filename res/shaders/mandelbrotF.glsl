#version 450 core

uniform float width;
uniform float height;
uniform float zoom;

uniform vec3 pos;
uniform vec2 base;

out vec4 FragColor;

vec2 squared(vec2 z) {
    return vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y);
}

float mandelbrot(vec2 coord){
	vec2 z = base / vec2(width, height) * 2 / zoom * 100;
    float n = 0;
	for(int i=0;i<100;i++){
		z = squared(z) + coord;
		if(dot(z, z) > 100*100) break; // we aleady reached infinity
        n += 1;
	}

    if (n > 99) return 0; // make sure center is black;

    n -= log2(log2(dot(z,z))); // got this from shader toy to smoother the values https://www.shadertoy.com/view/4df3Rn
    n += 8;

    return n/30;
}

void main()
{
    float ar = width / height;
    vec2 res = vec2(width, height);

    vec2 uv = vec2(gl_FragCoord.xy);

    // transpose to center an zoom;
    uv -= res / 2;
    uv /= zoom / 100;
    uv += res / 2;

    // [0, wh] -> [0, 1]
    uv /= res;

    // [0, 1] -> [-1, 1]
    uv = -1 + uv * 2;

    // apply camera pos
    uv += pos.xy / 3;

    uv.x *= ar;
    float N = mandelbrot(uv);
    FragColor = 0.1 + vec4(0.1*N, 0.6*N*N, N*0.8, 1);
};
