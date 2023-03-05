#pragma once

#include <stdio.h>
#include <string.h>
#include "Partition.hpp"
#include "CinemaStudio.hpp"

void inputPartition(struct Partition *partition);

void inputStudio(struct CinemaStudio *studio);

void printStudio(struct CinemaStudio studio);

void printPartition(struct Partition partition);