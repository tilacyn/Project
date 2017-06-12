#include "write.h"


void read_records(const char* in_file, BagFile& bf);

void select_data(const char* in_file, puint time_start, puint time_end,
                 std::string topic, bool topic_regex, std::string msg_regex, BagFile& bf);


void write_file(const char* in_file, const char* out_file, BagFile& bf);
