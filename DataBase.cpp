#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include "DataBase.h"
#include "CinemaStudio.hpp"
#include "Input.h"

void DataBase::init() {
    FILE *banks_file_m = fopen(STUDIO_DATA, "wb+");
    fclose(banks_file_m);
    FILE *filials_file_s = fopen(STUDIO_GARBAGE, "wb+");
    fclose(filials_file_s);
    FILE *indexes_file_i = fopen(PARTITION_DATA, "wb+");
    fclose(indexes_file_i);
    FILE *indexes_file_ = fopen(STUDIO_INDEX, "wb+");
    fclose(indexes_file_);
    FILE *indexes_fi = fopen(PARTITION_GARBAGE, "wb+");
    fclose(indexes_fi);

    for (int i = 0; i < STUDIO_GARBAGE_MAX_SIZE; i++) {
        studios_garbage[i] = -1;
        partitions_garbage[i] = -1;
    }
}


void DataBase::load_studio_garbage(bool mode) {
    FILE *collector_file = fopen(STUDIO_GARBAGE, mode ? "rb+" : "wb+");

    for (int i = 0; i < del_studios; i += 1) {
        mode ? fread(&studios_garbage[i], sizeof(int), 1, collector_file) : fwrite(&studios_garbage[i], sizeof(int),
                                                                                   1,
                                                                                   collector_file);
    }

    fclose(collector_file);
}

void DataBase::load_partition_garbage(bool mode) {
    FILE *collector_file = fopen(PARTITION_GARBAGE, mode ? "rb+" : "wb+");

    for (int i = 0; i < del_partitions; i += 1) {
        mode ? fread(&partitions_garbage[i], sizeof(int), 1, collector_file) : fwrite(&partitions_garbage[i], sizeof(int),
                                                                                      1,
                                                                                      collector_file);
    }

    fclose(collector_file);
}

void DataBase::load_index_table(bool mode) {
    FILE *collector_file = fopen(STUDIO_INDEX, mode ? "rb+" : "wb+");

    for (int i = 0; i < cnt_studios; i += 1) {
        mode ? fread(&studios_index[i].id, sizeof(int), 1, collector_file) : fwrite(&studios_index[i].id, sizeof(int),
                                                                                    1,
                                                                                    collector_file);
        mode ? fread(&studios_index[i].address, sizeof(int), 1, collector_file) : fwrite(&studios_index[i].address,
                                                                                         sizeof(int),
                                                                                         1, collector_file);
        mode ? fread(&studios_index[i].exists, sizeof(int), 1, collector_file) : fwrite(&studios_index[i].exists,
                                                                                        sizeof(int),
                                                                                        1, collector_file);
    }

    fclose(collector_file);
}

void DataBase::sort_index_table() {
    std::qsort(studios_index,
               cnt_studios, sizeof(Index),
               [](const void *a, const void *b) {
                   return (((Index *) a)->id - ((Index *) b)->id);
               }
    );
}

int DataBase::get_address(int key) {

    for (int i = 0; i < real_cnt_studios; i += 1) {
        if (studios_index[i].id == key && studios_index[i].exists == 1) {
            return studios_index[i].address;
        }
    }

    return -1;

}

void DataBase::insert_m(CinemaStudio *studio) {
    FILE *studio_file = fopen(STUDIO_DATA, "r+b");

    DataBase::load_index_table(true);
    studios_index[real_cnt_studios].id = studio->id;
    real_cnt_studios++;
    cnt_studios++;

    if (studios_garbage[0] == -1) {
        studios_index[real_cnt_studios - 1].address = (real_cnt_studios - 1) * sizeof(CinemaStudio);
        fseek(studio_file, (real_cnt_studios - 1) * sizeof(CinemaStudio), SEEK_SET);

        fwrite(&studio->id, sizeof(int), 1, studio_file);
        fwrite(&studio->name, sizeof(studio->name), 1, studio_file);
        fwrite(&studio->yearOfFoundation, sizeof(int), 1, studio_file);
        fwrite(&studio->address, sizeof(studio->address), 1, studio_file);
        fwrite(&studio->isPresent, sizeof(int), 1, studio_file);
        fwrite(&studio->firstPartition, sizeof(int), 1, studio_file);

        sort_index_table();
        load_index_table(false);
        fclose(studio_file);
    } else {
        studios_index[real_cnt_studios - 1].address = studios_garbage[0];
        fseek(studio_file, studios_garbage[0], SEEK_SET);

        fwrite(&studio->id, sizeof(int), 1, studio_file);
        fwrite(&studio->name, sizeof(studio->name), 1, studio_file);
        fwrite(&studio->yearOfFoundation, sizeof(int), 1, studio_file);
        fwrite(&studio->address, sizeof(studio->address), 1, studio_file);
        fwrite(&studio->isPresent, sizeof(int), 1, studio_file);
        fwrite(&studio->firstPartition, sizeof(int), 1, studio_file);

        shift_studio_garbage();
        sort_index_table();
        load_index_table(false);
        fclose(studio_file);
    }

}

bool DataBase::get_m(int id, struct CinemaStudio *studio) {
    int skip = get_address(id);
    if (skip == -1) {
        printf("Studio with this id doesn't exist!  \n");
        return false;
    }

    FILE *studio_file = fopen(STUDIO_DATA, "r+b");
    fseek(studio_file, skip, SEEK_SET);

    bool isPresent = true;
    fread(&studio->id, sizeof(int), 1, studio_file);
    fread(&studio->name, 20, 1, studio_file);
    fread(&studio->yearOfFoundation, sizeof(int), 1, studio_file);
    fread(&studio->address, 20, 1, studio_file);
    fread(&studio->isPresent, sizeof(int), 1, studio_file);
    fread(&studio->firstPartition, sizeof(int), 1, studio_file);
    fclose(studio_file);
    return true;
}

void DataBase::delete_m(int id) {
    int tmp = get_address(id);
    int tmpIndex = get_index(id);
    if (tmp == -1) {
        printf("Studio with this id doesn't exist! \n");
        return;
    }

    studios_index[tmpIndex].exists = 0;
    add_studio_garbage_address(tmp);
    load_studio_garbage(false);

    FILE *studio_file = fopen(STUDIO_DATA, "rb+");

    fseek(studio_file, tmp + 48, SEEK_SET);
    int result = 0;
    fwrite(&result, sizeof(int), 1, studio_file);
    cnt_studios--;
    del_studios++;
    fclose(studio_file);
    load_index_table(false);
}

void DataBase::update_m(CinemaStudio studio) {
    int skip = get_address(studio.id);
    if (skip == -1) {
        printf("Studio with this id doesn't exist! \n");
        return;
    }

    FILE *studio_file = fopen(STUDIO_DATA, "r+b");
    fseek(studio_file, skip, SEEK_SET);
    fwrite(&studio.id, sizeof(int), 1, studio_file);
    fwrite(&studio.name, 20, 1, studio_file);
    fwrite(&studio.yearOfFoundation, sizeof(int), 1, studio_file);
    fwrite(&studio.address, 20, 1, studio_file);

    fclose(studio_file);
}

void DataBase::insert_s(Partition partition) {
    CinemaStudio studio;
    this->get_m(partition.cinemaId, &studio);

    FILE *partition_file = fopen(PARTITION_DATA, "rb+");

    if (studio.firstPartition == -1) {
        studio.firstPartition = (cnt_partitions + del_partitions) * (int) sizeof(Partition);
        int skip = get_address(studio.id);
        if (skip == -1) {
            printf("Studio with this id doesn't exist! \n");
            return;
        }

        FILE *studio_file = fopen(STUDIO_DATA, "r+b");
        fseek(studio_file, skip + 52, SEEK_SET);
        fwrite(&studio.firstPartition, 4, 1, studio_file);
        fclose(studio_file);

        fseek(partition_file, 0, SEEK_END);
        fwrite(&partition, sizeof(Partition), 1, partition_file);
    } else {
        Partition tempPartition;
        fseek(partition_file, studio.firstPartition, SEEK_SET);
        fread(&tempPartition, sizeof(Partition), 1, partition_file);
        if (tempPartition.isPresent == 0) {
            fseek(partition_file, studio.firstPartition, SEEK_SET);
            fwrite(&partition.id, sizeof(int), 1, partition_file);
            fwrite(&partition.name, 20, 1, partition_file);
            fwrite(&partition.address, 20, 1, partition_file);
            fwrite(&partition.numberOfWorkers, sizeof(int), 1, partition_file);
            fwrite(&partition.cinemaId, sizeof(int), 1, partition_file);
            fseek(partition_file, 4, SEEK_CUR);
            fwrite(&partition.isPresent, sizeof(int), 1, partition_file);
            fclose(partition_file);
            del_partitions--;
            cnt_partitions++;
            return;
        }
        int address = tempPartition.nextPartitionAddress;
        while (tempPartition.nextPartitionAddress != -1) {
            fseek(partition_file, tempPartition.nextPartitionAddress, SEEK_SET);
            fread(&tempPartition, sizeof(Partition), 1, partition_file);
            if (tempPartition.isPresent == 0) {
                fseek(partition_file, address, SEEK_SET);
                fwrite(&partition.id, sizeof(int), 1, partition_file);
                fwrite(&partition.name, 20, 1, partition_file);
                fwrite(&partition.address, 20, 1, partition_file);
                fwrite(&partition.numberOfWorkers, sizeof(int), 1, partition_file);
                fwrite(&partition.cinemaId, sizeof(int), 1, partition_file);
                fseek(partition_file, 4, SEEK_CUR);
                fwrite(&partition.isPresent, sizeof(int), 1, partition_file);
                fclose(partition_file);
                del_partitions--;
                cnt_partitions++;
                return;

            }
            address = tempPartition.nextPartitionAddress;
        }
        tempPartition.nextPartitionAddress = (cnt_partitions + del_partitions) * (int) sizeof(Partition);
        this->update_s_with_address(tempPartition);

        fseek(partition_file, 0, SEEK_END);
        fwrite(&partition, sizeof(Partition), 1, partition_file);
    }
    cnt_partitions++;
    fclose(partition_file);
}

bool DataBase::get_s(int parent_id, int child_id, struct Partition *partition) {
    CinemaStudio studio;
    this->get_m(parent_id, &studio);

    if (studio.firstPartition == -1) {
        printf("Studio doesn't have partitions. \n");
        return false;
    } else {
        FILE *partition_file = fopen(PARTITION_DATA, "rb+");
        fseek(partition_file, studio.firstPartition, SEEK_SET);
        fread(partition, sizeof(Partition), 1, partition_file);
        if (partition->id == child_id) {
            fclose(partition_file);
            return true;
        }
        while (partition->nextPartitionAddress != -1) {
            fseek(partition_file, partition->nextPartitionAddress, SEEK_SET);
            fread(partition, sizeof(Partition), 1, partition_file);
            if (partition->id == child_id) {
                fclose(partition_file);
                return true;
            }
        }
        printf("No such partition \n");
        return false;
    }
}

void DataBase::delete_s(int parent_id, int child_id) {
    CinemaStudio studio;
    this->get_m(parent_id, &studio);

    if (studio.firstPartition == -1) {
        printf("Studio doesn't have partitions. \n");
        return;
    } else {
        Partition partition;
        FILE *partition_file = fopen(PARTITION_DATA, "rb+");
        fseek(partition_file, studio.firstPartition, SEEK_SET);
        fread(&partition, sizeof(Partition), 1, partition_file);
        int isPresent = 0;
        if (partition.id == child_id) {
            fseek(partition_file, studio.firstPartition + 56, SEEK_SET);
            fwrite(&isPresent, sizeof(int), 1, partition_file);
            fclose(partition_file);
            return;
        }
        while (partition.nextPartitionAddress != -1) {
            Partition prevPartitions = partition;
            fseek(partition_file, partition.nextPartitionAddress, SEEK_SET);
            fread(&partition, sizeof(Partition), 1, partition_file);
            if (partition.id == child_id) {
                fseek(partition_file, prevPartitions.nextPartitionAddress + 56, SEEK_SET);
                fwrite(&isPresent, sizeof(int), 1, partition_file);
                fclose(partition_file);
                return;
            }

        }
        printf("No such partition \n");
    }
    del_partitions++;
    cnt_partitions--;
}

void DataBase::ut_m() {
    FILE *studio_file_m = fopen(STUDIO_DATA, "rb+");
    printf("ut_m:\n");
    int id;

    char name[20];
    int yearOfFoundation;
    char address[20];

    int isPresent = true;
    int firstPartition = -1;


    for (int i = 0; i < real_cnt_studios; i++) {
        if (studios_index[i].exists == 1) {
            fseek(studio_file_m, get_address(studios_index[i].id), SEEK_SET);
            fread(&id, sizeof(int), 1, studio_file_m);
            fread(&name, sizeof(name), 1, studio_file_m);
            fread(&yearOfFoundation, sizeof(int), 1, studio_file_m);
            fread(&address, sizeof(address), 1, studio_file_m);
            fread(&isPresent, sizeof(int), 1, studio_file_m);
            fread(&firstPartition, sizeof(int), 1, studio_file_m);
            printf("Id: %d, Name: %s, yearOfFoundation: %d, name: %s , isPresent: %d, firstPartition: %d \n", id, name,
                   yearOfFoundation, address, isPresent, firstPartition);
        }
    }

    fclose(studio_file_m);
}

void DataBase::update_s_with_address(Partition newPartition) {
    CinemaStudio studio;
    this->get_m(newPartition.cinemaId, &studio);

    if (studio.firstPartition == -1) {
        printf("Studio doesn't have partitions. \n");
        return;
    } else {
        Partition partition;
        FILE *partition_file = fopen(PARTITION_DATA, "rb+");
        fseek(partition_file, studio.firstPartition, SEEK_SET);
        fread(&partition, sizeof(Partition), 1, partition_file);
        if (partition.id == newPartition.id) {
            fseek(partition_file, studio.firstPartition, SEEK_SET);
            fwrite(&newPartition.id, sizeof(int), 1, partition_file);
            fwrite(&newPartition.name, 20, 1, partition_file);
            fwrite(&newPartition.address, 20, 1, partition_file);
            fwrite(&newPartition.numberOfWorkers, sizeof(int), 1, partition_file);
            fwrite(&newPartition.cinemaId, sizeof(int), 1, partition_file);
            fwrite(&newPartition.nextPartitionAddress, sizeof(int), 1, partition_file);
            fclose(partition_file);
            return;
        }
        while (partition.nextPartitionAddress != -1) {
            Partition prevPartition = partition;
            fseek(partition_file, partition.nextPartitionAddress, SEEK_SET);
            fread(&partition, sizeof(Partition), 1, partition_file);
            if (partition.id == newPartition.id) {
                fseek(partition_file, prevPartition.nextPartitionAddress, SEEK_SET);
                fwrite(&newPartition.id, sizeof(int), 1, partition_file);
                fwrite(&newPartition.name, 20, 1, partition_file);
                fwrite(&newPartition.address, 20, 1, partition_file);
                fwrite(&newPartition.numberOfWorkers, sizeof(int), 1, partition_file);
                fwrite(&newPartition.cinemaId, sizeof(int), 1, partition_file);
                fwrite(&newPartition.nextPartitionAddress, sizeof(int), 1, partition_file);
                fclose(partition_file);
                return;
            }

        }
        printf("No such partition \n");
    }
}

void DataBase::update_s(Partition newPartition) {
    CinemaStudio studio;
    this->get_m(newPartition.cinemaId, &studio);

    if (studio.firstPartition == -1) {
        printf("Studio doesn't have partitions. \n");
        return;
    } else {
        Partition partition;
        FILE *partition_file = fopen(PARTITION_DATA, "rb+");
        fseek(partition_file, studio.firstPartition, SEEK_SET);
        fread(&partition, sizeof(Partition), 1, partition_file);
        if (partition.id == newPartition.id) {
            fseek(partition_file, studio.firstPartition, SEEK_SET);
            fwrite(&newPartition.id, sizeof(int), 1, partition_file);
            fwrite(&newPartition.name, 20, 1, partition_file);
            fwrite(&newPartition.address, 20, 1, partition_file);
            fwrite(&newPartition.numberOfWorkers, sizeof(int), 1, partition_file);
            fwrite(&newPartition.cinemaId, sizeof(int), 1, partition_file);
            fclose(partition_file);
            return;
        }
        while (partition.nextPartitionAddress != -1) {
            Partition prevPartition = partition;
            fseek(partition_file, partition.nextPartitionAddress, SEEK_SET);
            fread(&partition, sizeof(Partition), 1, partition_file);
            if (partition.id == newPartition.id) {
                fseek(partition_file, prevPartition.nextPartitionAddress, SEEK_SET);
                fwrite(&newPartition.id, sizeof(int), 1, partition_file);
                fwrite(&newPartition.name, 20, 1, partition_file);
                fwrite(&newPartition.address, 20, 1, partition_file);
                fwrite(&newPartition.numberOfWorkers, sizeof(int), 1, partition_file);
                fwrite(&newPartition.cinemaId, sizeof(int), 1, partition_file);
                fclose(partition_file);
                return;
            }

        }
        printf("No such partition \n");
    }
}

void DataBase::ut_s(int studioId) {
    CinemaStudio studio;
    this->get_m(studioId, &studio);
    Partition partition;

    if (studio.firstPartition == -1) {
        printf("Studio doesn't have partitions. \n");
        return;
    } else {
        FILE *partition_file = fopen(PARTITION_DATA, "rb+");
        fseek(partition_file, studio.firstPartition, SEEK_SET);
        fread(&partition, sizeof(Partition), 1, partition_file);
        printPartition(partition);
        while (partition.nextPartitionAddress != -1) {
            fseek(partition_file, partition.nextPartitionAddress, SEEK_SET);
            fread(&partition, sizeof(Partition), 1, partition_file);
            if(partition.isPresent) printPartition(partition);
        }
    }
}

int DataBase::get_index(int key) {
    for (int i = 0; i < real_cnt_studios; i += 1) { // can be faster --> log(cnt_banks)
        if (studios_index[i].id == key && studios_index[i].exists == 1) {
            return i;
        }
    }
    return -1;
}

void DataBase::shift_studio_garbage() {
    int i = 0;
    while (studios_garbage[i] != -1) {
        studios_garbage[i] = studios_garbage[i + 1];
        i++;
    }
}

void DataBase::shift_partition_garbage() {
    int i = 0;
    while (partitions_garbage[i] != -1) {
        partitions_garbage[i] = partitions_garbage[i + 1];
        i++;
    }
}

void DataBase::add_studio_garbage_address(int address) {
    int i = 0;
    while (studios_garbage[i] != -1) {
        i++;
    }
    studios_garbage[i] = address;
}

void DataBase::add_partition_garbage_address(int address) {
    int i = 0;
    while (partitions_garbage[i] != -1) {
        i++;
    }
    partitions_garbage[i] = address;
}


