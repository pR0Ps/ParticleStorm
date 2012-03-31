#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include <vector>
#include <QImage>
#include "gameengine.h"
#include "soundmanager.h"

class GameEngine;

class MainWindow : public QGLWidget{
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //define in implementation
    static const int MAX_X;
    static const int MAX_Y;
    static const int MAX_DIST;
    static const int NUM_STARS;
    static const int STAR_PAN_X;
    static const int STAR_PAN_Y;
    static const int CURSOR_OFFSET;
    static const int MAX_HIGHSCORES;
    static const int HIGHSCORES_SPACING;
    static const int MAX_HIGHSCORE_LETTERS;
    static const int HIGHSCORE_ENTRY_SPACING;
    static const int HIGHSCORE_ENTRY_BASEPOS;
    static const char* HS_FILE;

    /*
     * This function returns the position of the mouse relative to the OpenGL
     * canvas, for the purpose of polling player input and updating the location
     * of the player's avatar.
     *
     * Note: can use the x and y member functions to get the individual
     * coordinates of the QPoint returned.
     */
    QPoint getMousePos();
    Qt::MouseButtons getMouseState();
    bool getKeyPressed(int k);

    //get this instance
    static MainWindow* getInstance(){return instance;}

    int getGameType(){return gameType;}
    void shakeGameScreen(double time, int amt);
    void doneGame(unsigned int score);
    void pauseGame();

private:
    //self reference
    static MainWindow *instance;

    enum Mode{
        MENU,
        HIGHSCORES,
        HSENTRY,
        CREDITS
    };
    Mode menuMode;

    GameEngine *engine;
    SoundManager *soundManager;

    //star stuff
    struct mStar{
        double x;
        double y;
        double dist;
        mStar(double x, double y, double dist){
            this->x = x;
            this->y = y;
            this->dist = dist;
        }
    };
    std::vector<mStar*> *stars;
    const QColor *starClr;

    //timing stuff
    QTime *timer;

    //button stuff
    struct Button{
        Button(double x1, double y1, double x2, double y2, bool enabled = true){
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
            this->down = false;
            this->enabled = enabled;
        }
        bool mouseOver(QPoint point){
            if (!enabled) return false;
            return Util::coordInRect(point.x(), point.y(), x1, y1, x2, y2);
        }
        double x1;
        double y1;
        double x2;
        double y2;
        bool down;
        bool enabled;
    };
    Button *levelButton;
    Button *endlessButton;
    Button *zenButton;
    Button *resumeButton;
    Button *soundButton;
    Button *highScoresButton;
    Button *creditsButton;
    Button *backButton;
    Button *exitButton;

    //entry
    Button *upEntry;
    Button *downEntry;
    Button *nextEntry;
    int currEntry;
    std::vector<char> *nameEnter;
    int currScore;
    void setCurrLetter(int i);

    void displayHover(Button *b, bool l, bool r);

    //highscores
    struct HighScoreEntry{
        HighScoreEntry(std::string name, int score){
            this->name = name;
            this->score = score;
        }
        std::string name;
        int score;
    };
    static bool compareHS(const HighScoreEntry *h1, const HighScoreEntry *h2){return h1->score > h2->score;}

    std::vector<HighScoreEntry*> *highScoreValues;

    void setMode(Mode m);
    void loadScores();
    void saveScores();
    void sortScores();
    void addScore(std::string name, int score);

    QPoint currMousePos;

    //textures
    GLuint fontTex;
    GLuint titleTex;
    GLuint cursorTex;

    //zen/nonstop/leveled
    int gameType;

    //there is a game that's paused
    bool pausedGame;

    //sound
    bool muted;

    void initStars();
    void launchGame();

protected:
    void initializeGL();
    void paintGL();
    void timerEvent(QTimerEvent*);
    void update();

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    GLuint loadTexture(const char* c);
};


#endif // MAINWINDOW_H
