#pragma once

struct Partition {
    int id;
    char name[20];
    char address[20];
    int numberOfWorkers;
    int cinemaId;

    int nextPartitionAddress = -1;
    int isPresent = true;
};