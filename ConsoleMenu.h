#ifndef UNTITLED3_CONSOLEMENU_H
#define UNTITLED3_CONSOLEMENU_H

#include <cstdio>
#include "Partition.hpp"
#include "CinemaStudio.hpp"
#include "Input.h"
#include "DataBase.h"

class ConsoleMenu {
    DataBase db;

    void dbInit() {
        db.init();

        CinemaStudio studio;
        studio.id = 1;
        strcpy(studio.name, "Studio 1");
        studio.yearOfFoundation = 2014;
        strcpy(studio.address, "Street 1");
        db.insert_m(&studio);

        Partition partition;
        partition.id = 1;
        strcpy(partition.name, "Partition 1");
        partition.numberOfWorkers = 17;
        strcpy(partition.address, "Address 1");
        partition.cinemaId = 1;
        db.insert_s(partition);

        partition.id = 2;
        strcpy(partition.name, "Partition 2");
        partition.numberOfWorkers = 34;
        strcpy(partition.address, "Address 2");
        partition.cinemaId = 1;
        db.insert_s(partition);

        partition.id = 3;
        strcpy(partition.name, "Partition 3");
        partition.numberOfWorkers = 24;
        strcpy(partition.address, "Address 3");
        partition.cinemaId = 1;
        db.insert_s(partition);

        studio.id = 2;
        strcpy(studio.name, "Studio 2");
        studio.yearOfFoundation = 2009;
        strcpy(studio.address, "Street 2");
        db.insert_m(&studio);

        studio.id = 3;
        strcpy(studio.name, "Studio 3");
        studio.yearOfFoundation = 2001;
        strcpy(studio.address, "Street 3");
        db.insert_m(&studio);

        partition.id = 4;
        strcpy(partition.name, "Partition 4");
        partition.numberOfWorkers = 23;
        strcpy(partition.address, "Address 4");
        partition.cinemaId = 3;
        db.insert_s(partition);

        partition.id = 5;
        strcpy(partition.name, "Partition 5");
        partition.numberOfWorkers = 678;
        strcpy(partition.address, "Address 5");
        partition.cinemaId = 3;
        db.insert_s(partition);

        studio.id = 4;
        strcpy(studio.name, "Studio 4");
        studio.yearOfFoundation = 2010;
        strcpy(studio.address, "Street 4");
        db.insert_m(&studio);

        studio.id = 5;
        strcpy(studio.name, "Studio 5");
        studio.yearOfFoundation = 2005;
        strcpy(studio.address, "Street 5");
        db.insert_m(&studio);

        partition.id = 10;
        strcpy(partition.name, "Partition 10");
        partition.numberOfWorkers = 53;
        strcpy(partition.address, "Address 10");
        partition.cinemaId = 5;
        db.insert_s(partition);

        partition.id = 11;
        strcpy(partition.name, "Partition 11");
        partition.numberOfWorkers = 98;
        strcpy(partition.address, "Address 11");
        partition.cinemaId = 5;
        db.insert_s(partition);

        partition.id = 12;
        strcpy(partition.name, "Partition 12");
        partition.numberOfWorkers = 62;
        strcpy(partition.address, "Address 12");
        partition.cinemaId = 5;
        db.insert_s(partition);

        partition.id = 13;
        strcpy(partition.name, "Partition 13");
        partition.numberOfWorkers = 124;
        strcpy(partition.address, "Address 13");
        partition.cinemaId = 5;
        db.insert_s(partition);

        partition.id = 14;
        strcpy(partition.name, "Partition 14");
        partition.numberOfWorkers = 63;
        strcpy(partition.address, "Address 14");
        partition.cinemaId = 5;
        db.insert_s(partition);

        partition.id = 15;
        strcpy(partition.name, "Partition 15");
        partition.numberOfWorkers = 244;
        strcpy(partition.address, "Address 15");
        partition.cinemaId = 5;
        db.insert_s(partition);
    }

public:
    void run() {
        dbInit();
        struct CinemaStudio studio;
        struct Partition partition;
        while (1) {
            int choice;
            int id;
            char error[51];
            printf("==============================================\n");
            printf("Acceptable Options:\n0 Exit\n1 Insert studio\n2 Get studio\n3 Update studio\n4 Delete studio\n5 Insert partition\n6 Get partition\n7 Update partition\n8 Delete partition\n9 General Info\n10 General info about partitions \n");
            printf("==============================================\n");
            printf("Your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 0:
                    return;
                case 1:
                    inputStudio(&studio);
                    db.insert_m(&studio);
                    break;
                case 2:
                    printf("Enter studio ID: ");
                    scanf("%d", &id);
                    if(db.get_m(id, &studio)) printStudio(studio);
                    break;
                case 3:
                    inputStudio(&studio);
                    db.update_m(studio);
                    printf("Updated successfully\n");
                    break;
                case 4:
                    printf("Enter studio ID: ");
                    int studioId;
                    scanf("%d", &studioId);
                    db.delete_m(studioId);
                    break;
                case 5:
                    inputPartition(&partition);
                    db.insert_s(partition);
                    break;
                case 6:
                    printf("Enter studio ID: ");
                    scanf("%d", &studioId);
                    printf("Enter partition ID: ");
                    int partitionId;
                    scanf("%d", &partitionId);
                    if(db.get_s(studioId, partitionId, &partition)) printPartition(partition);
                    break;
                case 7:
                    inputPartition(&partition);
                    db.update_s(partition);
                    break;
                case 8:
                    printf("Enter studio ID: ");
                    studioId;
                    scanf("%d", &studioId);
                    printf("Enter partition ID: ");
                    partitionId;
                    scanf("%d", &partitionId);
                    db.delete_s(studioId, partitionId);
                    break;
                case 9:
                    db.ut_m();
                    break;
                case 10:
                    printf("Enter studio ID: ");
                    studioId;
                    scanf("%d", &studioId);
                    db.ut_s(studioId);
                    break;
                default:
                    printf("Invalid input, Try again!\n");

            }
        }
    }
};

#endif //UNTITLED3_CONSOLEMENU_H
