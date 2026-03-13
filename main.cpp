#include <cmath>
#include <ctime>
#include <cstdlib>

#include "model.h"
#include "tgaimage.h"

// constexpr int width  = 128;
// constexpr int height = 128;
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

double signed_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5 * ((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &buffer, TGAColor color) {
    int bbminx = std::min(std::min(ax, bx), cx);
    int bbminy = std::min(std::min(ay, by), cy);
    int bbmaxx = std::max(std::max(ax, bx), cx);
    int bbmaxy = std::max(std::max(ay, by), cy);

    double total_area = signed_area(ax, ay, bx, by, cx, cy);

    if (total_area < 1) return;
#pragma omp parallel for
    for (int x=bbminx; x <= bbmaxx; x++) {
        for (int y=bbminy; y <= bbmaxy; y++) {
            double a = signed_area(x, y, bx, by, cx, cy) / total_area;
            double b = signed_area(x, y, cx, cy, ax, ay) / total_area;
            double c = signed_area(x, y, ax, ay, bx, by) / total_area;
            if (a < 0 || b < 0 || c < 0) continue;
            buffer.set(x, y, color);
        }
    }
}

void wireframe(Model &model, TGAImage &buffer, TGAColor color) {
    for (int i = 0; i < model.faces.size(); i++) {
        auto face = model.faces[i];
        auto v0 = project(model.vertices[face.v0]);
        auto v1 = project(model.vertices[face.v1]);
        auto v2 = project(model.vertices[face.v2]);
        triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, buffer, { rand()%255, rand()%255, rand()%255, rand()%255 });
    }
    // for (int i = 0; i < model.vertices.size(); i++) {
    //     auto vertex = project(model.vertices[i]);
    //     buffer.set(vertex.x, vertex.y, white);
    // }
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
    Model model("./obj/african_head/african_head.obj");
    wireframe(model, framebuffer, red);

    // triangle(  7, 45, 35, 100, 45,  60, framebuffer, red);
    // triangle(120, 35, 90,   5, 45, 110, framebuffer, white);
    // triangle(115, 83, 80,  90, 85, 120, framebuffer, green);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}