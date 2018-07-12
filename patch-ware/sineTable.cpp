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

double asineD(double value){
    while(value > 1){
        value -= 1;
    }
    while(value < 0){
        value += 1;
    }
    double index = binSearch(value, sineTable, TABLE_SIZE * 0.25 - 1, TABLE_SIZE * 0.75 - 1);
    index = TABLE_SIZE * 0.5 - index;
    return (index / ((double)TABLE_SIZE)) * 360.;
}

double asineR(double value){
    while(value > 1){
        value -= 1;
    }
    while(value < 0){
        value += 1;
    }
    double index = binSearch(value, sineTable, TABLE_SIZE * 0.25 - 1, TABLE_SIZE * 0.75 - 1);
    index = TABLE_SIZE * 0.5 - index;
    return (index / (double)TABLE_SIZE) * 2 * PI;
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

double binSearch(double value, const double array[], int lowIndex = 5999, int highIndex = 17999){
    int midIndex = (lowIndex + highIndex) / 2;
    if(value == array[midIndex]){
        return midIndex;
    }
    if(lowIndex == midIndex || lowIndex >= highIndex){
        int high = 0;
        int low = 0;
        if(lowIndex == 5999 && value < array[lowIndex]){ // 1/4 index for the sine table
            low = lowIndex;
            high = low + 1;
        }
        else if(highIndex == 17999 && value > array[highIndex]){ // the 3/4 index for the sine table
            high = highIndex;
            low = high - 1;
        }
        else if(value < array[lowIndex]){
            low = lowIndex;
            high = low + 1;
        }
        else {
            high = highIndex;
            low = high - 1;
        }
        double lowVal = array[high];
        double highVal = array[low];
        double iratio = (value - lowVal) / (highVal - lowVal);
        
        return ((double)low + iratio);
        
    }
    if(array[midIndex] < value){
        return binSearch(value, array, lowIndex, midIndex - 1);
    }
    else{
        return binSearch(value, array, midIndex + 1, highIndex);
    }
}