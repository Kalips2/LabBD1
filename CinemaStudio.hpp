#pragma once

struct CinemaStudio {
    int id;

    char name[20];
    int yearOfFoundation;
    char address[20];

    int isPresent = 1;
    int firstPartition = -1;
};