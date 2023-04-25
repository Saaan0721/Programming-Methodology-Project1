class Page {
    public:
        Page();
        Page(int _x, int _y, int _width, int _height, int _id, char _content)
        :x(_x), y(_y), width(_width), height(_height), id(_id), content(_content) {}
        void set_position(int _x, int _y) { x=_x, y=_y; }
        void set_size(int _width, int _height) { width=_width, height=_height; }
        void set_id(int _id) { id=_id; }
        void set_content(char _content) { content=_content; }

        int* get_position() {
            int position[2] = {x, y};
            return position;
        }
        int* get_size() {
            int size[2] = {width, height};
            return size;
        }
        int get_id() { return id; };
        char get_content() { return content; };

    private:
        int x, y; // position of the page on the board
        int width, height; // width and height of the page 
        int id; // unique id for each page
        char content; 
};
