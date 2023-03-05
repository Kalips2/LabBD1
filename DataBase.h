//#pragma once
#pragma once

#ifndef UNTITLED3_DATABASE_H
#define UNTITLED3_DATABASE_H

#include "Index.hpp"
#include "Constants.h"
#include "CinemaStudio.hpp"
#include "Partition.hpp"

class DataBase {
private:
    Index studios_index[INDEX_MAX_SIZE]{};
    int studios_garbage[STUDIO_GARBAGE_MAX_SIZE]{};
    int partitions_garbage[PARTITION_GARBAGE_MAX_SIZE]{};


    int cnt_partitions = 0;
    int del_partitions = 0;

    int real_cnt_studios = 0;
    int cnt_studios = 0;
    int del_studios = 0;
public:
    int get_index(int id);

    void update_s(Partition newPartition);

    void ut_s(int studioId);

    void ut_m();

    void update_s_with_address(Partition newPartition);

    void delete_s(int parent_id, int child_id);

    bool get_s(int parent_id, int child_id, struct Partition *partition);

    void insert_s(Partition partition);

    void update_m(CinemaStudio studio);

    bool get_m(int id, struct CinemaStudio *studio);

    void delete_m(int id);

    void init();

    void load_studio_garbage(bool mode);

    void load_partition_garbage(bool mode);

    void load_index_table(bool mode);

    void sort_index_table();

    int get_address(int id);

    void insert_m(CinemaStudio *studio);

    void shift_studio_garbage();

    void shift_partition_garbage();

    void add_studio_garbage_address(int address);

    void add_partition_garbage_address(int address);
};

#endif //UNTITLED3_DATABASE_H
