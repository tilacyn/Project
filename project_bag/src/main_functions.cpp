#include "main_functions.h"


using namespace std;

char* first_symbols = new char[20];


void read_records(const char* in_file, BagFile& bf){
    std::ifstream ifs(in_file, std::ios::binary);
    readv(first_symbols, 13);
    while(ifs.peek() != std::ifstream::traits_type::eof()){
        char op = read_op(ifs);
        if(op == 0x02){
            MessageData md;
            ifs >> md;
        } else if(op == 0x04){
            IndexData id;
            ifs >> id;
            assert(!bf.chunks.empty());
            bf.chunks[bf.chunks.size() - 1].indexdata[id.conn] = id;
        } else if(op == 0x06){
            ChunkInfo ci;
            ifs >> ci;
            //cout << "Chunk Info: " << ci.data_end() << " " << ci.pos << "\n";
            bf.chunkinfo.push_back(ci);
        } else if(op == 0x03){
            ifs >> bf.bh;
            //cout << "Bag Header: " << bf.bh.data_end() << "\n";
        } else if(op == 0x05){
            Chunk c;
            ifs >> c;
            //cout << "Chunk: " << c.header_len << " " << c.data_len << " " << c.pos << "\n";
            bf.chunks.push_back(c);
        } else if(op == 0x07){
            Connection c;
            ifs >> c;
            //cout << "Connection: " << c.conn << " " << c.topic << "\n";
            //bf.connections.push_back(c);
            bf.map_conn[c.conn] = c;
            bf.map_conn[c.conn].has_any_suitable_messages = false;
        }
    }
    cout << "Reading finished\n";
    seq_chunk_to_info(bf.chunks, bf.chunkinfo);
    ifs.close();
}

void select_data(const char* in_file, puint time_start, puint time_end,
                 string topic, bool topic_regex, string msg_regex, BagFile& bf){
    ifstream ifs(in_file, ios::binary);
    make_map(time_start, time_end, topic, topic_regex, msg_regex, bf, ifs);
    cout << "Map Made\n";
    ifs.close();
}

void write_file(const char* in_file, const char* out_file, BagFile& bf){
    ifstream ifs(in_file, ios::binary);
    ofstream ofs(out_file, ios::binary);

    writev(first_symbols, 13);
    write(bf.bh, ifs, ofs);
    write(bf.selects, bf.chunks, ifs, ofs);
    write(bf.map_conn, ifs, ofs);
    write_all_the_chunk_info(bf.chunks, ifs, ofs);
    cout << "New file created\n";
}
