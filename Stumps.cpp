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
        Player p = batsmen.front();
        batsmen.pop();
        return p;
    }

    Player& front() {
        return batsmen.front();
    }

    bool empty() {
        return batsmen.empty();
    }
};

int playerCount;

struct BowlerNode {
    string name;
    string teamName; // New attribute for team association
    int wickets;
    int runsConceded;
    int balls;
    BowlerNode* next;

    BowlerNode(string n, string team) : name(n), teamName(team), wickets(0), runsConceded(0), balls(0), next(nullptr) {}
};

class BowlerPerformance {
public:
    BowlerNode* head;

    BowlerPerformance() : head(nullptr) {}

    void addOrUpdateBowler(string bowlerName, string teamName, int runs, int balls, bool wicket = false) {
        BowlerNode* curr = head;
        BowlerNode* prev = nullptr;

        while (curr && curr->name != bowlerName) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == nullptr) {
            BowlerNode* newBowler = new BowlerNode(bowlerName, teamName);
            if (prev == nullptr) {
                head = newBowler;
            } else {
                prev->next = newBowler;
            }
            curr = newBowler;
        }

        curr->runsConceded += runs;
        curr->balls += balls;
        if (wicket) {
            curr->wickets++;
        }
    }

    void displayBowlerStats(string bowlerName) {
        BowlerNode* curr = head;
        while (curr && curr->name != bowlerName) {
            curr = curr->next;
        }

        if (curr) {
            cout << "Bowler: " << curr->name << " | Team: " << curr->teamName
                 << " | Wickets: " << curr->wickets
                 << " | Runs Conceded: " << curr->runsConceded << endl;
        } else {
            cout << "New bowler introduced into the attack" << endl;
        }
    }

    void displayAndSaveWickets(ofstream& file, string teamName) {
        vector<pair<int, string>> bowlers;
        BowlerNode* curr = head;

        while (curr) {
            if (curr->teamName == teamName) {
                bowlers.emplace_back(curr->wickets, curr->name);
            }
            curr = curr->next;
        }

        sort(bowlers.rbegin(), bowlers.rend());

        file << "Bowler Performance for " << teamName << " (Wickets and Runs Conceded):\n";
        for (const auto& bowler : bowlers) {
            curr = head;
            while (curr && curr->name != bowler.second) {
                curr = curr->next;
            }
            if (curr) {
                file << curr->name << ": " << curr->wickets << " wickets, " << curr->runsConceded << " runs\n";
            }
        }
    }
};

class Team {
public:
    string name;
    BattingOrder battingOrder;
    int totalRuns;
    int totalWickets;
    map<string, int> batsmanScores;
    vector<string> bowlers;
    int size;

    Team(string n, int playerCount) : name(n), totalRuns(0), totalWickets(0) {
        for (int i = 0; i < playerCount; i++) {
            string playerName;
            cout << "Enter name of player " << (i + 1) << ": ";
            cin >> playerName;
            battingOrder.enqueue(Player(playerName));
            bowlers.push_back(playerName);
            size = playerCount;
        }
    }

    int getsize() {
        return battingOrder.batsmen.size();
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
        currentBatsman = battingOrder.dequeue();
        totalWickets++;
    }

    bool allOut() {
        return totalWickets == size - 1;
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

    void playInnings(Team& battingTeam, Team& bowlingTeam) {
        int ballsPlayed = 0;
        int currentOver = 0;
        Player striker = battingTeam.battingOrder.dequeue();
        Player nonStriker = battingTeam.battingOrder.dequeue();
        string bowlerName;

        while (ballsPlayed < overs * 6 && !battingTeam.allOut()) {
            if (ballsPlayed % 6 == 0) {
                cout << "Enter bowler for the next over: ";
                cin >> bowlerName;
            }

            bowlerPerformance.displayBowlerStats(bowlerName);

            string input;
            cout << "Enter ball outcome (runs: 0-6, w for wicket, nb for no-ball, wd for wide): ";
            cin >> input;

            if (input == "w") {
                bowlerPerformance.addOrUpdateBowler(bowlerName, bowlingTeam.name, 0, 1, true);
                battingTeam.nextBatsman(striker);
                ballsPlayed++;
            } else if (input == "nb") {
                int extras;
                cout << "Any runs scored on no ball: ";
                cin >> extras;
                battingTeam.totalRuns++;
                battingTeam.updateScore(striker, extras, 1, extras == 4, extras == 6);
                bowlerPerformance.addOrUpdateBowler(bowlerName, bowlingTeam.name, 1 + extras, 0, false);
            } else if (input == "wd") {
                battingTeam.totalRuns++;
                bowlerPerformance.addOrUpdateBowler(bowlerName, bowlingTeam.name, 1, 0, false);
            } else {
                int runs = stoi(input);
                bowlerPerformance.addOrUpdateBowler(bowlerName, bowlingTeam.name, runs, 1, false);
                battingTeam.updateScore(striker, runs, 1, runs == 4, runs == 6);
                cout << "Batsman: " << striker.name
                     << " | Runs: " << striker.runs
                     << " | Balls: " << striker.balls
                     << " | Strike Rate: " << fixed << setprecision(2) << striker.strikeRate << endl;
                if (runs % 2 != 0) {
                    swap(striker, nonStriker);
                }
                ballsPlayed++;
            }

            currentOver = ballsPlayed / 6;
            cout << "Current Over: " << currentOver << "." << ballsPlayed % 6
                 << " | Current Score: " << battingTeam.totalRuns << "/" << battingTeam.totalWickets << endl;

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
        int toss_result = rand() % 2;
        string tossResult = (toss_result == 0) ? "heads" : "tails";
        cout << "Toss Result: " << tossResult << endl;

        Team* tossWinner;
        Team* tossLoser;

        if (tossChoice == tossResult) {
            cout << team1.name << " wins the toss!\n";
            tossWinner = &team1;
            tossLoser = &team2;
        } else {
            cout << team2.name << " wins the toss!\n";
            tossWinner = &team2;
            tossLoser = &team1;
        }

        string choice;
        cout << tossWinner->name << ", choose to bat or bowl (bat/bowl): ";
        cin >> choice;

        if (choice == "bat") {
            cout << "\n" << tossWinner->name << " is batting first.\n";
            playInnings(*tossWinner, *tossLoser);
            target = tossWinner->totalRuns + 1;
            cout << "Target for " << tossLoser->name << ": " << target << "\n";
            playInnings(*tossLoser, *tossWinner);
        } else {
            cout << "\n" << tossLoser->name << " is batting first.\n";
            playInnings(*tossLoser, *tossWinner);
            target = tossLoser->totalRuns + 1;
            cout << "Target for " << tossWinner->name << ": " << target << "\n";
            playInnings(*tossWinner, *tossLoser);
        }

        declareWinner();
        saveMatchHistory();
    }

    void declareWinner() {
        if (team2.totalRuns >= target) {
            cout << team2.name << " Wins by " << playerCount - team2.totalWickets - 1 << " wickets!" << endl;
        } else if (team1.totalRuns >= target) {
            cout << team1.name << " Wins by " << playerCount - team1.totalWickets - 1 << " wickets!" << endl;
        } else if (team2.totalRuns == team1.totalRuns) {
            cout << "Match tied! No super over to follow" << endl;
        } else if (team1.totalRuns < target - 1 && team2.totalRuns == target - 1) {
            cout << team2.name << " Wins by " << target - 1 - team1.totalRuns << " runs!" << endl;
        } else if (team2.totalRuns < target - 1 && team1.totalRuns == target - 1) {
            cout << team1.name << " Wins by " << target - 1 - team2.totalRuns << " runs!" << endl;
        }
    }

    void saveMatchHistory() {
        ofstream file("match_history.txt", ios::app);
        if (file.is_open()) {
            file << "Match Summary:\n";
            file << "Team 1: " << team1.name << " | Runs: " << team1.totalRuns << " | Wickets: " << team1.totalWickets << "\n";
            file << "Team 2: " << team2.name << " | Runs: " << team2.totalRuns << " | Wickets: " << team2.totalWickets << "\n";

            if (team2.totalRuns >= target) {
                file << team2.name << " Wins by " << playerCount - team2.totalWickets - 1 << " wickets!" << endl;
            } else if (team1.totalRuns >= target) {
                file << team1.name << " Wins by " << playerCount - team1.totalWickets - 1 << " wickets!" << endl;
            } else if (team2.totalRuns == team1.totalRuns) {
                file << "Match tied! No super over to follow" << endl;
            } else if (team1.totalRuns < target - 1 && team2.totalRuns == target - 1) {
                file << team2.name << " Wins by " << target - 1 - team1.totalRuns << " runs!" << endl;
            } else if (team2.totalRuns < target - 1 && team1.totalRuns == target - 1) {
                file << team1.name << " Wins by " << target - 1 - team2.totalRuns << " runs!" << endl;
            }

            file << "Batsman Scores of " << team1.name << " (Descending Order):\n";
            team1.displayAndSaveBatsmanScores(file);

            file << "Batsman Scores of " << team2.name << " (Descending Order):\n";
            team2.displayAndSaveBatsmanScores(file);

            bowlerPerformance.displayAndSaveWickets(file, team1.name);
            bowlerPerformance.displayAndSaveWickets(file, team2.name);

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
                string s1, s2;
                cin.ignore();
                cout << "Enter team 1: ";
                getline(cin, s1);
                cout << "Enter team 2: ";
                getline(cin, s2);

                cout << "Enter number of players per team: ";
                cin >> playerCount;

                Team team1(s1, playerCount);
                Team team2(s2, playerCount);

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
