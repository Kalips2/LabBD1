#pragma once
#include <stdio.h>
#include <string.h>
#include "Partition.hpp"
#include "CinemaStudio.hpp"

void inputOrder(struct Order* order) {
    char address[20];
    char status[20];
    char pizzaName[20];
    int customerId;

    address[0] = '\0';
	printf("Enter order\'s address: ");
	scanf("%s", address);
	strcpy(order->address, address);

    pizzaName[0] = '\0';
    printf("Enter order\'s pizza name: ");
    scanf("%s", pizzaName);
    strcpy(order->pizzaName, pizzaName);

    printf("Enter customer ID: ");
    scanf("%d", &customerId);
    order->customerId = customerId;
}

void inputCustomer(struct Customer* customer) {
    char name[20];
    int bonusPoints;
    char defaultAddress[20];

    name[0] = '\0';
    printf("Enter customer\'s name: ");
    scanf("%s", name);
    strcpy(customer->name, name);

    printf("Enter customer bonus points: ");
    scanf("%d", &bonusPoints);
    customer->bonusPoints = bonusPoints;

    defaultAddress[0] = '\0';
    printf("Enter customers\'s default: ");
    scanf("%s", defaultAddress);
    strcpy(customer->defaultAddress, defaultAddress);
}