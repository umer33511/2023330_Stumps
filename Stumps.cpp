#include <bits/stdc++.h>
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
        Player& currentBatsman = battingOrder.front();
        currentBatsman.runs += runs;
        currentBatsman.balls += balls;
        if (isFour) currentBatsman.fours++;
        if (isSix) currentBatsman.sixes++;
        currentBatsman.strikeRate = (currentBatsman.runs / (float)currentBatsman.balls) * 100;
        totalRuns += runs;
    }

    void nextBatsman() {
        battingOrder.dequeue();
        totalWickets++;
    }

    bool allOut() {
        return battingOrder.empty();
    }
};

class Match {
public:
    Team team1;
    Team team2;
    int overs;
    int target;

    Match(Team t1, Team t2, int o) : team1(t1), team2(t2), overs(o), target(0) {}

    void playInnings(Team& battingTeam) {
        int ballsPlayed = 0;
        while (ballsPlayed < overs * 6 && !battingTeam.allOut()) {
            string input;
            cout << "Enter ball outcome (runs: 0-6, w for wicket, nb for no-ball): ";
            cin >> input;

            if (input == "w") {
                battingTeam.nextBatsman();
                cout << "Wicket! Total Wickets: " << battingTeam.totalWickets << endl;
            } else if (input == "nb") {
                battingTeam.totalRuns++;
                cout << "No Ball! 1 run added. Total Runs: " << battingTeam.totalRuns << endl;
            } else {
                int runs = stoi(input);
                battingTeam.updateScore(runs, 1, runs == 4, runs == 6);
            }
            ballsPlayed++;
            cout << "Current Score: " << battingTeam.totalRuns << "/" << battingTeam.totalWickets << endl;
            if (target > 0 && battingTeam.totalRuns >= target) {
                cout << battingTeam.name << " has chased the target successfully!" << endl;
                return;
            }
        }
        cout << "Innings Over: " << battingTeam.totalRuns << "/" << battingTeam.totalWickets << endl;
    }

    void startMatch() {
        cout << "\nTeam 1 Batting:\n";
        playInnings(team1);
        target = team1.totalRuns + 1;
        cout << "Target for Team 2: " << target << "\n";
        cout << "\nTeam 2 Batting:\n";
        playInnings(team2);
        declareWinner();
        saveMatchHistory();
    }

    void declareWinner() {
        if (team2.totalRuns >= target) {
            cout << "Team 2 Wins!" << endl;
        } else if (team1.totalRuns > team2.totalRuns) {
            cout << "Team 1 Wins!" << endl;
        } else {
            cout << "Match Drawn!" << endl;
        }
    }

    void saveMatchHistory() {
        ofstream file("match_history.txt", ios::app);
        if (file.is_open()) {
            file << "Match Summary:\n";
            file << "Team 1: " << team1.name << " | Runs: " << team1.totalRuns << " | Wickets: " << team1.totalWickets << "\n";
            file << "Team 2: " << team2.name << " | Runs: " << team2.totalRuns << " | Wickets: " << team2.totalWickets << "\n";
            if (team2.totalRuns >= target) {
                file << "Result: Team 2 Wins!\n";
            } else if (team1.totalRuns > team2.totalRuns) {
                file << "Result: Team 1 Wins!\n";
            } else {
                file << "Result: Match Drawn!\n";
            }
            file << "------------------------------------------\n";
            file.close();
            cout << "Match history saved to file." << endl;
        } else {
            cout << "Error: Unable to open file to save match history." << endl;
        }
    }
};

int main() {
    int choice;
    do {
        cout << "\nCricket Scoring System\n";
        cout << "1. Start New Match\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int playerCount;
                cout << "Enter number of players per team: ";
                cin >> playerCount;

                Team team1("Team 1", playerCount);
                Team team2("Team 2", playerCount);

                int overs;
                cout << "Enter number of overs: ";
                cin >> overs;

                Match match(team1, team2, overs);
                match.startMatch();
                break;
            }
            case 2: {
                cout << "Exiting the system. Goodbye!\n";
                break;
            }
            default: {
                cout << "Invalid choice! Please select again.\n";
                break;
            }
        }
    } while (choice != 2);

    return 0;
}
