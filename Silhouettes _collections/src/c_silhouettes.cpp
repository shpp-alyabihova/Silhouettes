#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "gbufferedimage.h"
#include "gwindow.h"
#include "random.h"
#include <hwvector.h>
#include <hwpqueue.h>


using namespace std;



/*
 * The struct for storing parameters of the separate object
 */
struct Silhouette{
    Silhouette(){
        width = 0;
        height = 0;
    }

    int width;
    int height;
    int headLength;
    int headWidth;
    int headX; // X coordinate of top point of the separate object
    int headY; // Y coordinate of the mid-point of the separate object
};

/*
 * Asks for the way to enter a filename (from the file with test images, or own entry).
 * If filename is correct then returns the pointer to the open image.
 * Requests again for filename if the image file does not exist or is unable to be read as a valid image.
 * @return GBufferedImage* - the pointer to the open image.
 */
GBufferedImage* openFile();


/*
 * Creates a Vector of filenames with image from text file.
 */
HWVector<string> createTestFilenames();


/*
 * Creates 2-d bool array of passed image.
 * For each pixel that has the average value of color is less than 128,
 * takes a true value in the array, which corresponds to the black point in the image.
 * @param GBufferedImage* takes the pointer to the open image.
 * @params int takes a values that characterize parameters of 2-d array (rows and columns).
 * @return a pointer to a 2-d array, that represents the binary image.
 */
bool ** binarizationImage(GBufferedImage* img, int rows, int columns);


/*
 * Create the 2-d bool array on the heap with as a array of pointers to the bool arrays,
 * where memory for all values is allocated with the first pointer of the array of pointers.
 * Rest pointers are placed over a fixed period equal to the width of the image.
 * @params int takes a values that characterize parameters of 2-d array (rows and columns).
 * @return a pointer to a 2-d array, that was create on the heap.
 */
bool ** createArrayOnHeap2d(int rows, int columns);


/*
 * Destructor for 2-d array that was allocated on the heap.
 * @param bool ** takes a pointer to the array of pointers.
 */
void destructorFor2dArray(bool **ptrIMG);


/*
 * Calculates a number of meeting of continuous black dots for approximate counting of the objects in each row
 * @param bool ** takes a pointer to a 2-d array, that represents the binary image.
 * @params int takes a values that characterize parameters of 2-d array (rows and columns).
 * @param int * takes a pointer to the array where the resulting values will be stored.
 */
void calculateNumOfObjInEachRows(bool **ptrIMG, int rows, int columns, int * ptrH);


/*
 * Calculates a number of black pixels in each columns for approximate counting of height of the object.
 * @param bool ** takes a pointer to a 2-d array, that represents the binary image.
 * @params int takes a values that characterize parameters of 2-d array (rows and columns).
 * @param int * takes a pointer to the array where the resulting values will be stored.
 */
void calculateNumOfPxInEachColumns(bool **ptrIMG, int rows, int columns, int * ptrV);

/*
 * Calculates rows of objects in the image for further applaying the correct algorithm
 * for estimating the number of people in the image.
 * @param int * takes a pointer to the array with values of number of black dots in each row.
 * @param int takes a size of the array, that is equal number of rows.
 * @return int - a number of rows of the objects.
 */
int calculateNumOfRowsOfObj(int * ptrH, int rows);


/*
 * Checks every pixel of the image and if it is black checks its neighboring pixels until the object is fully allocated.
 * @param bool ** takes a pointer to a 2-d array, that represents the binary image.
 * @param int takes a values that characterize parameters of 2-d array (rows and columns).
 * @param int takes a reference for storing average value of height of all objects.
 * @return HWVector container with all separated objects
 */
HWVector<Silhouette> findingAllSeparatedObjects(bool **ptrIMG, int rows, int columns, int & averageHeight);


/*
 * Checks every pixel of the image and if it is black checks its neighboring pixels until the object is fully allocated.
 * Analyzes all objects and calculates approximate number of persons.
 * @param HWVector takes a container with all separated objects.
 * @param int takes an average value of height of all objects.
 * @param PriorityQueue<Silhouette> takes a reference to the PriorityQueue where all separate objects will be stored
 * according to their placement in the image from left to right.
 * @return int - the estimated number of persons based on the objects parameters.
 */
int calculateEstimateNumOfObj(HWVector<Silhouette> & separatedObjects, int averageHeight, HWPriorityQueue<Silhouette> & objectsFeatures);



/*
 * Cheks given cell of the matrix and if it has true value (that corresponds to the black point in the image)
 * changes the value to false and cheks its neighboring cells.
 * Thus allocated a separate object in the image.
 * @params int takes a values that characterize parameters of 2-d array (rows and columns).
 * @params int i and j takes an address of the currect cell.
 * @param bool ** takes a pointer to a 2-d array, that represents the binary image.
 * @param Vector<int> takes a reference to the Vector where the number of black pixels of each columns for each separate object will be stored.
 * @param Vector<int> takes a reference to the Vector where the number of black pixels of each rows for each separate object will be stored.
 */
void recursionFillObject(int rows, int columns, int i, int j, bool **ptrIMG, HWVector<int> &, HWVector<int> &);


/*
 * Analyzes the number of black pixels in each column and in each row for each separate object and fills the struct's fields.
 * Resulting struct is added to the PriorityQueue.
 * @param Vector<int> takes a reference to the Vector where the number of black pixels of each columns for each separate object is stored.
 * @param Vector<int> takes a reference to the Vector where the number of black pixels of each rows for each separate object is stored.
 * @return Silhouette -  the struct with data of the separate object.
 */
Silhouette createObject(HWVector<int> & charactXOfObj, HWVector<int> & charactYOfObj);


/*
 * Determines the starting coordinate and the max number of pixels.
 * @param Vector<int> takes a reference to the Vector where the number of black pixels is stored.
 * @param int takes a variable for passing data that characterize the maximum length of the requested parameter.
 * @return int starting requested coordinate.
 */
int determiningParametersOfObject(HWVector<int> & charactOfObj, int & length);


/*
 * Calculates the number of persons in the image with a some error and outputs result  to the console.
 * @param PriorityQueue<Silhouette> takes all separate objects in PriorityQueue according to their placement in the image from left to right.
 * @param int takes a number of rows of objects.
 * @param int * takes a pointer to the array with values of number of blocks black dots in each row.
 * @param int * takes a pointer to the array with values of number of black dots in each column.
 * @param int takes a number of columns.
 * @param int takes the estimated number of persons based on the objects parameters.
 *
 */
void outputResultNumOfPerson(HWPriorityQueue<Silhouette> objectsFeatures, int rowsOfObj, int *ptrH, int * ptrV, int columns, int estimatedNumObj);


/*
 * Calculates a number of ups and downs of the number of pixels on the columns that can be interpreted as the number of heads.
 * @param int * takes a pointer to the array with values of number of black dots in each column.
 * @param int takes a size of the array, that is equal number of columns.
 * @param PriorityQueue<Silhouette> takes all separate objects in PriorityQueue according to their placement in the image from left to right.
 * @return int - estimated number of heads of the object.
 */
int countingHeads(int * ptrV, int columns, HWPriorityQueue<Silhouette> objectsFeatures);


/*
 * Checks whether the current column is the last one.
 * @param int takes a number of current column.
 * @param int takes a number of columns.
 * @return bool - true if number of passing column is equal a last one.
 */
bool isEnd(int i, int columns);

/*
 * Calculates a number of meeting of blocks black dots for approximate counting of the objects in the interval,
 * where expected localization of the heads and in iterval where expected localization of the legs.
 * Thus, determined the approximate number of heads and legs of the object.
 * @param PriorityQueue<Silhouette> takes all separate objects in PriorityQueue according to their placement in the image from left to right.
 * @param int * takes a pointer to the array with values of number of blocks black dots in each row.
 * @params int takes a variable for passing data that characterize the approximate number of legs.
 * @return int - the approximate number of heads.
 */
int countingEstimatedNumOfHeads(HWPriorityQueue<Silhouette> objectsFeatures, int * ptrH, int & estimatedNumOfLegs);


/*
 * Calculates the average value from the passing range.
 * @param int * takes a pointer to the array with values of number of blocks black dots in each row.
 * @params int takes a value of boundary of the range.
 * @return int - the resulting average value.
 */
int calculateAverageValue(int *ptrH, int from, int to);


/*
 * Analyzes the number of person in the image considering the data obtained.
 * @param int takes a number of separate objects.
 * @param int takes the estimated number of persons based on the objects parameters.
 * @param int takes the approximate number of heads.
 * @param int takes the approximate number of legs.
 * @return int the number of person in the image with some deviation.
 */
int analyzingNumOfObj(int numOfObj, int estimatedNumOfObj, int numOfHeads, int estimatedNumOfHeads, int estimatedNumOfLegs);


/*
 * Counts the number of persons in the image with some deviation.
 * Deviation is less when the objects are arranged in a single row.
 * The deviation increases when there are foreign objects.
 */
int main() {
    GBufferedImage* img = openFile();
    int rows = img->getHeight();
    int columns = img->getWidth();
    GWindow gw(columns, rows);
    gw.add(img);
    bool **ptrIMG = binarizationImage(img, rows, columns);

    int numOfObjH [rows] = {0}; // An array for storing the number of objects in each row
    calculateNumOfObjInEachRows(ptrIMG, rows, columns, numOfObjH);

    int numOfpxObjV [columns] = {0}; //An array for storing the number of pixels of each objects in each columns
    calculateNumOfPxInEachColumns(ptrIMG, rows, columns, numOfpxObjV);

    int rowsOfObj = calculateNumOfRowsOfObj(numOfObjH, rows); // Calculate number of rows of objects

    HWPriorityQueue<Silhouette> objectsFeatures; // A container for storing all of the separate objects
    int averageHeight = 0;
    HWVector<Silhouette> separatedObjects = findingAllSeparatedObjects(ptrIMG, rows, columns, averageHeight);
    int estimatedNumObj = calculateEstimateNumOfObj(separatedObjects, averageHeight, objectsFeatures);
    destructorFor2dArray(ptrIMG);

    if(objectsFeatures.isEmpty()){
        cout << "Not found suitable objects, which could be interpreted as the silhouettes of person" << endl;
    }
    else {
        outputResultNumOfPerson(objectsFeatures, rowsOfObj, numOfObjH, numOfpxObjV, columns, estimatedNumObj);
    }

    return 0;
}



//============================================================================================================================


GBufferedImage* openFile(){
    string filename;
    GBufferedImage* img = new GBufferedImage();
    bool choice = 1;
    cout << "Press 0 to run test or 1 to open file: ";
    cin >> choice;
    while(true){
        if(choice){
            cout << "Please, input correct filename: ";
            cin >> filename;
        }
        else{
            HWVector<string> filenames = createTestFilenames();
            int i;
            cout << "Enter a number from 1 to " << filenames.size() << " to open file from the list for testing: ";
            cin >> i;
            if ((i < 1) || (i > filenames.size())){
                i = randomInteger(1, filenames.size());
            }
            filename = filenames[i - 1];

        }
        try{
            img->load(filename);
            return img;
        }
        catch(...){
            cout << "The image file does not exist or is unable to be read as a valid image" << endl;
        }
    }

    return img;
}


HWVector<string> createTestFilenames(){
    HWVector<string> filenames;
    ifstream readFile;
    readFile.open("filenames.txt");
    string filename;
    while (getline(readFile, filename)){
        filenames.add(filename);
    }
    readFile.close();
    return filenames;
}



bool ** binarizationImage(GBufferedImage* img, int rows, int columns){

    bool **ptrIMG = createArrayOnHeap2d(rows, columns);
    for(int y = 0; y < rows; ++y){
        for(int x = 0; x < columns; ++x){
            int r = img->getRed(img->getRGB(x, y));
            int g = img->getGreen(img->getRGB(x, y));
            int b = img->getBlue(img->getRGB(x, y));
            if ((r + g + b)/3 < 128) { // sort out with black pixel if the average colour saturation less than half of the max saturation (255)
                ptrIMG[y][x] = 1;
            }
            else ptrIMG[y][x] = 0;
        }
    }
    return ptrIMG;
}


bool ** createArrayOnHeap2d(int rows, int columns){
    bool ** ptrIMG = new bool *[rows];
    ptrIMG[0] = new bool [rows * columns];
    for(int i = 1; i < rows; ++i){
        ptrIMG[i] = ptrIMG[i - 1] + columns;
    }
    return ptrIMG;
}

void destructorFor2dArray(bool **ptrIMG){
    delete [] ptrIMG [0];
    delete [] ptrIMG;
}

//==========================================================================================================================

void calculateNumOfObjInEachRows(bool **ptrIMG, int rows, int columns, int * ptrH){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j){
            if (ptrIMG[i][j] == 1){ // when find first black pixel
                ptrH[i]++;             // increase the counter
                while (ptrIMG[i][j] == 1) { // until meet white pixel
                    ++j;                // skip columns
                }
            }
        }
    }
}

void calculateNumOfPxInEachColumns(bool **ptrIMG, int rows, int columns, int * ptrV){
    for (int j = 0; j < columns; ++j) {
        for (int i = 0; i < rows; ++i){
            if (ptrIMG[i][j] == 1){
                ptrV[j]++;
            }
        }
    }
}

int calculateNumOfRowsOfObj(int * ptrH, int rows){
    int rowsOfObj = 1;
    int heightOfRow = 0; // calculate height of row to eliminate the influence of small objects
    for (int i = 1; i < rows; ++i){
        if ((ptrH[i] == 0) && (ptrH[i-1] > 0) && (heightOfRow <= (rows - i))){
            heightOfRow = 0;
            ++rowsOfObj;
        }
        ++heightOfRow;
    }
    return rowsOfObj;
}

//============================================================================================================================

HWVector<Silhouette> findingAllSeparatedObjects(bool **ptrIMG, int rows, int columns, int & averageHeight){
    int heightOfObjects = 0;
    HWVector<Silhouette> separatedObjects;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j){
            if (ptrIMG[i][j] == 1){
                HWVector<int> charactXOfObj(columns, 0);
                HWVector<int> charactYOfObj(rows, 0);
                recursionFillObject(rows, columns, i, j, ptrIMG, charactXOfObj, charactYOfObj);
                Silhouette dataObj = createObject(charactXOfObj, charactYOfObj);
                heightOfObjects += dataObj.height;
                separatedObjects.add(dataObj);
            }
        }
    }
    if (!separatedObjects.isEmpty())
        averageHeight = heightOfObjects/separatedObjects.size();
    return separatedObjects;
}



void recursionFillObject(int rows, int columns, int i, int j, bool **ptrIMG, HWVector<int> & charactXOfObj, HWVector<int> & charactYOfObj){
    if(ptrIMG[i][j] == 0) return;
    else {
        ptrIMG[i][j] = 0;
        charactXOfObj[j]++; // count the number of pixels in each column of object
        charactYOfObj[i]++; // count the number of pixels in each row of object
        if(j + 1 < columns){
            recursionFillObject(rows, columns, i, j+1, ptrIMG, charactXOfObj, charactYOfObj);
        }
        if(i - 1 > 0){
            recursionFillObject(rows, columns, i-1, j, ptrIMG, charactXOfObj, charactYOfObj);
        }
        if(i + 1 < rows){
            recursionFillObject(rows, columns, i+1, j, ptrIMG, charactXOfObj, charactYOfObj);
        }
        if(j - 1 > 0){
            recursionFillObject(rows, columns, i, j-1, ptrIMG, charactXOfObj, charactYOfObj);
        }
    }

}



Silhouette createObject(HWVector<int> & charactXOfObj, HWVector<int> & charactYOfObj){
    Silhouette dataObj;
    int heightOfObj = 0;
    int widthOfObj = 0;

    int startX = determiningParametersOfObject(charactXOfObj, heightOfObj); // determination of the height of the object
    // determination of the Y coordinates of the topmost point of the object
    dataObj.headY = determiningParametersOfObject(charactYOfObj, widthOfObj); // determination of the width of the object

    // determination of the X coordinates of central point of the object
    dataObj.headX = (widthOfObj/2 + startX);
    dataObj.width = widthOfObj;
    dataObj.height = heightOfObj;
    dataObj.headLength = heightOfObj/8; // according to body proportions head length is 1/8 of body length
    dataObj.headWidth = dataObj.headLength/7 * 4; // according to head proportions head width is 4/7 of head length

    return dataObj;
}


int determiningParametersOfObject(HWVector<int> & charactOfObj, int & length){
    bool flag = 1;
    int startCoord = 0;
    for(int i = 0; i < charactOfObj.size(); ++i){
        if(flag && charactOfObj[i] > 0){
            startCoord = i;
            flag = 0;
        }
        if(charactOfObj[i] > length){
            length = charactOfObj[i]; // find max length of object
        }
    }
    return startCoord;
}



int calculateEstimateNumOfObj(HWVector<Silhouette> & separatedObjects, int averageHeight, HWPriorityQueue<Silhouette> & objectsFeatures){
    int estimatedNumObj = 0;
    for (int i = 0; i < separatedObjects.size(); ++i) {
        Silhouette dataObj = separatedObjects[i];
        if ((dataObj.height > averageHeight/2) && (dataObj.headLength > 1)){
            objectsFeatures.enqueue(dataObj, dataObj.headX);
            // calculates num of persons of separate object according a body proportions (the width of the body is two length of head)
            int numObj = (dataObj.width / (2*dataObj.headLength));
            estimatedNumObj += numObj;
        }
    }
    return estimatedNumObj;
}

//===========================================================================================================================

void outputResultNumOfPerson(HWPriorityQueue<Silhouette> objectsFeatures, int rowsOfObj, int * ptrH, int * ptrV, int columns, int estimatedNumObj){
    int numOfPerson = 0;
    int probability = 0;
    // If silhouettes locate are located in several rows then assume that the number of persons are equal to the resulting estimated number of person
    if (rowsOfObj > 1){
        numOfPerson = estimatedNumObj;
        probability = 85;

    }
    else {
        int numOfHeads = countingHeads(ptrV, columns, objectsFeatures);
        int estimatedNumOfLegs = 0;
        int estimatedNumOfHeads = countingEstimatedNumOfHeads(objectsFeatures, ptrH, estimatedNumOfLegs);
        numOfPerson = analyzingNumOfObj(objectsFeatures.size(), estimatedNumObj, numOfHeads, estimatedNumOfHeads, estimatedNumOfLegs);
        numOfPerson = (numOfPerson == 0) ? 1 : numOfPerson;
        probability = 90;
    }
    cout << "number of persons = " << numOfPerson << " with probability " << probability << "%" << endl;
}

int countingHeads(int * ptrV, int columns, HWPriorityQueue<Silhouette> objectsFeatures){
    int numOfHeads = 0;
    int max_px = 0; //peak of pixels in each column
    int counter = 0; //counter to fix the ups and downs of the maximum number of pixels in each column
    bool flag = 0; // flag to ignore excess lowering of the maximum number of pixels in each column
    bool flagObj = 0; // flag to pop the next object
    Silhouette dataObject = objectsFeatures.dequeue();


    int limit = dataObject.headWidth / 2; // interval for monitoring the necessary lowering
    bool growing = 0; // flag for tracking pixels rising

    int i = 0;
    while(i < columns){
        while(ptrV[i] > max_px){
            ++counter;
            max_px = ptrV[i]; // fix max value
            ++i;
            if (counter > limit)
                growing = 1; // ascertain growth stage
            if(growing)
                flag = 1;   //set the flag to catch the lowering
            if (isEnd(i, columns)) // checking end of image
                return numOfHeads;
        }
        counter = 0;
        while (flag && ptrV[i] <= max_px) {
            ++counter;
            if(counter > limit){
                int drop =  max_px - ptrV[i]; // calculate the drop in current possition
                if(drop > dataObject.headLength){
                    if ((counter < dataObject.headWidth) || (i == columns - 1)){ //check the resulting drop for getting into allowable range
                            ++ numOfHeads;
                            flag = 0; // set the flag to ignore excess lowering
                            flagObj = 1; // set the flag for possibility to pop new object
                            growing = 0; // fix the reducing step
                    }
                    max_px = ptrV[i];
                    break;
                }
            }
            ++i;
            if (isEnd(i, columns)) // checking end of image
                return numOfHeads;
        }
        if(!flagObj && (ptrV[i] == 0) && (!objectsFeatures.isEmpty()))
            dataObject = objectsFeatures.dequeue();
        counter = 0;
        if (ptrV[i] <= max_px){ // processing excess lowering
            max_px = ptrV[i];
            ++i;
        }
    }
    return numOfHeads;
}


bool isEnd(int i, int columns){
    return i == columns;
}


int countingEstimatedNumOfHeads(HWPriorityQueue<Silhouette> objectsFeatures, int * ptrH, int & estimatedNumOfLegs){

    Silhouette currentObj = objectsFeatures.dequeue();
    int from = currentObj.headY + currentObj.height - currentObj.headLength;
    int to = currentObj.headY + currentObj.height;
    estimatedNumOfLegs = calculateAverageValue(ptrH, from, to);
    from = currentObj.headY + currentObj.headLength/3;
    to = currentObj.headY + currentObj.headLength;

    return calculateAverageValue(ptrH, from, to);
}

int calculateAverageValue(int *ptrH, int from, int to){
    int sum = 0;
    int num = 0;
    for (int i = from; i < to; ++i){
        sum += ptrH[i];
        ++num;
    }
    return (sum/num);
}
//=============================================================================================================================


int analyzingNumOfObj(int numOfObj, int estimatedNumOfObj, int numOfHeads, int estimatedNumOfHeads, int estimatedNumOfLegs){

    estimatedNumOfObj = (estimatedNumOfObj == 0 ? 1 : estimatedNumOfObj); //for a person is standing sideways, expected width is less than 1
    numOfHeads = (numOfHeads < numOfObj) ? numOfObj : numOfHeads;
    estimatedNumOfHeads = (estimatedNumOfHeads < numOfObj) ? numOfObj : estimatedNumOfHeads;
    int numOfPerson = 0;
    int minNumOfSilhuettes = (estimatedNumOfLegs + 1)/2;
    if ((minNumOfSilhuettes > numOfHeads) && (minNumOfSilhuettes <= estimatedNumOfObj)){
        numOfHeads = minNumOfSilhuettes;
    }
    if ((minNumOfSilhuettes > estimatedNumOfHeads) && (minNumOfSilhuettes <= estimatedNumOfObj)){
        estimatedNumOfHeads = minNumOfSilhuettes;
    }
    numOfPerson = (estimatedNumOfObj + numOfHeads + estimatedNumOfHeads)/3;
    numOfPerson = (numOfPerson < minNumOfSilhuettes) ? minNumOfSilhuettes : numOfPerson;
    return numOfPerson;
}
