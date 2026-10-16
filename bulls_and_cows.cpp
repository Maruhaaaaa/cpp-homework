// A little guessing game called
//    (for some obscure reason) <<Bulls and Cows>>.

#include "std_lib_facilities.h"
#include <ctime>

int count (const vector<char>& digits, char d)
{
  int k{};

  for (int i = 0; i < digits.size(); ++i)
    if (digits[i] == d)
      ++k;

  return k;
}

void validate (const vector<char>& number)
{
  for (int i = 0; i < number.size(); ++i)
  {
    if (number[i] < '0' || '9' < number[i])
      error("the number contains not a digit");

    if (count(number, number[i]) != 1)
      error("digits of the number are not unique");
  }
}

vector<char> generate_number(){
  vector<char> number;

  while(number.size() < 4){
    char d = '0' + (rand() % 10);
    if (count(number, d) == 0){
      number.push_back(d);
    }
  }
  return number;
}

struct score {
  int bulls;
  int cows;
};

score get_score(const vector<char>& secret, const vector<char>& guess){
  score s{0, 0};
  for (int i = 0; i < guess.size(); i++){
    if (guess[i] == secret[i])
      ++s.bulls;
    else if (count(secret, guess[i]) == 1)
      ++s.cows;
  }
  return s;
}

vector<vector<char>> all_combinations(){
  vector<vector<char>> digits;
  for (char a = '0'; a <= '9'; ++a){
    for (char b = '0'; b <= '9'; ++b){
      if (b == a) continue;
      for (char c = '0'; c <= '9'; ++c){
        if (c == a || c == b) continue;
        for (char d = '0'; d <= '9'; ++d){
          if (d == a || d == b || d == c) continue;
          digits.push_back({a, b, c, d});
        }
      }
    }
  }
  return digits;
}

vector<char> user_guess ()
{
  string raw_input;
  cout << "guess the number: ";
  cin >> raw_input;

  if (!cin)
    error("invalid input");

  if (raw_input.length() != 4){
    error("input must be exactly 4 digits");
  }

  vector<char> number(4);
  for (int i = 0; i < number.size(); ++i)
    number[i] = raw_input[i];

  validate(number);

  return number;
}


void pc_player(){
  vector<char> comp_secret = generate_number();
  /*cout << "for test: ";
  for (int i = 0; i < comp_secret.size(); ++i) {
      cout << comp_secret[i];
  }*/

  cout << "\nEnter your 4-digit number for the computer\n";
  vector<char> user_secret = user_guess();

  vector<vector<char>> digits = all_combinations();

  bool game_over = false;
  while(!game_over){
    cout << "\nYour turn to guess";
    vector<char> us_guess = user_guess();

    score user_result = get_score(comp_secret, us_guess);
    cout << user_result.bulls << " bull(s) and " << user_result.cows << " cow(s)\n";

    if (user_result.bulls == 4){
      cout << "\nYou win!\n";
      break;
    }

    cout << "Computer's turn\n";
    vector<char> comp_guess = digits[0];
    cout << "Computer guesses: ";
    for (int i = 0; i < comp_guess.size(); ++i) cout << comp_guess[i];

    score comp_result = get_score(user_secret, comp_guess);
    cout << "\n" << comp_result.bulls << " bull(s) and " << comp_result.cows << " cow(s)\n";

    if (comp_result.bulls == 4){
      cout << "\nComputer win!\n";
      break;
    }

    vector<vector<char>> new_digits;
    for (int i = 0; i < digits.size(); ++i){
      score result = get_score(digits[i], comp_guess);
      if (result.bulls == comp_result.bulls && result.cows == comp_result.cows){
        new_digits.push_back(digits[i]);
      }
    }
    digits = new_digits;
  }

}


int main ()
try
{
  srand(time(0));

  cout << "<<Bulls and Cows>>\n"
       << "Computer sets a number of 4 unique digits.\n"
       << "Try to guess it.\n"
       << "<Bull> means right digit in the right position.\n"
       << "<Cow> means right digit in the wrong position.\n"
       << "\n"
       << "game is on" << endl;

  pc_player();

  return 0;
}
catch (exception& e)
{
  cerr << e.what() << endl;
  return 1;
}
catch (...)
{
  cerr << "Oops, something went wrong..." << endl;
  return 2;
}
