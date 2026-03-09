#include <cmath>
#include <ctime>
#include <cstdlib>

#include "tgaimage.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &buffer, TGAColor color) {
    int dx = abs(bx - ax);
    int dy = abs(by - ay) * -1;
    int sx = ax < bx ? 1 : -1;
    int sy = ay < by ? 1 : -1;

    int error = dx + dy;
    int e2;
    while (true) {
        buffer.set(ax,ay,color);
        e2 = 2 * error;
        if (e2 >= dy) {
            if (ax == bx) break;
            error = error + dy;
            ax += sx;
        }
        if (e2 <= dx) {
            if (ay == by) break;
            error = error + dx;
            ay += sy;
        }
    }
}

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    std::srand(std::time({}));
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width, ay = rand()%height;
        int bx = rand()%width, by = rand()%height;
        line(ax, ay, bx, by, framebuffer, { rand()%255, rand()%255, rand()%255, rand()%255 });
    }

    // line(ax, ay, bx, by, framebuffer, blue);
    // line(cx, cy, bx, by, framebuffer, green);
    // line(cx, cy, ax, ay, framebuffer, yellow);
    // line(ax, ay, cx, cy, framebuffer, red);
    //
    // framebuffer.set(ax, ay, white);
    // framebuffer.set(bx, by, white);
    // framebuffer.set(cx, cy, white);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}