#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;
using std::ifstream;
using std::to_string;


void judge(string submit_file, string answer_file) {
    ifstream submit, answer;
    char submit_input, answer_input;
    bool isCorrect = true;
    
    submit.open(submit_file);
    answer.open(answer_file);

    while(submit >> submit_input && answer >> answer_input) {
        if(submit_input != answer_input) {
            cout << submit_input << " is different from " << answer_input << endl;
            isCorrect = false;
        }
    }
    if(isCorrect) {
        cout << submit_file << " is correct!!" << endl;
    }
}

void test(string submit_dir, string answer_dir) {
    for(int i = 0; i < 10; i++) {
        judge(submit_dir+"/output_"+to_string(i)+".txt", answer_dir+"/output_"+to_string(i)+".txt");
    }
}

int main(int argc, char *argv[])
{
    if (argc==3) test(string(argv[1]), string(argv[2]));
    else cerr << "Wrong arguments" << endl;

    return 0;
}