#include <iostream>
#include <deque>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace chrono;

class TypingSpeedTest
{
public:
   TypingSpeedTest()
{
    test_duration = 60;
    chooseTextOption(); 
    if (!custom_text) {
        chooseDifficultyLevel(); 
        loadDocument("sample_document" + to_string(difficulty_level) + ".txt"); 
        chooseRandomParagraph(); 
    }
    else {
        tokenizeWords(paragraph, passage_words);
    }
}


     void start_test()
    {
        start_time = high_resolution_clock::now();

        cout << "Type the following paragraph: " << endl;
        cout << paragraph << endl;

        string typed_paragraph;
        cout << "Please start typing: ";
        getline(cin, typed_paragraph);

        calculateAccuracy(typed_paragraph);
    }

    void stop_test()
    {
        end_time = high_resolution_clock::now();
    }

    double calculate_typing_speed()
    {
        double time_in_seconds = duration<double>(end_time - start_time).count();
        double typing_speed = (typed_words.size() / 0.82) / (time_in_seconds / 60.0);
        return typing_speed;
    }

    void calculateAccuracy(const string &typed_text)
{
    tokenizeWords(typed_text, typed_words);

    int total_matching_words = 0;
    int total_words_in_paragraph = min(typed_words.size(), passage_words.size());

    vector<string> highlighted_text;

    for (int i = 0; i < total_words_in_paragraph; ++i)
    {
        if (typed_words[i] == passage_words[i])
        {
            total_matching_words++;
            highlighted_text.push_back(typed_words[i]); //add correct words to highlighted text
        }
        else
        {
            //add incorrect words to highlighted with a marker 
            highlighted_text.push_back("*" + typed_words[i] + "*");
        }
    }

    int incorrect_words = total_words_in_paragraph - total_matching_words;
    int missed_words = passage_words.size() > typed_words.size() ? passage_words.size() - typed_words.size() : 0;

    double accuracy = 0.0;
    if (total_words_in_paragraph != 0)
    {
        accuracy = (static_cast<double>(total_matching_words) / total_words_in_paragraph) * 100.0;
    }
    cout << "Your accuracy is " << accuracy << "%" << endl;
    cout << "Number of correct words: " << total_matching_words << endl;
    cout << "Number of incorrect words: " << incorrect_words << endl;
    cout << "Number of missed words: " << missed_words << endl;

    //printhighlighted text
    for (const string& word : highlighted_text)
    {
        cout << word << " ";
    }
    cout << endl;
}


    void main()
    {
        start_test();
        stop_test();

        double typing_speed = calculate_typing_speed();

        cout << "Your typing speed is " << typing_speed << " words per minute." << endl;
    }

private:
    void tokenizeWords(const string &input, deque<string> &words)
    {
        words.clear();
        istringstream iss(input);
        string word;

        while (iss >> word)
        {
            words.push_back(word);
        }
    }

    void loadDocument(const string &filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            document.push_back(line);
        }
        file.close();
    }
    else
    {
        cout << "Failed to open file: " << filename << endl;
    }
}


    void chooseRandomParagraph()
    {
        if (!document.empty())
        {
            int randomIndex = rand() % document.size();
            paragraph = document[randomIndex];
            tokenizeWords(paragraph, passage_words);
        }
        else
        {
            paragraph = "No document loaded.";
        }
    }

    void chooseDifficultyLevel()
    {
        cout << "Choose a difficulty level (1: Easy, 2: Medium, 3: Hard, 4: Expert): ";
        cin >> difficulty_level;
        cin.ignore();  
    }

    void chooseTextOption()
    {
        cout << "Do you want to use your own text? (y/n): ";
        char use_own_text;
        cin >> use_own_text;
        cin.ignore();  

        if (use_own_text == 'y' || use_own_text == 'Y') {
            cout << "Enter your text: ";
            getline(cin, paragraph);
            tokenizeWords(paragraph, passage_words);
            custom_text = true;
        } else {
            custom_text = false;
        }
    }

    deque<string> typed_words; 
    deque<string> passage_words; 
    time_point<high_resolution_clock> start_time;
    time_point<high_resolution_clock> end_time;
    int test_duration;
    vector<string> document;
    string paragraph;
    int difficulty_level;  
    bool custom_text;  
};

int main()
{
    srand(static_cast<unsigned>(time(nullptr))); // Seed the random number generator
    TypingSpeedTest test;
    test.main();
    return 0;
}
