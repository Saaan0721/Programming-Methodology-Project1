#include <vector>
#include <cmath>

using std::abs;

class Page {
    public:
        // constructors
        Page() {}; 
        Page(int _x, int _y, int _width, int _height, int _id, char _content)
            :x(_x), y(_y), width(_width), height(_height), id(_id), content(_content) {}

        // mutators
        void set_x(int _x) { x=_x; }
        void set_y(int _y) { y=_y; }
        void set_width(int _width) { width=_width; }
        void set_height(int _height) { height=_height; }
        void set_id(int _id) { id=_id; }
        void set_content(char _content) { content=_content; }

        // accessors
        int get_x() { return x; }
        int get_y() { return y; }
        int get_width() { return width; }
        int get_height() { return height; }
        int get_id() { return id; };
        char get_content() { return content; };

        // "overlapped" means two pages are close enough for "above"
        // it only check distance between page, not depth.
        bool is_overlapped(Page p);
        bool is_overlapped(int _x, int _y);

        // one page is smaller than another means it has lower id.
        bool operator<(Page &p);

        // two pages are equal means that both pages have the same id.
        bool operator==(Page p);
        

    private:
        int x, y; // position of the page on the board
        int width, height; // width and height of the page 
        int id; // unique id for each page
        char content;
};

bool Page::is_overlapped(Page p) {
    double distance_x, distance_y;

    // if an area of any page is zero, it doesn't overlap any other page.
    if(!(width * height * p.get_width() * p.get_height())) {
        return false;
    }

    // calculate x, y distance between two pages.
    distance_x = abs((x + (double) width / 2.0) - (p.get_x() + (double) p.get_width() / 2.0));
    distance_y = abs((y + (double) height / 2.0) - (p.get_y() + (double) p.get_height() / 2.0));

    // check if two pages are close enough.
    return (distance_x < (double)(width + p.get_width()) / 2.0) &&
        (distance_y < (double)(height + p.get_height()) / 2.0);
}

// overloading
bool Page::is_overlapped(int _x, int _y) {
    double distance_x, distance_y;
    Page p = Page();
    p.set_x(_x);
    p.set_y(_y);
    p.set_width(1);
    p.set_height(1);

    if(!(width * height * p.get_width() * p.get_height())) {
        return false;
    }

    distance_x = abs((x + (double) width / 2.0) - (p.get_x() + (double) p.get_width() / 2.0));
    distance_y = abs((y + (double) height / 2.0) - (p.get_y() + (double) p.get_height() / 2.0));

    return (distance_x < (double)(width + p.get_width()) / 2.0) &&
        (distance_y < (double)(height + p.get_height()) / 2.0);
}

bool Page::operator<(Page &p) {
    return id < p.get_id();
}

bool Page::operator==(Page p) {
    return id == p.get_id();
}
