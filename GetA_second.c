#pragma config(Sensor, S1,     c1,             sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S2,     c2,             sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3,     c3,             sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor,  motorB,          lm,            tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          rm,            tmotorEV3_Large, PIDControl, encoder)

#define Black 1
#define Blue 2
#define Green 3
#define Yellow 4
#define Red 5
#define White 6
#define Brown 7
int nMotorSpeedSetting = 20, vertex = 0, count = 0, row = 0, val, positionRow = 0, positionColumn = 0, score = 0;
int columnNumber = 4, rowNumber = 3, back_count=0 ,directionUp=0, directionDown=0 ,directionLeft=1, directionRight=0;
int dt[7][7], M[7][7], visited[7][7];
int A[4] = {0, 0, 0, 0};
int B[4] = {0, 0, 0, 0};
int highValue = -864;
int getRedValue = -1325;


void setSpeedAndSleep(int leftSpeed, int rightSpeed, int time)
{
    setMotorSpeed(lm, leftSpeed);
    setMotorSpeed(rm, rightSpeed);
    sleep(time);
}

void stopMotor()
{
    setMotorSpeed(lm, 0);
    setMotorSpeed(rm, 0);
}

void getScore(){
    int i=0, certainColor=0, tmpColor=0;
    setSpeedAndSleep(-10, -10, 500);
    stopMotor();
    while(i<200){
        tmpColor = getColorName(c2);
        if(tmpColor == Green || tmpColor == Red){
            certainColor = tmpColor;
        }
        i++;
        setSpeedAndSleep(15, -15, 2);
    }
    setSpeedAndSleep(-15, 15, 400);
    stopMotor();
    if(certainColor == Red){
        if (visited[positionRow][positionColumn] == 1)
        {
            eraseDisplay();
            displayBigTextLine(1, "You have");
            displayBigTextLine(3, "already visited:");
            displayBigTextLine(5, "score -2");
            sleep(500);
            playTone(240, 44);
            sleep(300);
            score += M[positionRow][positionColumn];
        }
        else
        {
            eraseDisplay();
            displayBigTextLine(1, "RED:");
            displayBigTextLine(3, "score +5");
            sleep(500);
            playTone(240, 20);
            sleep(300);
            score += M[positionRow][positionColumn];
        }
    }
    else if(certainColor == Green){
        eraseDisplay();
        displayBigTextLine(1, "GREEN:");
        displayBigTextLine(3, "score -5");
        sleep(500);
        playTone(240, 20);
        sleep(300);
        score += M[positionRow][positionColumn];
    }
    else{
        eraseDisplay();
        displayBigTextLine(1, "NULL: 0");
        sleep(500);
        score += M[positionRow][positionColumn];
    }
    setSpeedAndSleep(15, 15, 400);
}

void patchSector(){
    int i=0, certainColor=0, tmpColor=0;
    setSpeedAndSleep(15, -15, 150);
    stopMotor();
    while(i<200){
        tmpColor = getColorName(c2);
        if(tmpColor == Green || tmpColor == Red) certainColor = tmpColor;
        i++;
        setSpeedAndSleep(15, -15, 1);
    }
    setSpeedAndSleep(-15, 15, 300);
    stopMotor();
    if(row % 2 == 0) { //even
       if(certainColor == Green){
           displayBigTextLine(1,"Map[%d][%d] = Green",row ,++count);
           M[row][count] = -6;
           sleep(500);
       }
       else if(certainColor == Red){
           displayBigTextLine(1,"Map[%d][%d] = RED",row ,++count);
           M[row][count] = 4;
           sleep(500);
       }
       else {
           displayBigTextLine(1,"Map[%d][%d] = NOT",row ,++count);
           M[row][count] = -1;
           sleep(500);
       }
    }
    else if(row % 2 != 0) { //odd
        if(certainColor == Green){
           displayBigTextLine(1,"Map[%d][%d] = Green",row ,columnNumber-(++count));
           M[row][columnNumber-count] = -6;
           sleep(500);
        }
        else if(certainColor == Red){
           displayBigTextLine(1,"Map[%d][%d] = RED",row ,columnNumber-(++count));
           M[row][columnNumber-count] = 4;
           sleep(500);
        }
        else {
           displayBigTextLine(1,"Map[%d][%d] = NOT",row ,columnNumber-(++count));
           M[row][columnNumber-count] = -1;
           sleep(500);
        }
    }
}

void getColorIncreaseVertex(short colorSensor)
{
    if (getColorName(colorSensor) == Yellow && count != columnNumber) vertex++;
    else vertex = 0;
}

void go(){
    val = 5;
    //Line tracing
    if (getColorName(c2) == Yellow) setSpeedAndSleep(nMotorSpeedSetting - val, nMotorSpeedSetting + val, 1);
    else setSpeedAndSleep(nMotorSpeedSetting + val, nMotorSpeedSetting - val, 1);
    if (row == 0) getColorIncreaseVertex(c3);
    else if (row % 2 == 0) getColorIncreaseVertex(c1);
    else getColorIncreaseVertex(c3);
    if (vertex == 1) patchSector();
}

void turnLeft()
{
    setSpeedAndSleep(24, 25, 400);
    while (getColorName(c1) > Yellow) setSpeedAndSleep(-nMotorSpeedSetting * 6 / 10, nMotorSpeedSetting * 6 / 10, 10);
    setSpeedAndSleep(-nMotorSpeedSetting * 6 / 10, nMotorSpeedSetting * 6 / 10, 400);
    while (getColorName(c2) > Yellow) setSpeedAndSleep(-nMotorSpeedSetting * 4 / 10, nMotorSpeedSetting * 4 / 10, 10);
    sleep(80);
    setSpeedAndSleep(0, 0, 200);
}

void turnRight()
{
    setSpeedAndSleep(25, 25, 350);
    while (getColorName(c3) > Yellow) setSpeedAndSleep(nMotorSpeedSetting * 6 / 10, -nMotorSpeedSetting * 6 / 10, 10);
    while (getColorName(c2) > Yellow) setSpeedAndSleep(nMotorSpeedSetting * 6 / 10, -nMotorSpeedSetting * 6 / 10, 1);
    sleep(80);
    setMotorSpeed(lm, 0);
    sleep(200);
    setMotorSpeed(rm, 0);
}

void turn180()
{
        setSpeedAndSleep(15, -15, 2250);
        setSpeedAndSleep(-15, -15, 1250);
}

void getColorIncreaseCount(short colorSensor)
{
    if (getColorName(colorSensor) == Yellow) count++;
    else count=0;
}

void resetRobotDirection()
{
    directionLeft = 0;
    directionUp = 0;
    directionDown = 0;
    directionRight = 0;
}

void visitedAndscoreSub(){
    visited[positionRow][positionColumn] = 1;
    if (M[positionRow][positionColumn] == 4 )
    {
        M[positionRow][positionColumn] = -3;
    }
    else if (M[positionRow][positionColumn] == -3)
    {
        M[positionRow][positionColumn] = -1000;
    }
}

void goUp()
{
    row++;
    if (directionLeft == 1)
    {
        resetRobotDirection();
        turnRight();
        directionUp = 1;
    }
    else if (directionRight == 1)
    {
        resetRobotDirection();
        turnLeft();
        directionUp = 1;
    }
    else if (directionDown == 1)
    {
        resetRobotDirection();
        turn180();
        directionUp = 1;
    }
    count = 0; positionRow--;
    while (true)
    {
        if (getColorName(c2) == Yellow) setSpeedAndSleep(nMotorSpeedSetting - val, nMotorSpeedSetting + val, 1);
        else setSpeedAndSleep(nMotorSpeedSetting + val, nMotorSpeedSetting - val, 1);
        if (positionColumn == 0) getColorIncreaseCount(c3);
        else if (positionRow == 0) getColorIncreaseCount(c1);
        else if (positionColumn == columnNumber) getColorIncreaseCount(c1);
        else if (positionRow == rowNumber) getColorIncreaseCount(c3);
        else getColorIncreaseCount(c3);
        if (count == 1)
        {
            setSpeedAndSleep(15, 15, 500);
            getScore();
            break;
        }
   }
    visitedAndscoreSub();
}

void goDown()
{
    if (directionUp == 1)
    {
        resetRobotDirection();
        turn180();
        directionDown = 1;
    }
    else if (directionRight == 1)
    {
        resetRobotDirection();
        turnRight();
        directionDown = 1;
    }
    else if (directionLeft == 1)
    {
        resetRobotDirection();
        turnLeft();
        directionDown = 1;
    }
    count = 0;
    positionRow++;
    while (true)
    {
        if (getColorName(c2) == Yellow) setSpeedAndSleep(nMotorSpeedSetting - val, nMotorSpeedSetting + val, 1);
        else setSpeedAndSleep(nMotorSpeedSetting + val, nMotorSpeedSetting - val, 1);
        if (positionColumn == 0) getColorIncreaseCount(c1);
        else if (positionRow == 0) getColorIncreaseCount(c3);
        else if (positionColumn == columnNumber) getColorIncreaseCount(c3);
        else if (positionRow == rowNumber) getColorIncreaseCount(c3);
        else getColorIncreaseCount(c3);
        if (count == 1)
        {
            setSpeedAndSleep(15, 15, 420);
            getScore();
            break;
        }
    }
    visitedAndscoreSub();
}

void goLeft()
{
    if (directionUp == 1)
    {
        resetRobotDirection();
        turnLeft();
        directionLeft = 1;
    }
    else if (directionRight == 1)
    {
        resetRobotDirection();
        turn180();
        directionLeft = 1;
    }
    else if (directionDown == 1)
    {
        resetRobotDirection();
        turnRight();
        directionLeft = 1;
    }
    count = 0;
    positionColumn--;
    while (true)
    {
        if (getColorName(c2) == Yellow) setSpeedAndSleep(nMotorSpeedSetting - val, nMotorSpeedSetting + val, 1);
        else setSpeedAndSleep(nMotorSpeedSetting + val, nMotorSpeedSetting - val, 1);

        if (positionRow == 0) getColorIncreaseCount(c1);
        else if (positionColumn == 0) getColorIncreaseCount(c3);
        else if (positionRow == rowNumber) getColorIncreaseCount(c3);
        else if (positionColumn == columnNumber) getColorIncreaseCount(c1);
        else getColorIncreaseCount(c3);
        if (count == 1)
        {
            setSpeedAndSleep(15, 15, 420);
            getScore();
            break;
        }
    }
    visitedAndscoreSub();
}

void goRight()
{
    if (directionUp == 1)
    {
        resetRobotDirection();
        turnRight();
        directionRight = 1;
    }
    else if (directionLeft == 1)
    {
        resetRobotDirection();
        turn180();
        directionRight = 1;
    }
    else if (directionDown == 1)
    {
        resetRobotDirection();
        turnLeft();
        directionRight = 1;
    }
    count = 0;
    positionColumn++;
    while (true)
    {
        if (getColorName(c2) == Yellow) setSpeedAndSleep(nMotorSpeedSetting - val, nMotorSpeedSetting + val, 1);
        else setSpeedAndSleep(nMotorSpeedSetting + val, nMotorSpeedSetting - val, 1);
        if (positionRow == rowNumber) getColorIncreaseCount(c1);
        else if (positionColumn == 0) getColorIncreaseCount(c3);
        else if (positionRow == 0) getColorIncreaseCount(c3);
        else if (positionColumn == columnNumber) getColorIncreaseCount(c1);
        else getColorIncreaseCount(c3);
        if (count == 1)
        {
            setSpeedAndSleep(15, 15, 420);
            getScore();
            break;
        }
    }
    visitedAndscoreSub();
}


void completeSearch(){
    sleep(1000);
    while (true){
        if (count != columnNumber) go();
        if (count == columnNumber)
        { // if robot is end of column
            if (row == rowNumber) return; // all grid scanning complete
            if (row % 2 == 0)
            { // move backward
                setSpeedAndSleep(35, 30, 200);
                turnRight();
            }
            else
            { // if robot is middle of line
                setSpeedAndSleep(30, 35, 200);
                turnLeft();
            }
            row++;
            if (row % 2 == 0)
            {
                while (getColorName(c1) == White) go();
                count = -1;
                patchSector();
                setSpeedAndSleep(30, 35, 200);
                turnLeft();
            }
            else
            {
                while (getColorName(c3) == White) go();// move next column(black col line)
                count = -1;
                patchSector();
                setSpeedAndSleep(35, 30, 200);
                turnRight(); // turn agian
            }
            count = 0; // reset scaning, position
            sleep(400);
        }
    }
}

void printMap()
{
    char map[4][5];
    for (int i = 0; i <= rowNumber; i++)
    {
        for (int j = 0; j <= columnNumber; j++)
        {
            if (M[i][j] == -1) map[i][j] = '+';
            else if (M[i][j] == 4) map[i][j] = 'O';
            else if (M[i][j] == -6) map[i][j] = 'X';
        }
    }
    for(int i = 0; i <=rowNumber; i++)
       for(int j = 0; j <= columnNumber; j++)
          displayStringAt( (j+1)*10, 100-(i+1)*10, "%c", map[i][j] );
    sleep(5000);
}

void backMovingPoint()
{
    while(true){
        if (getColorName(c2) == Yellow) setSpeedAndSleep(nMotorSpeedSetting - val, nMotorSpeedSetting + val, 1);
        else setSpeedAndSleep(nMotorSpeedSetting + val, nMotorSpeedSetting - val, 1);
        if (getColorName(c1) == Yellow){
            back_count++;
            setSpeedAndSleep(15, 15, 420);
            setSpeedAndSleep(0, 0, 420);
        }
        if(back_count == columnNumber) return;
    }
}
void getRedPatch()
{
   if (positionColumn == columnNumber && positionRow == rowNumber)
    {
        B[0] = -100;
        B[1] = -100;
        B[2] = M[positionRow-1][positionColumn];
        B[3] = M[positionRow][positionColumn-1];
    }
    else if (positionRow == 0 && positionColumn == columnNumber)
    {
        B[0] = -100;
        B[1] = -100;
        B[2] = M[positionRow+1][positionColumn];
        B[3] = M[positionRow][positionColumn-1];
    }
    else if (positionRow == rowNumber && positionColumn == 0)
    {
        B[0] = -100;
        B[1] = -100;
        B[2] = M[positionRow][positionColumn+1];
        B[3] = M[positionRow-1][positionColumn];
    }
    else if(positionRow == 0)
    {
       B[0] = -100;
        B[1] = M[positionRow+1][positionColumn];
        B[2] = M[positionRow][positionColumn-1];
        B[3] = M[positionRow][positionColumn+1];
    }
    else if (positionColumn == 0)
    {
       B[0] = M[positionRow+1][positionColumn];
        B[1] = M[positionRow-1][positionColumn];
        B[2] = -100;
        B[3] = M[positionRow][positionColumn+1];
     }
    else if (positionRow == rowNumber)
    {
       B[0] = -100;
        B[1] = M[positionRow-1][positionColumn];
        B[2] = M[positionRow][positionColumn-1];
        B[3] = M[positionRow][positionColumn+1];
    }
    else if (positionColumn == columnNumber)
    {
       B[0] = M[positionRow+1][positionColumn];
        B[1] = M[positionRow-1][positionColumn];
        B[2] = M[positionRow][positionColumn-1];
        B[3] = -100;
    }
    else
    {
       B[0] = M[positionRow+1][positionColumn];
        B[1] = M[positionRow-1][positionColumn];
        B[2] = M[positionRow][positionColumn-1];
        B[3] = M[positionRow][positionColumn+1];
    }
    int temp = 0;
    for(int k=0; k<3; k++)
    {
        for(int n=0; n<3-k; n++)
        {
            if(B[n]<B[n+1])
            {
                temp = B[n];
                B[n] = B[n+1];
                B[n+1] = temp;
            }
        }
    }
    if (B[0] == 4) {
        getRedValue = 4;
    }
}


void cal(){
    /*
        input your Algorithm. and dt table renewal
    */
    for(int i=0; i<=rowNumber; i++){
        for(int j=0; j<=columnNumber; j++){
            if(i==0 && j==0) dt[i][j] = M[i][j];
            else if(i == 0) dt[i][j] = dt[i][j-1] + M[i][j];
            else if(j==0) dt[i][j] = dt[i-1][j] + M[i][j];
            else {
                if(dt[i-1][j] > dt[i][j-1]) dt[i][j] = dt[i-1][j] + M[i][j];
                else dt[i][j] = dt[i][j-1] + M[i][j];
            }
        }
    }
    dt[4][0] = -125;
    dt[4][1] = -125;
    dt[4][2] = -125;
    dt[4][3] = -125;
    dt[4][4] = -125;
    dt[4][5] = -125;
    dt[0][5] = -125;
    dt[1][5] = -125;
    dt[2][5] = -125;
    dt[3][5] = -125;
}

void getHighValue()
{
    if (positionColumn == columnNumber && positionRow == rowNumber)
    {
        A[0] = -100;
        A[1] = -100;
        A[2] = dt[positionRow-1][positionColumn];
        A[3] = dt[positionRow][positionColumn-1];
    }
    else if (positionRow == 0 && positionColumn == columnNumber)
    {
        A[0] = -100;
        A[1] = -100;
        A[2] = dt[positionRow+1][positionColumn];
        A[3] = dt[positionRow][positionColumn-1];
    }
    else if (positionRow == rowNumber && positionColumn == 0)
    {
        A[0] = -100;
        A[1] = -100;
        A[2] = dt[positionRow][positionColumn+1];
        A[3] = dt[positionRow-1][positionColumn];
    }
    else if(positionRow == 0)
    {
       A[0] = -100;
        A[1] = dt[positionRow+1][positionColumn];
        A[2] = dt[positionRow][positionColumn-1];
        A[3] = dt[positionRow][positionColumn+1];
    }
    else if (positionColumn == 0)
    {
       A[0] = dt[positionRow+1][positionColumn];
        A[1] = dt[positionRow-1][positionColumn];
        A[2] = -100;
        A[3] = dt[positionRow][positionColumn+1];
     }
    else if (positionRow == rowNumber)
    {
       A[0] = -100;
        A[1] = dt[positionRow-1][positionColumn];
        A[2] = dt[positionRow][positionColumn-1];
        A[3] = dt[positionRow][positionColumn+1];
    }
    else if (positionColumn == columnNumber)
    {
       A[0] = dt[positionRow+1][positionColumn];
        A[1] = dt[positionRow-1][positionColumn];
        A[2] = dt[positionRow][positionColumn-1];
        A[3] = -100;
    }
    else
    {
       A[0] = dt[positionRow+1][positionColumn];
        A[1] = dt[positionRow-1][positionColumn];
        A[2] = dt[positionRow][positionColumn-1];
        A[3] = dt[positionRow][positionColumn+1];
    }
    int temp = 0;
    for(int k=0; k<3; k++)
    {
        for(int n=0; n<3-k; n++)
        {
            if(A[n]<A[n+1])
            {
                temp = A[n];
                A[n] = A[n+1];
                A[n+1] = temp;
            }
        }
    }
    highValue=A[0];
}
void anotherStrategy_go_left()
{
	if(positionColumn == 1)return;
    if(positionRow == rowNumber) {
        if (M[positionRow-2][positionColumn]==4 && M[positionRow-1][positionColumn] == -1) {
            goUp(); goUp(); goDown(); goDown();
        }
    }
    else if(positionRow == rowNumber-1) {
        if (M[positionRow-2][positionColumn]==4 && M[positionRow-1][positionColumn] == -1) {
            goUp(); goUp(); goDown(); goDown();
        }
    }
    else if(positionRow == rowNumber-2) {
        if (M[positionRow+2][positionColumn]==4 && M[positionRow+1][positionColumn] == -1) {
            goDown(); goDown(); goUp(); goUp();
        }
    }
    else if(positionRow == rowNumber-3) {
        if (M[positionRow+2][positionColumn]==4 && M[positionRow+1][positionColumn] == -1) {
            goDown(); goDown(); goUp(); goUp();
        }
    }
}

void anotherStrategy_go_right()
{
	if(positionColumn == 1)return;
     if(positionRow == rowNumber) {
        if (M[positionRow-2][positionColumn]==4 && M[positionRow-1][positionColumn] == -1) {
            goUp(); goUp(); goDown(); goDown();
        }
    }
    else if(positionRow == rowNumber-1) {
        if (M[positionRow-2][positionColumn]==4 && M[positionRow-1][positionColumn] == -1) {
            goUp(); goUp(); goDown(); goDown();
        }
    }
    else if(positionRow == rowNumber-2) {
        if (M[positionRow+2][positionColumn]==4 && M[positionRow+1][positionColumn] == -1) {
            goDown(); goDown(); goUp(); goUp();
        }
    }
    else if(positionRow == rowNumber-3) {
        if (M[positionRow+2][positionColumn]==4 && M[positionRow+1][positionColumn] == -1) {
            goDown(); goDown(); goUp(); goUp();
        }
    }
}

void anotherStrategy_go_up()
{
	if(positionColumn == 1)return;
    if(positionColumn == columnNumber) {
        if (M[positionRow][positionColumn-2]==4 && M[positionRow][positionColumn-1] == -1) {
            goLeft(); goLeft(); goRight(); goRight();
        }
    }
    else if(positionColumn == columnNumber-1) {
        if (M[positionRow][positionColumn-2]==4 && M[positionRow][positionColumn-1] == -1) {
            goLeft(); goLeft(); goRight(); goRight();
        }
    }
    else if(positionColumn == columnNumber-2) {
        if (M[positionRow][positionColumn-2]==4 && M[positionRow][positionColumn-1] == -1) {
            goLeft(); goLeft(); goRight(); goRight();
        }
        if (M[positionRow][positionColumn+2]==4 && M[positionRow][positionColumn+1] == -1) {
            goRight(); goRight(); goLeft(); goLeft();
        }
    }
    else if(positionColumn == columnNumber-3) {
        if (M[positionRow][positionColumn+2]==4 && M[positionRow][positionColumn+1] == -1) {
            goRight(); goRight();goLeft(); goLeft();
        }
    }



    else if(positionColumn == columnNumber-4) {
        if (M[positionRow][positionColumn+2]==4 && M[positionRow][positionColumn+1] == -1) {
            goRight(); goRight(); goLeft(); goLeft();
        }
  }
}

void anotherStrategy_go_down()
{
	if(positionColumn == 1)return;
    if(positionColumn == columnNumber) {
        if (M[positionRow][positionColumn-2]==4 && M[positionRow][positionColumn-1] == -1) {
            goLeft(); goLeft(); goRight(); goRight();
        }
    }
    else if(positionColumn == columnNumber-1) {
        if (M[positionRow][positionColumn-2]==4 && M[positionRow][positionColumn-1] == -1) {
            goLeft(); goLeft(); goRight(); goRight();
        }
    }
    else if(positionColumn == columnNumber-2) {
        if (M[positionRow][positionColumn-2]==4 && M[positionRow][positionColumn-1] == -1) {
            goLeft(); goLeft(); goRight(); goRight();
        }
        if (M[positionRow][positionColumn+2]==4 && M[positionRow][positionColumn+1] == -1) {
            goRight(); goRight(); goLeft(); goLeft();
        }
    }
    else if(positionColumn == columnNumber-3) {
        if (M[positionRow][positionColumn+2]==4 && M[positionRow][positionColumn+1] == -1) {
            goRight(); goRight();goLeft(); goLeft();
        }
    }
    else if(positionColumn == columnNumber-4) {
        if (M[positionRow][positionColumn+2]==4 && M[positionRow][positionColumn+1] == -1) {
            goRight(); goRight(); goLeft(); goLeft();
        }
    }
}


task main()
{
    while (getButtonPress(1) == 0) sleep(10);
    M[0][0] = -1;
    completeSearch();
    eraseDisplay();
    turn180();
    backMovingPoint();
    turn180();
    stopMotor();
    printMap();
    eraseDisplay();
    positionColumn = columnNumber; positionRow = rowNumber; row = 0;
    // go back
    while (positionRow != 0 || positionColumn != 0){
        cal();
        getHighValue();
        getRedPatch();
        if(positionRow != 0 && positionColumn != 0){
            if (getRedValue == M[positionRow][positionColumn-1])
            {
            		anotherStrategy_go_left();
                goLeft();
          	}
            else if(getRedValue == M[positionRow][positionColumn+1])
            {
            	anotherStrategy_go_right();
               goRight();
            }
            else if(getRedValue == M[positionRow-1][positionColumn])
            {
            	anotherStrategy_go_up();
             	  goUp();

            }
            else if (getRedValue == M[positionRow+1][positionColumn])
          	{
          		anotherStrategy_go_down();
                goDown();
            }
            else if(highValue == dt[positionRow][positionColumn-1])
            {
                anotherStrategy_go_left();
                goLeft();
            }
            else if(highValue == dt[positionRow][positionColumn+1])
            {
                anotherStrategy_go_right();
                goRight();
            }
            else if(highValue == dt[positionRow-1][positionColumn])
            {
                anotherStrategy_go_up();
                goUp();
            }
            else if(highValue == dt[positionRow+1][positionColumn])
            {
                anotherStrategy_go_down();
                goDown();
            }
        }
        else if (positionRow == 0)
        {
            if (getRedValue == M[positionRow][positionColumn-1])
            {
                anotherStrategy_go_left();
                goLeft();
            }
            else if(getRedValue == M[positionRow][positionColumn+1])
            {
                anotherStrategy_go_right();
                goRight();
            }
            else if (getRedValue == M[positionRow+1][positionColumn])
            {
                anotherStrategy_go_down();
                goDown();
            }
            else if(highValue == dt[positionRow][positionColumn-1])
            {
                anotherStrategy_go_left();
                goLeft();
            }
            else if(highValue ==dt[positionRow][positionColumn+1])
            {
                anotherStrategy_go_right();
                goRight();
            }
            else if(highValue == dt[positionRow+1][positionColumn])
            {
                anotherStrategy_go_down();
                goDown();
            }
        }
        else if (positionColumn == 0)
        {
            if(getRedValue == M[positionRow][positionColumn+1])
            {
                anotherStrategy_go_right();
                goRight();
            }
            else if(getRedValue == M[positionRow-1][positionColumn])
            {
                anotherStrategy_go_up(); // ok
                goUp();
            }
            else if (getRedValue == M[positionRow+1][positionColumn])
            {
                anotherStrategy_go_down();
                goDown();
            }


            else if(highValue == dt[positionRow][positionColumn+1])
            {
                anotherStrategy_go_right();
                goRight();
            }
            else if(highValue == dt[positionRow-1][positionColumn])
            {
                anotherStrategy_go_up(); // ok
                goUp(); //ok

            }
            else if(highValue == dt[positionRow+1][positionColumn])
            {
                anotherStrategy_go_down();
                goDown();
            }
        }
        eraseDisplay();
    }
    playTone(240, 20);
    sleep(300);
    stopMotor();
    while (getButtonPress(1) == 0) sleep(10);
    eraseDisplay();
    displayBigTextLine(1, "Final Score =");
    displayBigTextLine(3, "%d", score);
    sleep(5000);
}
