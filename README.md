Project Title: STUMPS - Cricket Scoring Simulation

Project By: MOHAMMAD MUSA ALI, MUHAMMAD UMER, SAROSH ISHAQ

Our project is a Cricket Scoring System designed to simulate and manage the scoring of cricket matches efficiently using advanced data structures such as queues, stacks, and linked lists. It provides an interactive platform for scoring, tracking player statistics, and determining match outcomes (win probability), making it a valuable tool for cricket scorers to organize matches and get detailed statistics for the game.

1. Team Creation:
Users can create two teams by entering the team names and the number of players in each team.
Player names are entered for both the batting and bowling lineup, and each player is assumed to be able to bowl.

2. Batting Order:
A queue is used to manage the batting order. Players are dequeued when it's their turn to bat, and when a batsman is out, the next player is automatically queued to replace them.

3. Bowling Performance Tracking:
Bowling statistics are tracked using a linked list structure where each bowler's name, team name, wickets, runs conceded, and balls bowled are stored.
Users can update bowler stats with details such as runs conceded, balls bowled, and wickets taken.
Bowler performance is displayed and saved to a file after sorting based on wickets.

4. Match Simulation:
The match is played with input prompts for runs scored on each ball, no-balls, wide balls, and wickets.
The match consists of overs, and the user inputs details of each ball bowled during the innings.
Batsmen’s scores, balls faced, strike rates, fours, and sixes are updated dynamically during the match.
Partnerships between batsmen are tracked in an adjacency matrix, recording runs between pairs of batsmen.

5. Innings Management:
The match consists of two innings, where one team bats first and the other bowls. The teams alternate based on the outcome of the toss.
A player is declared out if they score a wicket, and a new batsman is dequeued to replace them.
The system handles situations like wides, no-balls, and wickets.

6. Player Statistics:
Batting statistics like total runs, balls faced, strike rate, fours, and sixes are tracked for each player.
The batting order is maintained in a queue, and a player’s score can be updated after each ball.
Bowler statistics are also managed, including the number of wickets taken, runs conceded, and balls bowled.

7. Partnership Tracking:
An adjacency matrix is used to track partnerships between batsmen, recording the total runs scored between any two batsmen.
Partnerships are displayed after the match, showing the runs between each pair of batsmen.

8. Match Outcome and Result Declaration:
The match’s outcome is determined by the target set by the first batting team. The second team must score more than the target to win.
The match can end in a tie, or the winner is declared by runs or wickets.
The system tracks the winner and the margin of victory.

9. Match History:
After each match, the match details (including team scores, batsman scores, bowler stats, and partnerships) are saved in a file named match_history.txt.
This file logs all the matches played, storing scores in descending order for batsmen and bowler performance statistics.

10. Interactive Interface:
The system provides a menu for the user to start a new match or exit the program.
The user is prompted to input player names, team names, and match details like overs and toss results interactively.

11. Extras and Validations:
There are validations in place for runs scored (only valid runs between 0 to 6 are allowed).
Additional checks for valid no-balls and wides ensure the match is played accurately.

12. Data Sorting:
The batsman scores are sorted in descending order using the merge sort algorithm before displaying and saving them.
Bowler performance is also sorted based on wickets taken, ensuring that the best-performing bowlers are highlighted.

13. File I/O:
Match history, including detailed scores and statistics, is saved to a file after each match, allowing for later review.
