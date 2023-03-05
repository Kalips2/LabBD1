//
// Created by Lenovo on 21/02/2023.
//


#include <cstdio>
#include <cstring>
#include "CinemaStudio.hpp"
#include "Partition.hpp"

void inputPartition(struct Partition *partition) {
    int id;
    char name[20];
    char address[20];
    int numberOfWorkers;
    int cinemaId;

    printf("Enter partition ID: ");
    scanf("%d", &id);
    partition->id = id;

    name[0] = '\0';
    printf("Enter partition\'s name: ");
    scanf("%s", name);
    strcpy(partition->name, name);

    address[0] = '\0';
    printf("Enter partition\'s address: ");
    scanf("%s", address);
    strcpy(partition->address, address);

    printf("Enter partition\'s number of workers: ");
    scanf("%d", &numberOfWorkers);
    partition->numberOfWorkers = numberOfWorkers;

    printf("Enter customer ID: ");
    scanf("%d", &cinemaId);
    partition->cinemaId = cinemaId;
}

void inputStudio(struct CinemaStudio *studio) {
    int id;
    char name[20];
    int yearOfFoundation;
    char address[20];

    printf("Enter studio ID: ");
    scanf("%d", &id);
    studio->id = id;

    name[0] = '\0';
    printf("Enter studio\'s name: ");
    scanf("%s", name);
    strcpy(studio->name, name);

    printf("Enter studio\'s year of foundation: ");
    scanf("%d", &yearOfFoundation);
    studio->yearOfFoundation = yearOfFoundation;

    address[0] = '\0';
    printf("Enter studio\'s address: ");
    scanf("%s", address);
    strcpy(studio->address, address);
}

void printStudio(struct CinemaStudio studio) {
    printf("CinemaStudio\'s id: %d\n", studio.id);
    printf("CinemaStudio\'s name: %s\n", studio.name);
    printf("CinemaStudio\'s year of foundation: %d\n", studio.yearOfFoundation);
    printf("CinemaStudio\'s address: %s\n", studio.address);
    printf("CinemaStudio\'s isPresent: %d\n", studio.isPresent);
    printf("CinemaStudio\'s first order name: %d\n", studio.firstPartition);
}

void printPartition(struct Partition partition) {
    printf("Partition id: %d \n", partition.id);
    printf("Partition name: %s\n", partition.name);
    printf("Partition address: %s\n", partition.address);
    printf("Partition number of workers: %d\n", partition.numberOfWorkers);
    printf("Partition cinemaId: %d\n", partition.cinemaId);
    printf("Partition nextLogicalAddress: %d\n", partition.nextPartitionAddress);
    printf("Partition isPresent: %d\n", partition.isPresent);
    printf("-------------------------\n");
}