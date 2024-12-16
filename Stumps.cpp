#include <iostream>
using namespace std;

struct Player{
    string name;
    int runs;
    int balls;
    int fours;
    int sixes;
    float strikeRate;

    Player(string n) : name(n), runs(0), balls(0), fours(0), sixes(0), strikeRate(0.0) {}
};

class BattingOrder {
public:
    queue<Player> batsmen;

    void enqueue(Player p) {
        batsmen.push(p);
    }

    Player dequeue() {
        if (batsmen.empty()) {
            throw runtime_error("No batsmen left");
        }
        Player p = batsmen.front();
        batsmen.pop();
        return p;
    }

    Player& front() {
        if (batsmen.empty()) {
            throw runtime_error("No batsmen left");
        }
        return batsmen.front();
    }

    void rotateStrike() {
        if (batsmen.size() > 1) {
            Player p = dequeue();
            enqueue(p);
        }
    }

    bool empty() {
        return batsmen.empty();
    }
};

// Stack to represent bowler performance
class BowlerPerformance {
public:
    stack<int> wickets;
    stack<int> runs;

    void pushWicket(int w) {
        wickets.push(w);
    }

    void pushRuns(int r) {
        runs.push(r);
    }

    int popWicket() {
        if (wickets.empty()) {
            throw runtime_error("No wickets to pop");
        }
        int w = wickets.top();
        wickets.pop();
        return w;
    }

    int popRuns() {
        if (runs.empty()) {
            throw runtime_error("No runs to pop");
        }
        int r = runs.top();
        runs.pop();
        return r;
    }

    bool empty() {
        return wickets.empty() && runs.empty();
    }
};

// Class to represent a Team
class Team {
public:
    string name;
    BattingOrder battingOrder;
    int totalRuns;
    int totalWickets;

    Team(string n, int playerCount) : name(n), totalRuns(0), totalWickets(0) {
        for (int i = 0; i < playerCount; i++) {
            string playerName;
            cout << "Enter name of player " << (i + 1) << ": ";
            cin >> playerName;
            battingOrder.batsmen.push(Player (playerName));
        }
    }

    void updateScore(int runs, int balls, bool isFour, bool isSix) {
        totalRuns += runs;
        if (runs > 0) {
            Player& currentBatsman = battingOrder.front();
            currentBatsman.runs += runs;
            currentBatsman.balls += balls;
            if (isFour) currentBatsman.fours++;
            if (isSix) currentBatsman.sixes++;
            currentBatsman.strikeRate = (currentBatsman.runs / (float)currentBatsman.balls) * 100;
        }
    }

    void nextBatsman() {
        if (!battingOrder.empty()) {
            battingOrder.dequeue();
        }
    }

    void rotateStrike() {
        battingOrder.rotateStrike();
    }
};

class Match{
public:
    Team team1;
    Team team2;
    int overs;
    int currentOver;
    int ballsInCurrentOver;
    int target;
    int currentScore;
    BowlerPerformance bowlerPerformance;
    Team* battingTeam;
    Team* bowlingTeam;

    Match(Team t1, Team t2, int o) : team1(t1), team2(t2), overs(o), currentOver(0), ballsInCurrentOver(0), currentScore(0) {
        target = team1.totalRuns + 1; // Target for team 2
        battingTeam = &team1;
        bowlingTeam = &team2;
    }
};

struct MatchNode {
    Match match;
    MatchNode* next;

    MatchNode(Match m) : match(m), next(NULL) {}
};

int main(){
        int choice;
    MatchHistory history;

    do {
        cout << "\nCricket Scoring System\n";
        cout << "1. Start New Match\n";
        cout << "2. View Match History\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                startMatch(history);
                break;
            case 2:
                history.displayHistory();
                break;
            case 3:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please select again.\n";
        }
    } while (choice != 3);
  return 0;
}
