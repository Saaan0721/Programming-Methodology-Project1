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

        // job functions
        void insert_page(int x, int y, int width, int height, int id, int content);
        void insert_page(Page p); // overloading
        void delete_page(int id);
        void delete_page(Page p); // overloading
        void modify_content(int id, char content);
        void modify_position(int id, int x, int y);

        // my functions

        void draw_board();
        // Postcondition: board is drawn using page vector.

        vector<Page> above_page(int id);
        // Precondition: id of page
        // find all pages above page of given id and sort pages based on id,
        // return a vector of page above.

        Page find_page(int id); 
        // Precondition: id of page
        // return a page found in page vector.

        int find_index(int id);
        // Precondition: id of page
        // return an index in page vector.

        void clear_stack(); 
        // Postcondition: clear page_stack.

        void insert_stack(); 
        // Postcondition: insert pages in page_stack.

    private:
        int num_jobs, width, height; 
        ofstream& output; 
        char* board; 

        // my variables
        vector<Page> page; // pages on board
        stack<Page> page_stack; // pages to be insert(used in delete())
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
}

Board::~Board() {
    delete board;
    
}


void Board::print_board() {
    int h, w;

    draw_board(); // draw a board everytime before print a board.

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

    // base case: if a page don't have above pages, delete the page and print a board.
    if(above.size() == 0) {
        page.erase(page.begin() + find_index(id));
        print_board();
    }
    // recursive step
    else {
        // delete all above pages.
        for(Page p:above) {
            delete_page(p);
        }

        // delete the page and print a board.
        page.erase(page.begin() + find_index(id));
        print_board();

        // insert pages in page_stack.
        while(!page_stack.empty()) {
            insert_page(page_stack.top());
            page_stack.pop();
        }
    }
}

// overloading, but this func don't insert pages.
void Board::delete_page(Page p) {
    vector<Page> above = above_page(p.get_id());

    // if p doesn't exit in page, terminate the func.
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

    clear_stack();

    // base case: if page don't have above pages, modify a content of the page and print a board.
    if(above.size() == 0) {
        page.erase(page.begin() + find_index(target_page.get_id()));
        print_board();
        target_page.set_content(content);
        insert_page(target_page);
    }
    // recursive step
    else {
        // delete all above pages.
        for(Page p:above) {
            delete_page(p);
        }

        // modify a content of the page and print a board.
        page.erase(page.begin() + find_index(target_page.get_id()));
        print_board();
        target_page.set_content(content);
        insert_page(target_page);
        
        // insert above pages in page_stack.
        while(!page_stack.empty()) {
            insert_page(page_stack.top());
            page_stack.pop();
        }
    }
}

void Board::modify_position(int id, int x, int y) {
    vector<Page> above = above_page(id);
    Page target_page = find_page(id);

    clear_stack();

    // base case: if page don't have above pages, modify a position of the page and print a board.
    if(above.size() == 0) {
        page.erase(page.begin() + find_index(target_page.get_id()));
        print_board();
        target_page.set_x(x);
        target_page.set_y(y);
        insert_page(target_page);
    }
    // recursive step
    else {
        // delete all above pages.
        for(Page p:above) {
            delete_page(p);
        }

        // modify a position of the page and print a board.
        page.erase(page.begin() + find_index(target_page.get_id()));
        print_board();
        target_page.set_x(x);
        target_page.set_y(y);
        insert_page(target_page);

        // insert above pages in page_stack.
        while(!page_stack.empty()) {
            insert_page(page_stack.top());
            page_stack.pop();
        }
    } 
}

void Board::draw_board() {
    // initialize the board to blank.
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
        }
    }
    // put all pages on the board.
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
    
    // for every point, check if another page is above.
    for(int y = target_page.get_y(); y < target_page.get_y() + target_page.get_height(); y++) {
        for(int x = target_page.get_x(); x < target_page.get_x() + target_page.get_width(); x++) {
            for(int i = find_index(id) + 1; i < page.size(); i++) {

                // "overlapped" means two pages are close enough for "above"
                // it only check distance between page, not depth.
                if(page.at(i).is_overlapped(x, y)) {

                    // if page.at(i) is not in above, push back page.at(i) in above.
                    if(find(above.begin(), above.end(), page.at(i)) == above.end()) {
                        above.push_back(page.at(i));
                    }

                    // do not check this point because the point is already overlapped.
                    break;
                }
            }
        }
    }

    // sort above pages based on id.
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
