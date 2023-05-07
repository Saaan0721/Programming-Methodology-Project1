#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>

#include "page.h"

using std::endl;
using std::ofstream;
using std::vector;
using std::stack;


class Board {
    public:
        Board(int num_jobs, int width, int height, ofstream& output_stream);
        ~Board();

        void print_board();
        void print_job(int job_idx, char job_type, int id);

        //job functions
        void insert_page(int x, int y, int width, int height, int id, int content);
        void insert_page(Page p); //overloading
        void delete_page(int id);
        void delete_page(Page p); //overloading
        void modify_content(int id, char content);
        void modify_position(int id, int x, int y);

        void draw_board();
        vector<Page> above_page(int id);
        Page find_page(int id);
        int find_index(int id);
        bool compare(Page p1, Page p2);
        void clear_stack();
        void insert_stack();

    private:
        int num_jobs, width, height, num_page; 
        ofstream& output; 
        char* board; 
        vector<Page> page;
        stack<Page> page_stack;
};


Board::Board(int num_jobs, int width, int height, ofstream& output_stream): output(output_stream) {
    this->width = width;
    this->height = height;
    this->num_jobs = num_jobs;

    board = new char[width*height];

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
        }
    }

    num_page = 0;
}

Board::~Board() {
    delete board;
    
}


void Board::print_board() {
    int h, w;

    draw_board();

    for (w = 0; w < width+2; w++) output << "- ";
    output << endl;
    
    for (h = 0; h < height; h++) {
        output << "| ";
        for (w = 0; w < width; w++) {
            output << board[h*width + w] << " ";
        }
        output << "| " << endl;
    }

    for (w = 0; w < width+2; w++) output << "- ";
    output << endl;
}

void Board::print_job(int job_idx, char job_type, int id) {
    output << ">> (" << job_idx <<") ";
    switch(job_type) {
        
        case 'i':
            output << "Insert ";
            break;
        case 'd':
            output << "Delete ";
            break;
        case 'm':
            output << "Modify ";
            break;
    }

    output << id << endl;
}

void Board::insert_page(int x, int y, int width, int height, int id, int content) {
    page.push_back(Page(x, y, width, height, id, content));
    print_board();
}

void Board::insert_page(Page p) {
    page.push_back(p);
    print_board();
}

void Board::delete_page(int id) {
    Page target_page = find_page(id);
    vector<Page> above = above_page(id);
    clear_stack();

    if(above.size() == 0) {
        page.erase(page.begin() + find_index(id));
        print_board();
    }
    else {
        for(Page p:above) {
            delete_page(p);
        }

        page.erase(page.begin() + find_index(id));
        print_board();

        while(!page_stack.empty()) {
            insert_page(page_stack.top());
            page_stack.pop();
        }
    }
}

void Board::delete_page(Page p) {
    vector<Page> above = above_page(p.get_id());

    if(find(page.begin(), page.end(), p) == page.end()) {
        return;
    }

    if(above.size() == 0) {
        page_stack.push(p);
        page.erase(page.begin() + find_index(p.get_id()));
        print_board();
    }
    else {
        for(Page p:above) {
            delete_page(p);
        }

        page_stack.push(p);
        page.erase(page.begin() + find_index(p.get_id()));
        print_board();
    }
}

void Board::modify_content(int id, char content) {
    vector<Page> above = above_page(id);
    Page target_page = find_page(id);

    if(above.size() == 0) {
        page.erase(page.begin() + find_index(target_page.get_id()));
        print_board();
        target_page.set_content(content);
        insert_page(target_page);
    }
    else {
        for(Page p:above) {
            delete_page(p);
        }
        page.erase(page.begin() + find_index(target_page.get_id()));
        print_board();
        target_page.set_content(content);
        insert_page(target_page);
        
        while(!page_stack.empty()) {
            insert_page(page_stack.top());
            page_stack.pop();
        }
    }
}

void Board::modify_position(int id, int x, int y) {
    vector<Page> above = above_page(id);
    Page target_page = find_page(id);

    if(above.size() == 0) {
        page.erase(page.begin() + find_index(target_page.get_id()));
        print_board();
        target_page.set_x(x);
        target_page.set_y(y);
        insert_page(target_page);
    }
    else {
        for(Page p:above) {
            delete_page(p);
        }
        page.erase(page.begin() + find_index(target_page.get_id()));
        print_board();
        target_page.set_x(x);
        target_page.set_y(y);
        insert_page(target_page);

        while(!page_stack.empty()) {
            insert_page(page_stack.top());
            page_stack.pop();
        }
    } 
}

void Board::draw_board() {
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
        }
    }
    for(Page p: page) {
        for(int h = 0; h < p.get_height(); h++) {
            for(int w = 0; w < p.get_width(); w++) {
                board[(h+p.get_y())*width + (w+p.get_x())] = p.get_content();
            }
        }
    }
}

vector<Page> Board::above_page(int id) {
    vector<Page> above;
    Page target_page = find_page(id);
    
    for(int y = target_page.get_y(); y < target_page.get_y() + target_page.get_height(); y++) {
        for(int x = target_page.get_x(); x < target_page.get_x() + target_page.get_width(); x++) {
            for(int i = find_index(id) + 1; i < page.size(); i++) {
                if(page.at(i).is_overlapped(x, y)) {
                    if(find(above.begin(), above.end(), page.at(i)) == above.end()) {
                        above.push_back(page.at(i));
                    }
                    break;
                }
            }
        }
    }

    sort(above.begin(), above.end());

    return above;
}

Page Board::find_page(int id) {
    for(int i = 0; i < page.size(); i++) {
        if(page.at(i).get_id() == id) {
            return page.at(i);
        }
    }

    return Page();
}

int Board::find_index(int id) {
    for(int i = 0; i < page.size(); i++) {
        if(page.at(i).get_id() == id) {
            return i;
        }
    }

    return -1;
}

bool Board::compare(Page p1, Page p2) {
    return (p1.get_id() < p2.get_id());
}

void Board::clear_stack() {
    while(!page_stack.empty()) {
        page_stack.pop();
    }
}

void Board::insert_stack() {
    while (!page_stack.empty())
    {
        insert_page(page_stack.top());
        page_stack.pop();
    }
    
}
