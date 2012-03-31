#include "mainwindow.h"
#include "gameengine.h"
#include "util.h"
#include "algorithm"
#include "fstream"
#include "ostream"

const int MainWindow::MAX_X = 600;
const int MainWindow::MAX_Y = 600;
const int MainWindow::MAX_DIST = 10;
const int MainWindow::NUM_STARS = 255;
const int MainWindow::STAR_PAN_X = 500;
const int MainWindow::STAR_PAN_Y = 0;
const int MainWindow::CURSOR_OFFSET = 10;
const int MainWindow::MAX_HIGHSCORES = 10;
const int MainWindow::HIGHSCORES_SPACING = 35;
const int MainWindow::MAX_HIGHSCORE_LETTERS = 3;
const int MainWindow::HIGHSCORE_ENTRY_SPACING = 20;
const int MainWindow::HIGHSCORE_ENTRY_BASEPOS = MAX_Y - 350;
const char* MainWindow::HS_FILE = "highscores.dat";

MainWindow* MainWindow::instance = NULL;

MainWindow::MainWindow(QWidget *parent) : QGLWidget(parent){
    setWindowTitle("Particle Storm");
    setFixedSize(MAX_X,MAX_Y);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    //set up the game engine
    engine = new GameEngine();
    engine->setMouseTracking(true);

    //self reference
    instance = this;

    gameType = 0;
    muted = false;
    pausedGame = false;

    //init stars
    initStars();

    //Menu buttons
    levelButton = new Button(145, 300, 440, 330);
    endlessButton = new Button(110, 200, 470, 230);
    zenButton = new Button(170, 100, 410, 130);
    resumeButton = new Button(MAX_X - 186, 6, MAX_X - 6, 36);
    backButton = new Button(6, 6, 126, 36);
    soundButton = new Button (MAX_X - 180, MAX_Y - 15, MAX_X, MAX_Y);
    highScoresButton = new Button (0, MAX_X - 15, 150, MAX_X);
    creditsButton = new Button (245, MAX_Y - 15, 350, MAX_Y);
    exitButton = new Button(6, 6, 126, 36);
    //entry (set them later)
    upEntry = new Button(0, 0, 0, 0);
    downEntry = new Button(0, 0, 0, 0);
    nextEntry = new Button(0, 0, 0,0);

    //highscores
    highScores = new std::vector<HighScoreEntry*>();
    loadScores();
    //name entry
    setCurrLetter(0);
    nameEnterCharacters = new std::vector<char>();
    for (int i = 0 ; i < MAX_HIGHSCORE_LETTERS ; i++)
        nameEnterCharacters->push_back('A');

    //timing stuff
    timer = new QTime();
    timer->start();
    startTimer(1/(double)GameEngine::MAX_FPS*1000);

    //sounds
    soundManager = new SoundManager();
    soundManager->playSound(SoundManager::TITLE);

    //start with the main menu
    setMode(MENU);
}

MainWindow::~MainWindow(){  
    while(!stars->empty()) delete stars->back(), stars->pop_back();
    delete stars;
    delete starClr;
    delete timer;
    delete levelButton;
    delete zenButton;
    delete endlessButton;
    delete resumeButton;
    delete backButton;
    delete soundButton;
    delete creditsButton;
    delete highScoresButton;
    delete exitButton;
    //entry
    delete upEntry;
    delete downEntry;
    delete nextEntry;
    delete nameEnterCharacters;

    while(!highScores->empty()) delete highScores->back(), highScores->pop_back();
    delete highScores;

    delete soundManager;
    delete engine;
}

void MainWindow::doneGame(const unsigned int score){
    pausedGame = false;
    engine->setVisible(false);
    instance->setVisible(true);
    engine->reset();
    currScore = score;
    setMode(HSENTRY);
}

void MainWindow::pauseGame(){
    pausedGame = true;
    engine->setVisible(false);
    instance->setVisible(true);
    soundManager->playSound(muted ? SoundManager::NONE : SoundManager::TITLE);
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (levelButton->mouseOver(currMousePos)) levelButton->down = true;
    else if (endlessButton->mouseOver(currMousePos)) endlessButton->down = true;
    else if (zenButton->mouseOver(currMousePos)) zenButton->down = true;
    else if (resumeButton->mouseOver(currMousePos)) resumeButton->down = true;
    else if (backButton->mouseOver(currMousePos)) backButton->down = true;
    else if (soundButton->mouseOver(currMousePos)) soundButton->down = true;
    else if (creditsButton->mouseOver(currMousePos)) creditsButton->down = true;
    else if (highScoresButton->mouseOver(currMousePos)) highScoresButton->down = true;
    else if (exitButton->mouseOver(currMousePos)) exitButton->down = true;
    //entry
    else if (upEntry->mouseOver(currMousePos)) upEntry->down = true;
    else if (downEntry->mouseOver(currMousePos)) downEntry->down = true;
    else if (nextEntry->mouseOver(currMousePos)) nextEntry->down = true;
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    //starting a new game
    if (levelButton->mouseOver(currMousePos) && levelButton->down){
        gameType = LevelManager::LEVELED;
        launchGame();
    }
    else if(endlessButton->mouseOver(currMousePos) && endlessButton->down){
        gameType = LevelManager::NONSTOP;
        launchGame();
    }
    else if(zenButton->mouseOver(currMousePos) && zenButton->down){
        gameType = LevelManager::ZEN;
        launchGame();
    }

    //resume a game in progress
    else if (pausedGame && resumeButton->mouseOver(currMousePos) && resumeButton->down){
        instance->setVisible(false);
        engine->setVisible(true);
        engine->resume();
        soundManager->playSound(muted ? SoundManager::NONE : SoundManager::GAME);
    }

    //toggle menu modes
    else if (soundButton->mouseOver(currMousePos) && soundButton->down){
        muted = !muted;
        soundManager->playSound(muted ? SoundManager::NONE : SoundManager::TITLE);
    }
    else if (creditsButton->mouseOver(currMousePos) && creditsButton->down){
        setMode(menuMode == CREDITS ? MENU : CREDITS);
    }
    else if (highScoresButton->mouseOver(currMousePos) && highScoresButton->down){
        setMode(menuMode == HIGHSCORES ? MENU : HIGHSCORES);
    }
    else if (backButton->mouseOver(currMousePos) && backButton->down){
        setMode(MENU);
    }

    //exit
    else if (exitButton->mouseOver(currMousePos) && exitButton->down){
        //save scores
        saveScores();
        exit(0);
    }

    //highscore entry buttons
    else if (upEntry->mouseOver(currMousePos) && upEntry->down){
        //increment curr letter
        nameEnterCharacters->at(currEntry)++;
        if (nameEnterCharacters->at(currEntry) > 90) nameEnterCharacters->at(currEntry) = 65;
    }
    else if (downEntry->mouseOver(currMousePos) && downEntry->down){
        //decrement curr letter
        nameEnterCharacters->at(currEntry)--;
        if (nameEnterCharacters->at(currEntry) < 65) nameEnterCharacters->at(currEntry) = 90;
    }
    else if (nextEntry->mouseOver(currMousePos) && nextEntry->down){
        //move to the next letter/finish entry
        if (currEntry < MAX_HIGHSCORE_LETTERS - 1)
            setCurrLetter(currEntry + 1);
        else{
            std::string temp= "";
            for (int i = 0 ; i < MAX_HIGHSCORE_LETTERS ; i++)
                temp += nameEnterCharacters->at(i);
            addScore(temp, currScore);
            setCurrLetter(0);
            setMode(HIGHSCORES);
        }
    }

    //reset button states
    levelButton->down = false;
    endlessButton->down = false;
    zenButton->down = false;
    resumeButton->down = false;
    soundButton->down = false;
    highScoresButton->down = false;
    creditsButton->down = false;
    backButton->down = false;
    exitButton->down = false;
    //entry
    upEntry->down = false;
    downEntry->down = false;
    nextEntry->down = false;
}

void MainWindow::initializeGL(){
    //get current OpenGL context
    makeCurrent();

    //window stuff
    glViewport(0, 0, MAX_X, MAX_Y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, MAX_X, 0, MAX_Y); //orthogonal, not perspective
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);

    //load textures
    fontTex = loadTexture(":/Images/font.png");
    titleTex = loadTexture(":/Images/title.png");
    cursorTex = loadTexture(":/Images/cursor.png");
}

void MainWindow::timerEvent(QTimerEvent *){
    if (instance->isVisible()){
        update();
        updateGL();
    }
}

void MainWindow::update(){
    double deltaTime = timer->restart()/(float)1000;

    //update the sound manager
    soundManager->update(deltaTime);

    //get mouse position
    currMousePos = mapFromGlobal(QCursor::pos());
    currMousePos.setY(MAX_Y - currMousePos.y());

    //move stars
    const double panX = STAR_PAN_X * deltaTime;
    const double panY = STAR_PAN_Y * deltaTime;
    for (int i = 0 ; i < NUM_STARS ; i++){
        stars->at(i)->x += panX/(float)stars->at(i)->dist;
        stars->at(i)->y += panY/(float)stars->at(i)->dist;

        //wrap the star around when panned offscreen
        if (stars->at(i)->x > MAX_X)  stars->at(i)->x -= MAX_X;
        else if (stars->at(i)->x < 0) stars->at(i)->x = MAX_X - stars->at(i)->x;
        if (stars->at(i)->y > MAX_Y)  stars->at(i)->y -= MAX_Y;
        else if (stars->at(i)->y < 0) stars->at(i)->y = MAX_Y - stars->at(i)->y;
    }
}

//draw the scene
void MainWindow::paintGL(){
    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    //draw the stars
    glBegin(GL_POINTS);
    for(int i = 0; i < NUM_STARS; i++){
        glVertex2d(stars->at(i)->x, stars->at(i)->y);
    }
    glEnd();

    //qDebug() << currMousePos;

    //draw the menus
    if (menuMode == HIGHSCORES){ //highscores scene
        //highscores display
        Util::drawString("HIGHSCORES", MAX_X / 2, MAX_Y - 100, fontTex, true, false, 3, 3);
        for (int i = 0 ; i < std::min((int)highScores->size(), MAX_HIGHSCORES) ; i++){
            Util::drawString(Util::doubleToString(highScores->at(i)->score, 25, 0), 100, MAX_Y - 170 - i * HIGHSCORES_SPACING, fontTex);
            Util::drawString(Util::doubleToString(i+1, 2, 0) + ". " + highScores->at(i)->name, 100, MAX_Y - 170 - i * HIGHSCORES_SPACING, fontTex);
        }

        //buttons
        Util::drawString("HIGHSCORES", 0, MAX_Y - 15, fontTex);
        Util::drawString("CREDITS", MAX_X/2, MAX_Y - 15, fontTex, true);
        Util::drawString("TOGGLE SOUND", MAX_X - 180, MAX_Y - 15, fontTex);
        Util::drawString("BACK", 6, 6, fontTex, false, false, 2, 2, true);
        displayHover(backButton, false, true);

    }
    else if (menuMode == HSENTRY){ //entering highscore name
        Util::drawString("ENTER YOUR NAME", MAX_X/2, MAX_Y - 200, fontTex, true);
        //draw letters
        for (int i = 0 ; i < MAX_HIGHSCORE_LETTERS ; i++){
            Util::drawChar(nameEnterCharacters->at(i), (MAX_X - MAX_HIGHSCORE_LETTERS * HIGHSCORE_ENTRY_SPACING)/2 + i * HIGHSCORE_ENTRY_SPACING , MAX_Y - 350, fontTex);
        }

        //draw buttons
        Util::drawChar('+', upEntry->x1, upEntry->y1, fontTex);
        Util::drawChar('-', downEntry->x1, downEntry->y1, fontTex);
        Util::drawChar('>', nextEntry->x1, nextEntry->y1, fontTex);
    }
    else if (menuMode == CREDITS){ //show credits
        //draw credits
        Util::drawString("CREDITS", MAX_X / 2, MAX_Y - 100, fontTex, true, false, 3, 3);

        //could for loop it, but would have to define the array to iterate over
        Util::drawString("DANIEL BAMBRICK..RECORD KEEPER", MAX_X/2, MAX_Y - 175 - 0 * 55, fontTex, true);
        Util::drawString("MARK DION............AI MASTER", MAX_X/2, MAX_Y - 175 - 1 * 55, fontTex, true);
        Util::drawString("JULIA GREIG............MANAGER", MAX_X/2, MAX_Y - 175 - 2 * 55, fontTex, true);
        Util::drawString("DONALD KESTER...LEVEL DESIGNER", MAX_X/2, MAX_Y - 175 - 3 * 55, fontTex, true);
        Util::drawString("ANDREW MCMULLEN....CODE MONKEY", MAX_X/2, MAX_Y - 175 - 4 * 55, fontTex, true);
        Util::drawString("CAREY METCALFE.......ARCHITECT", MAX_X/2, MAX_Y - 175 - 5 * 55, fontTex, true);
        Util::drawString("LUKE WALKER...ABILITY DESIGNER", MAX_X/2, MAX_Y - 175 - 6 * 55, fontTex, true);

        //buttons
        Util::drawString("HIGHSCORES", 0, MAX_Y - 15, fontTex);
        Util::drawString("CREDITS", MAX_X/2, MAX_Y - 15, fontTex, true);
        Util::drawString("TOGGLE SOUND", MAX_X - 180, MAX_Y - 15, fontTex);
        Util::drawString("BACK", 6, 6, fontTex, false, false, 2, 2, true);
        displayHover(backButton, false, true);
    }
    else if (menuMode == MENU){ //regular menu scene
        //draw the button text
        Util::drawString("HIGHSCORES", 0, MAX_Y - 15, fontTex);
        Util::drawString("CREDITS", MAX_X/2, MAX_Y - 15, fontTex, true);
        Util::drawString("TOGGLE SOUND", MAX_X - 180, MAX_Y - 15, fontTex);

        Util::drawString("LEVEL MODE", MAX_X/2, 300, fontTex, true, false, 2, 2);
        displayHover(levelButton, true, true);

        Util::drawString("ENDLESS PLAY", MAX_X/2, 200, fontTex, true, false, 2, 2);
        displayHover(endlessButton, true, true);

        Util::drawString("ZEN MODE", MAX_X/2, 100, fontTex, true, false, 2, 2);
        displayHover(zenButton, true, true);

        Util::drawString("EXIT",6, 6, fontTex, false, false, 2, 2);
        displayHover(exitButton, false, true);

        if (pausedGame){
            Util::drawString("RESUME", MAX_X - 186, 6, fontTex, false, false, 2, 2, true);
            displayHover(resumeButton, true, false);
        }

        //draw the title ew ew hard code. IMG has width 552, height 106
        Util::drawTexture((MAX_X-552)/2, (MAX_Y-75-106), (((MAX_X-552)/2)+552), (MAX_Y-75), titleTex);
    }
}

//displays the hover icon(s)
void MainWindow::displayHover(Button *b, bool left, bool right){
    if (b->mouseOver(currMousePos)){
        if (left) Util::drawTexture(b->x1 - CURSOR_OFFSET - 23, b->y1 - 5, b->x1 - CURSOR_OFFSET, b->y2 + 5, cursorTex);
        if (right) Util::drawTexture(b->x2 + CURSOR_OFFSET + 23, b->y1 - 5, b->x2 + CURSOR_OFFSET, b->y2 + 5, cursorTex);
    }
}

//set the location of the +/-/> buttons
void MainWindow::setCurrLetter(int i){
    currEntry= i;
    //change button positions
    const int nX1 = (MAX_X - MAX_HIGHSCORE_LETTERS * HIGHSCORE_ENTRY_SPACING)/2 + (MAX_HIGHSCORE_LETTERS + 1) * HIGHSCORE_ENTRY_SPACING;
    const int cX1 = (MAX_X - MAX_HIGHSCORE_LETTERS * HIGHSCORE_ENTRY_SPACING)/2 + currEntry * HIGHSCORE_ENTRY_SPACING;

    //next button
    nextEntry->x1 = nX1;
    nextEntry->x2 = nX1 + HIGHSCORE_ENTRY_SPACING;
    nextEntry->y1 = HIGHSCORE_ENTRY_BASEPOS;
    nextEntry->y2 = HIGHSCORE_ENTRY_BASEPOS + HIGHSCORE_ENTRY_SPACING;

    //up button
    upEntry->x1 = cX1;
    upEntry->x2 = cX1 + Util::FONT_CHAR_DIMENSION_X;
    upEntry->y1 = HIGHSCORE_ENTRY_BASEPOS + Util::FONT_CHAR_DIMENSION_Y + 3;
    upEntry->y2 = HIGHSCORE_ENTRY_BASEPOS + Util::FONT_CHAR_DIMENSION_Y * 2 + 3;

    //down button
    downEntry->x1 = cX1;
    downEntry->x2 = cX1 + Util::FONT_CHAR_DIMENSION_X;
    downEntry->y1 = HIGHSCORE_ENTRY_BASEPOS - Util::FONT_CHAR_DIMENSION_Y - 3;
    downEntry->y2 = HIGHSCORE_ENTRY_BASEPOS - 3;
}

//launch the game
void MainWindow::launchGame(){
    if (pausedGame) engine->reset();
    instance->setVisible(false);
    engine->setVisible(true);
    engine->start(gameType);
    soundManager->playSound(muted ? SoundManager::NONE : SoundManager::GAME);
}

//init the stars
void MainWindow::initStars(){
    starClr = new QColor(255, 255, 255);
    stars = new std::vector<mStar*>;
    stars->reserve(NUM_STARS);
    for(int i = 0; i < NUM_STARS; i++){
        stars->push_back(new mStar(qrand() % MAX_X, qrand() % MAX_Y, Util::randInt(1,MAX_DIST)));
    }
}

//load a texture from a file
GLuint MainWindow::loadTexture(const char* c){
    GLuint ret;
    QImage tex;
    tex = QGLWidget::convertToGLFormat(QImage(c));
    if (tex.isNull()){
        qDebug() << "Error loading texture" << c;
        return 0;
    }
    //save settings
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    //generate texture slot
    glGenTextures(1, &ret);
    //bind, load, unbind texture
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    //restore settings
    glPopAttrib();
    return ret;
}

//highscores stuff
void MainWindow::loadScores(){
    std::ifstream inFile;
    inFile.open(HS_FILE, std::ifstream::in);

    //no highscores found
    if (inFile.fail()) return;

    //read in the highscores
    std::string tempName, tempScore;
    for(;;){
            inFile >> tempName;
            inFile >> tempScore;
            if (inFile.eof()) break;
            highScores->push_back(new HighScoreEntry(tempName, atoi(tempScore.c_str())));
    }
    sortScores();
}
void MainWindow::saveScores(){
    //write the scores to a file
    std::ofstream outFile;
    outFile.open(HS_FILE, std::ofstream::out | std::ofstream::trunc);
    for(unsigned int i = 0 ; i < highScores->size() ; i++){
        outFile << highScores->at(i)->name << "\n";
        outFile << highScores->at(i)->score << "\n";
    }
    outFile.flush();
    outFile.close();
}

void MainWindow::sortScores(){
    std::sort(highScores->begin(), highScores->end(), compareHS);
    while(highScores->size() > MAX_HIGHSCORES) highScores->pop_back();
}
void MainWindow::addScore(std::string name, int score){
    highScores->push_back(new HighScoreEntry(name, score));
    sortScores();
}
void MainWindow::setMode(Mode m){
    //set all the buttons
    levelButton->enabled = m == MENU;
    endlessButton->enabled =  m == MENU;
    zenButton->enabled =  m == MENU;
    resumeButton->enabled =  m == MENU;
    exitButton->enabled = m == MENU;
    creditsButton->enabled = (m == CREDITS || m == HIGHSCORES || m == MENU);
    backButton->enabled = (m == HIGHSCORES || m == CREDITS);
    highScoresButton->enabled = (m == CREDITS || m == HIGHSCORES || m == MENU);
    soundButton->enabled = (m == CREDITS || m == HIGHSCORES || m == MENU);
    //entry
    upEntry->enabled = (m == HSENTRY);
    downEntry->enabled = (m == HSENTRY);
    nextEntry->enabled = (m == HSENTRY);
    this->menuMode = m;
}

//ENGINE STUFF (refactor this?)
//engine mouse stuff
QPoint MainWindow::getMousePos(){
    return engine->getMousePos();
}
Qt::MouseButtons MainWindow::getMouseState(){
    return engine->getMouseState();
}
//engine keyboard stuff
bool MainWindow::getKeyPressed(int k){
    return engine->getKeyPressed(k);
}

void MainWindow::shakeGameScreen(double time, int amt){
    engine->shakeScreen(time, amt);
}
