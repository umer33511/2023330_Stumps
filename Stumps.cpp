#include <bits/stdc++.h>
#include "sorting.cpp"
#include "BowlerStack for sorting.cpp"
#include "BattingOrderQueue.cpp"
using namespace std;

int playerCount;
// Class for handling batting order of each team
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
// Node for linked list to store bowlers
struct BowlerNode {
    string name;
    string teamName;
    int wickets;
    int runsConceded;
    int balls;
    BowlerNode* next;

    BowlerNode(string n, string team) : name(n), teamName(team), wickets(0), runsConceded(0), balls(0), next(nullptr) {}
};
// Class for bowlers containing linked list insertion, traversal etc
class BowlerPerformance {
public:
    BowlerNode* head;

    BowlerPerformance() : head(nullptr) {}

    void addOrUpdateBowler(string bowlerName, string teamname, int runs, int ball, bool wicket = false) {
        BowlerNode* curr = head;
        BowlerNode* prev = nullptr;

        while (curr && curr->name != bowlerName) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == nullptr) {
            BowlerNode* newBowler = new BowlerNode(bowlerName, teamname);
            if (prev == nullptr) {
                head = newBowler;
            } else {
                prev->next = newBowler;
            }
            curr = newBowler;
        }

        curr->runsConceded += runs;
        curr->balls += ball;
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
            cout << "Bowler: " << curr->name << " | Wickets: " << curr->wickets
                 << " | Runs Conceded: " << curr->runsConceded << endl;
        } else {
            cout << "New bowler introduced into the attack" << endl;
        }
    }

    void displayAndSaveBowlerStats(ofstream& file, string teamname) {
        BowlerStack stack;

        BowlerNode* curr = head;
        while (curr) {
            if (curr->teamName == teamname) {
                int ballsBowled = curr->balls;
                stack.push(curr->name, curr->wickets, curr->runsConceded, ballsBowled);
            }
            curr = curr->next;
        }

        stack.sortStack();

        file << "Bowler Performance for team " << teamname << " (Sorted by Wickets):\n";
        while (!stack.isEmpty()) {
            BowlerStackNode* node = stack.pop();
            file << node->name << ": " << node->wickets << " wickets, " << node->runsConceded
                 << " runs, " << node->ballsBowled << " balls\n";
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
    vector<string> bowlers; // List of bowlers in the team
    int size;
    string* playerNames;      // Array to store player names

    Team(string n, int playerCount) : name(n), totalRuns(0), totalWickets(0), size(playerCount) {
        playerNames = new string[playerCount];
        // Input player names
        for (int i = 0; i < playerCount; i++) {
            cout << "Enter name of player " << (i + 1) << ": ";
            cin >> playerNames[i];
            battingOrder.enqueue(Player(playerNames[i]));
            bowlers.push_back(playerNames[i]); // Assume all players can bowl
            size=playerCount;
        }
    }


    void updateScore(Player& batsman, int runs, int balls, bool isFour, bool isSix) { // update score function to update scores for every batsman.
        batsman.runs += runs;
        batsmanScores[batsman.name] += runs;
        batsman.balls += balls;
        if (isFour) batsman.fours++;
        if (isSix) batsman.sixes++;
        batsman.strikeRate = (batsman.runs / (float)batsman.balls) * 100;
        totalRuns += runs;
    }

    void nextBatsman(Player& currentBatsman) {
        currentBatsman = battingOrder.dequeue(); // using enqueue to remove batsman
        totalWickets++; // increment total wickets
    }

    bool allOut() {
        return totalWickets == size - 1;
    }

    void displayAndSaveBatsmanScores(ofstream& file) { // storing and displaying batter scores through file handling. 
        int size = batsmanScores.size();
        int* scores = new int[size];
        string* names = new string[size];

        int index = 0;
        for (auto& [name, score] : batsmanScores) {
            scores[index] = score;
            names[index++] = name;
        }

        mergeSort(scores, names, 0, size - 1); // merge sorting to sort batsman scores.

        for (int i = 0; i < size; i++) {
            file << names[i] << ": " << scores[i] << " runs\n";
        }

        delete[] scores;
        delete[] names;
    }

};

class Match { // Match class for match simualtion 
public:
    Team team1;
    Team team2;
    int overs;
    int target;
    BowlerPerformance bowlerPerformance;

    // Separate adjacency matrices for partnerships
    int** adjMatrixTeam1; 
    int** adjMatrixTeam2;

    // Player names mapped by indices
    string* playerNamesTeam1;
    string* playerNamesTeam2;

    Match(Team t1, Team t2, int o) : team1(t1), team2(t2), overs(o), target(0) {
        adjMatrixTeam1 = new int*[playerCount];
        adjMatrixTeam2 = new int*[playerCount];
        playerNamesTeam1 = new string[playerCount];
        playerNamesTeam2 = new string[playerCount];

        for (int i = 0; i < playerCount; i++) {
            adjMatrixTeam1[i] = new int[playerCount]();
            adjMatrixTeam2[i] = new int[playerCount]();
        }

        // Initialize player names
        for (int i = 0; i < playerCount; i++) {
            playerNamesTeam1[i] = t1.bowlers[i]; // Assuming bowlers array holds player names
            playerNamesTeam2[i] = t2.bowlers[i];
        }
    }

    ~Match() {
        for (int i = 0; i < playerCount; i++) {
            delete[] adjMatrixTeam1[i];
            delete[] adjMatrixTeam2[i]; // to avoid memory leaks
        }
        delete[] adjMatrixTeam1;
        delete[] adjMatrixTeam2;
        delete[] playerNamesTeam1;
        delete[] playerNamesTeam2;
    }

    void updatePartnership(int** adjMatrix, int batsman1Index, int batsman2Index, int runs) {
        if (batsman1Index < playerCount && batsman2Index < playerCount) {
            adjMatrix[batsman1Index][batsman2Index] += runs;
            adjMatrix[batsman2Index][batsman1Index] += runs; // Symmetric
        }
    }

    void displayPartnerships(int** adjMatrix, const string& teamName, string* playerNames) {
        cout << "Partnerships for " << teamName << ":\n";
        for (int i = 0; i < playerCount; i++) {
            for (int j = i + 1; j < playerCount; j++) { // Avoid duplicate pairs
                if (adjMatrix[i][j] > 0) {
                    cout << playerNames[i] << " & " << playerNames[j]
                         << ": " << adjMatrix[i][j] << " runs\n";
                }
            }
        }
    }

    void playInnings(Team& battingTeam, Team& bowlingTeam, int** adjMatrix, string* playerNames) { // play innings function to simulate an innings.
        int ballsPlayed = 0;
        int currentOver = 0; 
        Player striker = battingTeam.battingOrder.dequeue();
        Player nonStriker = battingTeam.battingOrder.dequeue();
        int strikerIndex = 0;
        int nonStrikerIndex = 1;
        int currentPartnershipRuns = 0;
        string bowlerName;

        while (ballsPlayed < overs * 6 && battingTeam.totalWickets < battingTeam.size - 1) {
            if (ballsPlayed % 6 == 0) {
                cout << "Enter bowler for the next over: ";
                cin >> bowlerName;
            }
            
			bowlerPerformance.displayBowlerStats(bowlerName);
			
            string input;
            cout << "Enter ball outcome (runs: 0-6, w for wicket, nb for no-ball, wd for wide): ";
            cin >> input;

            if (input == "w") {
            	bowlerPerformance.addOrUpdateBowler(bowlerName,bowlingTeam.name,0,1,true);
                updatePartnership(adjMatrix, strikerIndex, nonStrikerIndex, currentPartnershipRuns);
                currentPartnershipRuns = 0;

                battingTeam.totalWickets++;
                if (battingTeam.totalWickets < battingTeam.size - 1) {
                    if (!battingTeam.battingOrder.empty()) {
                        striker = battingTeam.battingOrder.dequeue();
                        strikerIndex = battingTeam.totalWickets + 1; // Update striker index
                    } else {
                        cout << "All batsmen are out. Ending innings.\n";
                        break; // All out
                    }
                } else {
                    cout << "All batsmen are out. Ending innings.\n";
                    break; // All out
                }
                ballsPlayed++;
            } else if (input == "nb") {
                int extras;
                cout << "Any runs scored on no ball: ";
                cin >> extras;
                bowlerPerformance.addOrUpdateBowler(bowlerName,bowlingTeam.name,1+extras,0,false);
                battingTeam.updateScore(striker, extras, 1, extras == 4, extras == 6);
                currentPartnershipRuns += 1 + extras;
            } else if (input == "wd") {
                battingTeam.totalRuns++;
                currentPartnershipRuns++;
                bowlerPerformance.addOrUpdateBowler(bowlerName,bowlingTeam.name,1,0,false);
            } else {
                int runs = stoi(input);
                if(runs>6 || runs<0){
                	cout<<"Enter valid runs: "<<endl;
                	cin>>input;
                	runs=stoi(input);
				}
                currentPartnershipRuns += runs;
				bowlerPerformance.addOrUpdateBowler(bowlerName,bowlingTeam.name, runs,1, false);
                battingTeam.updateScore(striker, runs, 1, runs == 4, runs == 6);
                cout << "Batsman: " << striker.name
                     << " | Runs: " << striker.runs
                     << " | Balls: " << striker.balls
                     << " | Strike Rate: " << fixed << setprecision(2) << striker.strikeRate << endl;
                if (runs % 2 != 0) {
                    swap(striker, nonStriker); // swapping striker and non striker after every odd runs or over.
                    swap(strikerIndex, nonStrikerIndex);
                }
                ballsPlayed++;
            }

            currentOver = ballsPlayed / 6;
            cout << "Current Over: " << currentOver << "." << ballsPlayed % 6
                 << " | Current Score: " << battingTeam.totalRuns << "/" << battingTeam.totalWickets << endl;

            if (ballsPlayed % 6 == 0) {
                swap(striker, nonStriker);
                swap(strikerIndex, nonStrikerIndex);
                cout << "End of Over! Striker and Non-Striker swapped." << endl;
            }

            if (target > 0 && battingTeam.totalRuns >= target) {
                cout << battingTeam.name << " has chased the target successfully!" << endl;
                break;
            }
        }

        if (!battingTeam.allOut()) {
            updatePartnership(adjMatrix, strikerIndex, nonStrikerIndex, currentPartnershipRuns);
        }

        cout << "Innings Over: " << battingTeam.totalRuns << "/" << battingTeam.totalWickets << endl;
    }

void startMatch() {
    string tossChoice;
    cout << "Enter your choice for the toss (heads/tails): "; // start match function for user input displays
    cin >> tossChoice;

    srand(time(0));
    string tossResult = (rand() % 2 == 0) ? "heads" : "tails";
    cout << "Toss Result: " << tossResult << endl;

    Team* tossWinner = (tossChoice == tossResult) ? &team1 : &team2;
    Team* tossLoser = (tossWinner == &team1) ? &team2 : &team1;

    string choice;
    cout << tossWinner->name << ", choose to bat or bowl (bat/bowl): ";
    cin >> choice;

    if (choice == "bat") {
        // Toss winner bats first
        playInnings(*tossWinner, *tossLoser, 
                    (tossWinner == &team1 ? adjMatrixTeam1 : adjMatrixTeam2), 
                    (tossWinner == &team1 ? playerNamesTeam1 : playerNamesTeam2));
        target = tossWinner->totalRuns + 1;
        cout << "Target for " << tossLoser->name << ": " << target << "\n";
        // Toss loser bats second
        playInnings(*tossLoser, *tossWinner, 
                    (tossLoser == &team1 ? adjMatrixTeam1 : adjMatrixTeam2), 
                    (tossLoser == &team1 ? playerNamesTeam1 : playerNamesTeam2));
    } else {
        // Toss loser bats first
        playInnings(*tossLoser, *tossWinner, 
                    (tossLoser == &team1 ? adjMatrixTeam1 : adjMatrixTeam2), 
                    (tossLoser == &team1 ? playerNamesTeam1 : playerNamesTeam2));
        target = tossLoser->totalRuns + 1;
        cout << "Target for " << tossWinner->name << ": " << target << "\n";
        // Toss winner bats second
        playInnings(*tossWinner, *tossLoser, 
                    (tossWinner == &team1 ? adjMatrixTeam1 : adjMatrixTeam2), 
                    (tossWinner == &team1 ? playerNamesTeam1 : playerNamesTeam2));
    }

    declareWinner();

    // Display partnerships correctly
    displayPartnerships(adjMatrixTeam1, team1.name, playerNamesTeam1);
    displayPartnerships(adjMatrixTeam2, team2.name, playerNamesTeam2);

    saveMatchHistory();
}



    void declareWinner() { // functions to declare winner according to cricket rules
        if (team2.totalRuns >= target) { 
            cout << team2.name << " Wins by " << team1.size - team2.totalWickets - 1 << " wickets!" << endl;
        } else if (team1.totalRuns >= target) {
            cout << team1.name << " Wins by " << team1.size - team1.totalWickets - 1 << " wickets!" << endl;
        } else if (team2.totalRuns == team1.totalRuns) {
            cout << "Match tied! No super over to follow" << endl;
        } else if (team1.totalRuns < target - 1 && team2.totalRuns == target - 1) {
            cout << team2.name << " Wins by " << target - 1 - team1.totalRuns << " runs!" << endl;
        } else if (team2.totalRuns < target - 1 && team1.totalRuns == target - 1) {
            cout << team1.name << " Wins by " << target - 1 - team2.totalRuns << " runs!" << endl;
        }
    }

    void saveMatchHistory() { // saving match history in match summary file through file handling.
        ofstream file("match_history.txt", ios::app);
        if (file.is_open()) {
            file << "Match Summary:\n";
            file << "Team 1: " << team1.name << " | Runs: " << team1.totalRuns << " | Wickets: " << team1.totalWickets << "\n";
            file << "Team 2: " << team2.name << " | Runs: " << team2.totalRuns << " | Wickets: " << team2.totalWickets << "\n";
            if (team2.totalRuns >= target) {
                file << team2.name << " Wins by " << team1.size - team2.totalWickets - 1 << " wickets!" << endl;
            } else if (team1.totalRuns >= target) {
                file << team1.name << " Wins by " << team1.size - team1.totalWickets - 1 << " wickets!" << endl;
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
            bowlerPerformance.displayAndSaveBowlerStats(file, team1.name);
            bowlerPerformance.displayAndSaveBowlerStats(file, team2.name);
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
        cout << "\nCricket Scoring System\n"; // Main Menu 
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

//                int playerCount;
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
