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

class Team{
public:
    string name;
    int totalRuns;
    int totalWickets;
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
  return 0;
}
