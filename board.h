#include <fstream>
#include <vector>

#include "page.h"

using std::endl;
using std::ofstream;
using std::vector;
using std::cout;

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
        void modify_content(int id, char content);
        void modify_position(int id, int x, int y);

        void draw_board();
        vector<Page> above_page(int id);
        Page find_page(int id);

    private:
        int num_jobs, width, height, num_page; 
        ofstream& output; 
        char* board; 
        vector<Page> page;
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


void Board::insert_page(int x, int y, int width, int height, int id, int content) {
    page.push_back(Page(x, y, width, height, id, content));
    print_board();
}

void Board::insert_page(Page p) {
    page.push_back(p);
    print_board();
}

void Board::delete_page(int id) {
    vector<Page> above = above_page(id);
    int idx;
    for(int i = 0; i < page.size(); i++) {
        if(page.at(i).get_id() == id) {
            idx = i;
            break;
        }
    }

    if(above.size() == 0) {
        page.erase(page.begin() + idx);
        print_board();
    }
    else {
        for(Page p:above) {
            delete_page(p.get_id());
            // print_board();
        }
        page.erase(page.begin() + idx);
        print_board();
        for(Page p:above) {
            insert_page(p);
            print_board();
        }
    }
}

void Board::modify_content(int id, char content) {
    vector<Page> above = above_page(id);
    Page target_page = find_page(id);

    if(above.size() == 0) {
        delete_page(id);
        target_page.set_content(content);
        insert_page(target_page);
    }
    else {
        for(Page p:above) {
            delete_page(p.get_id());
            print_board();
        }
        delete_page(id);
        target_page.set_content(content);
        insert_page(target_page);
        for(Page p:above) {
            insert_page(p);
        }
    }
}

void Board::modify_position(int id, int x, int y) {
    vector<Page> above = above_page(id);
    Page target_page = find_page(id);

    if(above.size() == 0) {
        delete_page(id);
        target_page.set_x(x);
        target_page.set_y(y);
        insert_page(target_page);
    }
    else {
        for(Page p:above) {
            delete_page(p.get_id());
            print_board();
        }
        delete_page(id);
        target_page.set_x(x);
        target_page.set_y(y);
        insert_page(target_page);
        for(Page p:above) {
            insert_page(p);
        }
    } 
}

vector<Page> Board::above_page(int id) {
    vector<Page> above;

    for(int i = 0; i < page.size() - 1; i++) {
        if(page.at(i).get_id() == id) {
            for(int j = i + 1; j < page.size(); j++) {
                if(page.at(i).is_overlapped(page.at(j))) {
                    above.push_back(page.at(j));
                    for(int k = i + 1; k < j; k++) {
                        if(page.at(j).is_overlapped(page.at(k))) {
                            above.pop_back();
                            break;
                        }
                    }
                }
            }
        }
    }

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
