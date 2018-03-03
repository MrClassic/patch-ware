/* **********************************************************************
 * File:   sineTable.h
 * Author: Nate Gallegos
 *
 * Log
 *      4/30/17
 *      File Created
 *      added accessors for sine, cosine and tangent (degrees and radians)
 *      TO DO: 
 *      arc and secant functions...?
 ********************************************************************* */


#include"sineTable.h"

double sineD(double angle) {
    //enforce positive angle
    while (angle < 0) {
        angle += 360.0;
    }
    //enforce first rotation value
    while (angle >= 360.0) {
        angle -= 360.0;
    }
    
    //get the number "between" table values
    int lowIndex = (int) ((angle / 360.0) * (double) TABLE_SIZE);
    int highIndex = lowIndex + 1;
    if(lowIndex >= TABLE_SIZE){
        lowIndex -= TABLE_SIZE;
    }
    if(highIndex >= TABLE_SIZE){
        highIndex -= TABLE_SIZE;
    }
    double high = sineTable[highIndex];
    double low = sineTable[lowIndex];
    double out = low;
    double additionFactor = (angle / 360.0) * (double) TABLE_SIZE;
    additionFactor -= (double)lowIndex;
    out += (high - low) * additionFactor;
    return out;
}

double sineR(double angle) {
    const double TWO_PI = (2 * PI);
    //enforce positive angle
    while (angle < 0) {
        angle += TWO_PI;
    }
    //enforce first rotation value
    while (angle >= (TWO_PI)) {
        angle -= TWO_PI;
    }
    
    //get the number "between" table values
    int lowIndex = (int) ((angle / TWO_PI) * (double) TABLE_SIZE);
    int highIndex = lowIndex + 1;
    if(lowIndex >= TABLE_SIZE){
        lowIndex -= TABLE_SIZE;
    }
    if(highIndex >= TABLE_SIZE){
        highIndex -= TABLE_SIZE;
    }
    double high = sineTable[highIndex];
    double low = sineTable[lowIndex];
    double out = low;
    double additionFactor = (angle / TWO_PI) * (double) TABLE_SIZE;
    additionFactor -= (double)lowIndex;
    out += (high - low) * additionFactor;
    return out;
}

double cosineD(double angle){
    angle += 90.0;
    return sineD(angle);
}

double cosineR(double angle){
    angle += (PI / 2);
    return sineR(angle);
}

double tanD(double angle){
    return (sineD(angle) / cosineD(angle));
}

double tanR(double angle){
    return (sineR(angle) / cosineR(angle));
}
