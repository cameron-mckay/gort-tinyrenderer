#include <cmath>
#include <ctime>
#include <cstdlib>

#include "model.h"
#include "tgaimage.h"

constexpr int width  = 800;
constexpr int height = 800;
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

vec2 project(vec3 point) {
    return { (point.x + 1.) * (width/2), (point.y+1.) * (height/2) };
}

int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    // std::srand(std::time({}));
    // for (int i=0; i<(1<<24); i++) {
    //     int ax = rand()%width, ay = rand()%height;
    //     int bx = rand()%width, by = rand()%height;
    //     line(ax, ay, bx, by, framebuffer, { rand()%255, rand()%255, rand()%255, rand()%255 });
    // }

    // line(ax, ay, bx, by, framebuffer, blue);
    // line(cx, cy, bx, by, framebuffer, green);
    // line(cx, cy, ax, ay, framebuffer, yellow);
    // line(ax, ay, cx, cy, framebuffer, red);
    //
    // framebuffer.set(ax, ay, white);
    // framebuffer.set(bx, by, white);
    // framebuffer.set(cx, cy, white);

    Model test("./obj/diablo3_pose/diablo3_pose.obj");


    for (int i = 0; i < test.faces.size(); i++) {
        auto face = test.faces[i];
        auto v0 = project(test.vertices[face.v0]);
        auto v1 = project(test.vertices[face.v1]);
        auto v2 = project(test.vertices[face.v2]);

        line(v0.x, v0.y, v1.x, v1.y, framebuffer, red);
        line(v1.x, v1.y, v2.x, v2.y, framebuffer, red);
        line(v2.x, v2.y, v0.x, v0.y, framebuffer, red);
        framebuffer.set(v0.x, v0.y, white);
        framebuffer.set(v1.x, v1.y, white);
        framebuffer.set(v2.x, v2.y, white);
    }


    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}