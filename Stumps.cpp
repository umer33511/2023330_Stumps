#include <iostream>
#include <queue>
#include <stack>
#include <string>

using namespace std;

struct Player {
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

class BowlerPerformance {
public:
    stack<int> wickets;
    stack<int> runs;

    void pushWicket(int w) {
        wickets.push(w);
    }

    void pushRuns(int r) {
        runs.push(r);
        if (r > 0) { // Only update if runs were scored
            bowlingTeam->bowlerPerformance.pushRuns(r); // Update bowling team's performance
        }
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
            battingOrder.enqueue(Player(playerName));
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

class Match {
public:
    Team team1;
    Team team2;
    int overs;
    int currentOver;
    int ballsInCurrentOver;
    int target;
    int currentScore;
    BowlerPerformance bowlerPerformance; // Shared performance for both teams

private:
    Team* battingTeam;
    Team* bowlingTeam;

    void swapTeams() {
        swap(battingTeam, bowlingTeam);  // swap pointers for efficient team switching
    }

public:
    Match(Team t1, Team t2, int o) : team1(t1), team2(t2), overs(o), currentOver(0), ballsInCurrentOver(0), currentScore(0) {
        target = team1.totalRuns + 1; // Target for team 2
        battingTeam = &team1;
        bowlingTeam = &team2;
    }

    void playInnings() {
        while (currentOver < overs && battingTeam->totalWickets < 10) {
            // Simulate a ball
            int runs = rand() % 7; // Random runs from 0 to 6
            bool isFour = runs == 4;
            bool isSix = runs == 6;

            battingTeam->updateScore(runs, 1, isFour, isSix);
            currentScore += runs;
            bowlerPerformance.pushRuns(runs);

            if (runs == 0) {
                ballsInCurrentOver++;
                if (ballsInCurrentOver == 6) {
                    currentOver++;
                    ballsInCurrentOver = 0;
                    swapTeams(); // Switch teams after each over
                }
            } else {
                battingTeam->rotateStrike();
                ballsInCurrentOver = 0;
                currentOver++;
                swapTeams(); // Switch teams after a boundary or wicket
            }

            if (runs == 0 && rand() % 10 == 0) { // Random wicket
                battingTeam->nextBatsman();
                battingTeam->totalWickets++;
                bowlerPerformance.pushWicket(1);
            }
        }
    }
};

struct MatchNode {
    Match match;
    MatchNode* next;

    MatchNode(Match m) : match(m), next(NULL) {}
};

class MatchHistory {
public:
    MatchNode* head;

    MatchHistory() {
        head = nullptr;
    }

    void addMatch(Match match) {
        MatchNode* newNode = new MatchNode(match);
        newNode->next = head;
        head = newNode;
    }

    void displayHistory() {
        MatchNode* current = head;
        while (current != nullptr) {
            cout << "Match Details:\n";
            cout << "Team 1: " << current->match.team1.name << "\n";
            cout << "Team 2: " << current->match.team2.name << "\n";
            cout << "Overs: " << current->match.overs << "\n";
            cout << "Target: " << current->match.target << "\n";
            // Add more details as needed
            current = current->next;
        }
    }
};

int main() {
    MatchHistory history;

    do {
        cout << "\nCricket Scoring System\n";
        cout << "1. Start New Match\n";
        cout << "2. View Match History\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                int playerCount;
                cout << "Enter number of players per team: ";
                cin >> playerCount;

                Team team1("Team 1", playerCount);
                Team team2("Team 2", playerCount);

                int overs;
                cout << "Enter number of overs per innings: ";
                cin >> overs;

                Match match(team1, team2, overs);
                match.playInnings();
                // Second innings can be simulated similarly
                history.addMatch(match);
                break;
            }
            case 2: {
                history.displayHistory();
                break;
            }
            case 3: {
                cout << "Exiting the system. Goodbye!\n";
                break;
            }
            default: {
                cout << "Invalid choice! Please select again.\n";
            }
        }
    } while (choice != 3);

    return 0;
}
