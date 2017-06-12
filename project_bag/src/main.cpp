#include "main_functions.h"
#include <exception>


using namespace std;

BagFile bf;

class simple_exception{
public:
    const string str;
    string what() const { return str;}
    simple_exception(string s) : str(s) {};
};

unsigned int atoui(const char* s){
    unsigned int n = 0;
    for(unsigned int i = 0; i < strlen(s); i++){
        n *= 10;
        n += static_cast<unsigned int>(s[i] - '0');
    }
    return n;
}

void name_cpy(string s, char* arr){
    for(unsigned int i = 0; i < s.length(); i++){
        arr[i] = s[i];
    }
}


int main(int argc, char* argv[]){
    char* in_file = new char[100];
    char* out_file = new char[100];
    puint time_start = {0, 0};
    puint time_end = {UINT_MAX, UINT_MAX};
    bool topic_regex = false;
    string msg_regex;
    string topic;

    try{
        if(argc < 2) throw simple_exception("No nessecary keys found");
        if(!strcmp(argv[1], "-f")){
            if(argc < 3) throw simple_exception("Filename not found");
            ifstream ifs(argv[2]);
            while(ifs.peek() != std::ifstream::traits_type::eof()){
                string field;
                ifs >> field;
                if(field == "in"){
                    if(ifs.peek() == std::ifstream::traits_type::eof()) throw simple_exception("Infilename not found");
                    string in_file_str;
                    ifs >> in_file_str;
                    strcpy(in_file, in_file_str.c_str());
                } else if (field == "out"){
                    if(ifs.peek() == std::ifstream::traits_type::eof()) throw simple_exception("Outfilename not found");
                    string out_file_str;
                    ifs >> out_file_str;
                    cout << "Important: " << out_file_str << "\n";
                    //name_cpy(out_file_str, out_file);
                    strcpy(out_file, out_file_str.c_str());
                } else if(field == "topic"){
                    if(ifs.peek() == std::ifstream::traits_type::eof()) throw simple_exception("Topic not found");
                    ifs >> topic;
                    if(topic == "regex"){
                        topic_regex = true;
                        ifs >> topic;
                    }
                }else if(field == "time"){
                    if(ifs.peek() == std::ifstream::traits_type::eof()) throw simple_exception("Time not found");
                    ifs >> time_start.first >> time_start.second >> time_end.first >> time_end.second;
                } else if(field == "regex"){
                    if(ifs.peek() == std::ifstream::traits_type::eof()) throw simple_exception("Infilename not found");
                    ifs >> msg_regex;
                }
            }
            ifs.close();
        }
        else for(int i = 1; i < argc; i++){
            if(!strcmp(argv[i], "--in") || !strcmp(argv[i], "-i")){
                if(i >= argc - 1) throw simple_exception("Infilename not found");
                strcpy(in_file, argv[++i]);
            }else if(!strcmp(argv[i], "--out") || !strcmp(argv[i], "-o")){
                if(i >= argc - 1) throw simple_exception("Outfilename not found");
                strcpy(out_file, argv[++i]);
            } else if(!strcmp(argv[i], "--time") || !strcmp(argv[i], "-t")){
                if(i >= argc - 1) throw simple_exception("Time_start not found");
                time_start.first = atoui(argv[++i]);

                if(i >= argc - 1) throw simple_exception("Time_start not found");
                time_start.second = atoui(argv[++i]);

                if(i >= argc - 1) throw simple_exception("Time_end not found");
                time_end.first = atoui(argv[++i]);

                if(i >= argc - 1) throw simple_exception("Time_end not found");
                time_end.second = atoui(argv[++i]);
            } else if(!strcmp(argv[i], "--topic")){
                if(i >= argc - 1) throw simple_exception("Topic not found");
                if(!strcmp(argv[i + 1], "-r") || !strcmp(argv[i + 1], "--regex")){
                    topic_regex = true;
                    i++;
                }
                if(i >= argc - 1) throw simple_exception("Topic not found");
                topic = make_str(argv[++i]);
            } else if(!strcmp(argv[i], "--msgfilter=regex")){
                if(i >= argc - 1) throw simple_exception("Message regex not found");
                msg_regex = make_str(argv[++i]);
            }
        }
    }
    catch(simple_exception& e){
        cerr << e.what() << "\n";
        return 0;
    }

    read_records(in_file, bf);
    select_data(in_file, time_start, time_end, topic, topic_regex, msg_regex, bf);
    write_file(in_file, out_file, bf);
    delete[] in_file;
    delete[] out_file;

    return 0;
}
