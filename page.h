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

    private:
        int x, y; // position of the page on the board
        int width, height; // width and height of the page 
        int id; // unique id for each page
        char content; 
};
