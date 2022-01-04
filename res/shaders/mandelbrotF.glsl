#version 450 core

uniform float width;
uniform float height;
uniform float zoom;

uniform vec3 pos;

out vec4 FragColor;

vec2 squared(vec2 z) {
    return vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y);
}

float mandelbrot(vec2 coord){
	vec2 z = vec2(0,0);
    float n = 0;
	for(int i=0;i<100;i++){
		z = squared(z) + coord;
		if(dot(z, z) > 100*100) break;
        n += 1;
	}

    if (n > 99) return 0; // make sure center is black;

	return n/50; // This handles color intensity
}

void main()
{
    float ar = width / height;

    vec2 uv = vec2(gl_FragCoord.xy) / zoom * 100;
    uv.x = uv.x / width;
    uv.y = uv.y / height;
    uv += -1 + pos.xy / 3;
    uv.x *= ar;
    float N = mandelbrot(uv);
    FragColor = 0.1 + vec4(0.1*N, 0.6*N, N, 1);
};
