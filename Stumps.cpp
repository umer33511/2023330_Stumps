#include <bits/stdc++.h>
#include "sorting.cpp"
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
    map<string, int> wickets;

    void addWicket(string bowlerName) {
        wickets[bowlerName]++;
    }

    void displayAndSaveWickets(ofstream& file) {
    int size = wickets.size();
    int* wicketsCount = new int[size];
    string* names = new string[size];

    int index = 0;
    for(auto& [name, count] : wickets) {
        wicketsCount[index] = count;
        names[index++] = name;
    }

    countingSort(wicketsCount, names, size);

    file << "Bowler Wickets (Descending Order):\n";
    for (int i = 0; i < size; i++) {
        file << names[i] << ": " << wicketsCount[i] << " wickets\n";
    }

    delete[] wicketsCount;
    delete[] names;
}
};

class Team {
public:
    string name;
    BattingOrder battingOrder;
    int totalRuns;
    int totalWickets;
    map<string, int> batsmanScores; // Track scores of batsmen

    Team(string n, int playerCount) : name(n), totalRuns(0), totalWickets(0) {
        for (int i = 0; i < playerCount; i++) {
            string playerName;
            cout << "Enter name of player " << (i + 1) << ": ";
            cin >> playerName;
            battingOrder.enqueue(Player(playerName));
        }
    }

    void updateScore(Player& batsman, int runs, int balls, bool isFour, bool isSix) {
        batsman.runs += runs;
        batsmanScores[batsman.name] += runs;
        batsman.balls += balls;
        if (isFour) batsman.fours++;
        if (isSix) batsman.sixes++;
        batsman.strikeRate = (batsman.runs / (float)batsman.balls) * 100;
        totalRuns += runs;
    }

    void nextBatsman(Player& currentBatsman) {
        if (!battingOrder.empty()) {
            currentBatsman = battingOrder.dequeue();
            totalWickets++;
        }
    }

    bool allOut() {
        return battingOrder.empty();
    }

    void displayAndSaveBatsmanScores(ofstream& file) {
    int size = batsmanScores.size();
    int* scores = new int[size];
    string* names = new string[size];

    int index = 0;
    for (auto& [name, score] : batsmanScores) {
        scores[index] = score;
        names[index++] = name;
    }

    mergeSort(scores, names, 0, size - 1);

    file << "Batsman Scores (Descending Order):\n";
    for (int i = 0; i < size; i++) {
        file << names[i] << ": " << scores[i] << " runs\n";
    }

    delete[] scores;
    delete[] names;
}
};

class Match {
public:
    Team team1;
    Team team2;
    int overs;
    int target;
    BowlerPerformance bowlerPerformance;

    Match(Team t1, Team t2, int o) : team1(t1), team2(t2), overs(o), target(0) {}

    void playInnings(Team& battingTeam, string bowlerName) {
        int ballsPlayed = 0;
        int currentOver = 0;
        Player striker = battingTeam.battingOrder.dequeue();
        Player nonStriker = battingTeam.battingOrder.dequeue();

        while (ballsPlayed < overs * 6 && !battingTeam.allOut()) {
            string input;
            cout << "Enter ball outcome (runs: 0-6, w for wicket, nb for no-ball): ";
            cin >> input;

            if (input == "w") {
                bowlerPerformance.addWicket(bowlerName);
                cout << "Wicket! Total Wickets: " << battingTeam.totalWickets + 1 << endl;
                battingTeam.nextBatsman(striker);
            } else if (input == "nb") {
                battingTeam.totalRuns++;
                cout << "No Ball! 1 run added. Total Runs: " << battingTeam.totalRuns << endl;
            } else {
                int runs = stoi(input);
                battingTeam.updateScore(striker, runs, 1, runs == 4, runs == 6);
                cout << "Batsman: " << striker.name 
                     << " | Runs: " << striker.runs 
                     << " | Balls: " << striker.balls
                     << " | Strike Rate: " << fixed << setprecision(2) << striker.strikeRate << endl;
                if (runs % 2 != 0) {
                    swap(striker, nonStriker);
                }
            }
            ballsPlayed++;
            currentOver = ballsPlayed / 6;
            cout << "Current Over: " << currentOver << "." << ballsPlayed % 6 << " | Current Score: " << battingTeam.totalRuns << "/" << battingTeam.totalWickets << endl;
            
            if (ballsPlayed % 6 == 0) {
                swap(striker, nonStriker);
                cout << "End of Over! Striker and Non-Striker swapped." << endl;
            }

            if (target > 0 && battingTeam.totalRuns >= target) {
                cout << battingTeam.name << " has chased the target successfully!" << endl;
                return;
            }
        }
        cout << "Innings Over: " << battingTeam.totalRuns << "/" << battingTeam.totalWickets << endl;
    }

    void startMatch() {
        string tossChoice, bowler1, bowler2;
        cout << "Enter your choice for the toss (heads/tails): ";
        cin >> tossChoice;

        srand(time(0));
        string tossResult = (rand() % 2 == 0) ? "heads" : "tails";
        cout << "Toss Result: " << tossResult << endl;

        Team* tossWinner;
        Team* tossLoser;

        if (tossChoice == tossResult) {
            cout << "Team 1 wins the toss!\n";
            tossWinner = &team1;
            tossLoser = &team2;
        } else {
            cout << "Team 2 wins the toss!\n";
            tossWinner = &team2;
            tossLoser = &team1;
        }

        string choice;
        cout << tossWinner->name << ", choose to bat or bowl (bat/bowl): ";
        cin >> choice;

        if (choice == "bat") {
            cout << "\n" << tossWinner->name << " is batting first.\n";
            cout << "Enter name of Bowler for " << tossLoser->name << ": ";
            cin >> bowler1;
            playInnings(*tossWinner, bowler1);
            target = tossWinner->totalRuns + 1;
            cout << "Target for " << tossLoser->name << ": " << target << "\n";
            cout << "Enter name of Bowler for " << tossWinner->name << ": ";
            cin >> bowler2;
            playInnings(*tossLoser, bowler2);
        } else {
            cout << "\n" << tossLoser->name << " is batting first.\n";
            cout << "Enter name of Bowler for " << tossWinner->name << ": ";
            cin >> bowler1;
            playInnings(*tossLoser, bowler1);
            target = tossLoser->totalRuns + 1;
            cout << "Target for " << tossWinner->name << ": " << target << "\n";
            cout << "Enter name of Bowler for " << tossLoser->name << ": ";
            cin >> bowler2;
            playInnings(*tossWinner, bowler2);
        }

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
            team1.displayAndSaveBatsmanScores(file);
            team2.displayAndSaveBatsmanScores(file);
            bowlerPerformance.displayAndSaveWickets(file);
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
