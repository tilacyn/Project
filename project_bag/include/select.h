#include "records.h"

class Select{
public:
    // новый оффсет сообщений, которые нам подходят
    std::map <int, int> new_offset;

    // У нас на каждый Chunk приходится по Селекту, создадим Select для конкретного Chunk.
    void make_map(puint time_start, puint time_end, std::string topic, bool topic_regex, std::string msg_regex, Chunk& c,
                  std::map<int, Connection>& map_conn, std::ifstream& ifs);

    // Для каждой IndexData просмотрим все сообщения, указанные в data и будем модифицировать.
    void sift_index_data(long long chunk_data_start, puint time_start, puint time_end, std::string msg_regex,
                         IndexData& id, std::ifstream& ifs);
};

class BagFile{
public:
    std::vector<Chunk> chunks;
    //std::vector<Connection> connections;
    std::vector<Select> selects;
    BagHeader bh;
    std::vector<ChunkInfo> chunkinfo;
    std::map <int, Connection> map_conn;
};


// Чтобы сделать правильную chunk_new_data_len
void sift_conn_in_chunk(Chunk& c, std::ifstream& ifs);
void make_map(puint time_start, puint time_end, std::string topic, bool topic_regex,
              std::string msg_regex, BagFile& bf, std::ifstream& ifs);

// Чтобы посчитать conn_count

void count_conns(BagFile& bf);
