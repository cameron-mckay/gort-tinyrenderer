#include <cmath>
#include <ctime>
#include <cstdlib>

#include "model.h"
#include "tgaimage.h"

// constexpr int width  = 128;
// constexpr int height = 128;
constexpr int width  = 8000;
constexpr int height = 8000;
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

void wireframe(Model &model, TGAImage &buffer) {
    for (int i = 0; i < model.faces.size(); i++) {
        auto face = model.faces[i];
        auto v0 = project(model.vertices[face.v0]);
        auto v1 = project(model.vertices[face.v1]);
        auto v2 = project(model.vertices[face.v2]);
        line(v0.x, v0.y, v1.x, v1.y, buffer, green);
        line(v1.x, v1.y, v2.x, v2.y, buffer, green);
        line(v2.x, v2.y, v0.x, v0.y, buffer, green);
        buffer.set(v0.x, v0.y, white);
        buffer.set(v1.x, v1.y, white);
        buffer.set(v2.x, v2.y, white);
    }
}

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &buffer, TGAColor color) {
    if (ay>by) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    if (ay>cy) {
        std::swap(ax, cx);
        std::swap(ay, cy);
    }
    if (by>cy) {
        std::swap(bx, cx);
        std::swap(by, cy);
    }

    // int dxa = abs(cx - ax);
    // int dya = abs(cy - ay) * -1;
    // int sxa = ax < cx ? 1 : -1;
    // int sya = ay < cy ? 1 : -1;
    //
    // int dxb = abs(cx - ax);
    // int dyb = abs(cy - ay) * -1;
    // int sxb = ax < cx ? 1 : -1;
    // int syb = ay < cy ? 1 : -1;
    //
    // int errora = dxa + dya;
    // int errorb = dxb + dyb;
    // int e2;
    // while (true) {
    //     buffer.set(ax,ay,color);
    //     e2 = 2 * error;
    //     if (e2 >= dy) {
    //         if (ax == bx) break;
    //         error = error + dy;
    //         ax += sx;
    //     }
    //     if (e2 <= dx) {
    //         if (ay == by) break;
    //         error = error + dx;
    //         ay += sy;
    //     }
    // }

    // line(ax, ay, bx, by, buffer, green);
    // line(bx, by, cx, cy, buffer, green);
    // line(cx, cy, ax, ay, buffer, red);
}

int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);

    /* first triangle */
    // int ax =  7, ay =  3;
    // int bx = 12, by = 37;
    // int cx = 62, cy = 53;
    // line(ax, ay, bx, by, framebuffer, blue);
    // line(cx, cy, bx, by, framebuffer, green);
    // line(cx, cy, ax, ay, framebuffer, yellow);
    // line(ax, ay, cx, cy, framebuffer, red);
    // framebuffer.set(ax, ay, white);
    // framebuffer.set(bx, by, white);
    // framebuffer.set(cx, cy, white);

    /* random lines */
    // std::srand(std::time({}));
    // for (int i=0; i<(1<<24); i++) {
    //     int ax = rand()%width, ay = rand()%height;
    //     int bx = rand()%width, by = rand()%height;
    //     line(ax, ay, bx, by, framebuffer, { rand()%255, rand()%255, rand()%255, rand()%255 });
    // }

    /* diablo wireframe */
    Model model("./obj/diablo3_pose/diablo3_pose.obj");
    wireframe(model, framebuffer);

    // triangle(  7, 45, 35, 100, 45,  60, framebuffer, red);
    // triangle(120, 35, 90,   5, 45, 110, framebuffer, white);
    // triangle(115, 83, 80,  90, 85, 120, framebuffer, green);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}