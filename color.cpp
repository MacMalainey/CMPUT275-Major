#include <math.h>
#include <Arduino.h>

// https://gist.github.com/kuathadianto/200148f53616cbd226d993b400214a7fs
uint16_t HSVtoRGB(int H, double S, double V) {
	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if(H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if(H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if(H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if(H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if(H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	uint16_t rgb = 0;

	rgb += (Rs + m) * 255 * 65536;  // 65536 = 16 ^ 4 is hex offset to move 0xAA to 0xRR0000
	rgb += (Gs + m) * 255 * 256; // 256 = 16 ^ 2 is hex offset to move 0xGG to 0x00GG00
	rgb += (Bs + m) * 255; // stays at 0x0000BB
    return rgb;
}

uint16_t genNeonColor() {
	return HSVtoRGB(random(256), 1.0, 0.05);
}