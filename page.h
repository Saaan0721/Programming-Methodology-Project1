class Page {
    public:
        Page() {}; 
        Page(int _x, int _y, int _width, int _height, int _id, char _content)
            :x(_x), y(_y), width(_width), height(_height), id(_id), content(_content) {}
        void set_x(int _x) { x=_x; }
        void set_y(int _y) { y=_y; }
        void set_width(int _width) { width=_width; }
        void set_height(int _height) { height=_height; }
        void set_id(int _id) { id=_id; }
        void set_content(char _content) { content=_content; }

        int get_x() { return x; }
        int get_y() { return y; }
        int get_width() { return width; }
        int get_height() { return height; }
        int get_id() { return id; };
        char get_content() { return content; };
        bool is_overlapped(Page p);
        

    private:
        int x, y; // position of the page on the board
        int width, height; // width and height of the page 
        int id; // unique id for each page
        char content;
};

bool Page::is_overlapped(Page p2) {
    double distance_x, distance_y;
    distance_x = (x + (double) width / 2) - (p2.get_x() + (double) p2.get_width() / 2);
    distance_y = (y + (double) height / 2) - (p2.get_y() + (double) p2.get_height() / 2);

    return (distance_x < (double)(width + p2.get_width()) / 2) &&
        (distance_y < (double)(height + p2.get_height()) / 2);
}
