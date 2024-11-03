// MusicPlaylistManager
// DSAD PROJECT 1
//224CS2026(Raushan Raj)(Mtech_IstSEM)
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include<limits>
using namespace std;


// Created a class duration for taking input from user in hr ,min ,sec
class Duration {
public:
    int hours;
    int minutes;
    int seconds;

     Duration(int h = 0, int m = 0, int s = 0) {
        hours = h;
        minutes = m;
        seconds = s;
    }

    int toSeconds() const {
        return hours * 3600 + minutes * 60 + seconds;
    }

    void fromSeconds(int totalSeconds) {
        hours = totalSeconds / 3600;
        totalSeconds %= 3600;
        minutes = totalSeconds / 60;
        seconds = totalSeconds % 60;
    }

    void display() const {
        cout << hours << "h " << minutes << "m " << seconds << "s";
    }
};

class Song {
public:
    string title;
    string artist;
    Duration duration;  // Using the Duration class
    Song* next;

    Song(string title, string artist, Duration duration) {
        this->title = title;
        this->artist = artist;
        this->duration = duration;
        this->next = nullptr;
    }
};

class Playlist {
public:
    Playlist() {
        head = nullptr;
    }

    ~Playlist() {
        while (head != nullptr) {
            Song* temp = head;
            head = head->next;
            delete temp;
        }
    }
   // Functionality for addding song at the last
    void addSong(string title, string artist, Duration duration) {
        Song* newSong = new Song(title, artist, duration);
        if (head == nullptr) {
            head = newSong;
        } else {
            Song* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newSong;
        }
        cout << "Added song: " << title << " by " << artist << " (";
        duration.display();
        cout << ")" << endl;
    }
   // Functionality for adding song to a particular position
    void addSongAtPosition(string title, string artist, Duration duration, int position) {
        Song* newSong = new Song(title, artist, duration);
        if (position == 1) {
            newSong->next = head;
            head = newSong;
        } else {
            Song* current = head;
            for (int i = 1; i < position - 1 && current != nullptr; i++) {
                current = current->next;
            }
            if (current == nullptr) {
                cout << "Position Not Found. Adding to the end." << endl;
                addSong(title, artist, duration);
                return;
            }
            newSong->next = current->next;
            current->next = newSong;
        }
        cout << "Added song at position " << position << ": " << title << " by " << artist << " (";
        duration.display();
        cout << ")" << endl;
    }

    // Functionality for removing song from a given index

    void removeSong(int index) {
        if (head == nullptr) {
            cout << "The playlist is empty." << endl;
            return;
        }
        if (index == 1) {
            Song* temp = head;
            head = head->next;
            cout << "Removed song: " << temp->title << " by " << temp->artist << endl;
            delete temp;
        } else {
            Song* current = head;
            Song* previous = nullptr;
            int i = 1;
            while (i < index && current != nullptr) {
                previous = current;
                current = current->next;
                i++;
            }
            if (current == nullptr) {
                cout << "Invalid song index." << endl;
                return;
            }
            previous->next = current->next;
            cout << "Removed song: " << current->title << " by " << current->artist << endl;
            delete current;
        }
    }
  // Functionality for displaying the playlist
    void displayPlaylist() {
        if (head == nullptr) {
            cout << "The playlist is empty." << endl;
            return;
        }
        Song* current = head;
        int position = 1;
        cout << "Playlist:" << endl;
        while (current != nullptr) {
            cout << position << ". " << current->title << " by " << current->artist << " (";
            current->duration.display();
            cout << ")" << endl;
            current = current->next;
            position++;
        }
    }

 // Functionality for search song by title , artist
    void searchSong(string keyword) {
        Song* current = head;
        int position = 1;
        bool found = false;

        while (current != nullptr) {
            if (current->title == keyword || current->artist == keyword) {
                cout << "Found song at position " << position << ": " << current->title << " by " << current->artist << endl;
                found = true;
            }
            current = current->next;
            position++;
        }

        if (!found) {
            cout << "No song found with the title or artist: " << keyword << endl;
        }
    }

// To reorder the song , calculated the length and checked the bounds  
    void reorderSong(int fromIndex, int toIndex) {
        if (fromIndex == toIndex) return;

    // Calculate the length of the playlist
    int length = 0;
    Song* current = head;
    while (current != nullptr) {
        length++;
        current = current->next;
    }

    // Validate indices (1-based)
    if (fromIndex < 1 || fromIndex > length || toIndex < 1 || toIndex > length) {
        cout << "Invalid index. Please enter indices between 1 and " << length << "." << endl;
        return; // Exit function if indices are out of bounds
    }

        Song* prevFrom = nullptr;
        Song* from = head;
        int i = 1;

        while (from != nullptr && i < fromIndex) {
            prevFrom = from;
            from = from->next;
            i++;
        }

        if (from == nullptr) {
            cout << "Invalid fromIndex." << endl;
            return;
        }

        if (prevFrom != nullptr) {
            prevFrom->next = from->next;
        } else {
            head = from->next;
        }

        if (toIndex == 1) {
            from->next = head;
            head = from;
        } else {
            Song* prevTo = nullptr;
            Song* to = head;
            int j = 1;

            while (to != nullptr && j < toIndex) {
                prevTo = to;
                to = to->next;
                j++;
            }

            from->next = to;
            prevTo->next = from;
        }
        cout << "Moved song from position " << fromIndex << " to position " << toIndex << endl;
    }


// Functionality to save the playlist to a file in same folder
    void savePlaylist(string filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Failed to open file for saving." << endl;
            return;
        }

        Song* current = head;
        while (current != nullptr) {
            file << current->title << "," << current->artist << "," << current->duration.hours << "," << current->duration.minutes << "," << current->duration.seconds << endl;
            current = current->next;
        }
        file.close();
        cout << "Playlist saved to " << filename << endl;
    }
// Functionality to loadPlaylist from a saved file
    void loadPlaylist(string filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Failed to open file for loading." << endl;
            return;
        }

        while (head != nullptr) {
            Song* temp = head;
            head = head->next;
            delete temp;
        }

        string line, title, artist;
        int h, m, s;

        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, title, ',');
            getline(ss, artist, ',');
            ss >> h; ss.ignore();
            ss >> m; ss.ignore();
            ss >> s;
            addSong(title, artist, Duration(h, m, s));
        }
        file.close();
        cout << "Playlist loaded from " << filename << endl;
    }
//  Functionality to Sort the playlist By tilte or artist either in ascending or descending order  
void sortPlaylist(bool sortByTitle, bool ascending) {
     if (head == nullptr) {  // Check if playlist is empty
        cout << "Playlist is empty, nothing to sort." << endl;
        return;
    }
    head = mergeSort(head, sortByTitle, ascending);
    

    if (sortByTitle) {
        if (ascending) {
            cout << "Playlist sorted by title in ascending order." << endl;
        } else {
            cout << "Playlist sorted by title in descending order." << endl;
        }
    } else {
        if (ascending) {
            cout << "Playlist sorted by artist in ascending order." << endl;
        } else {
            cout << "Playlist sorted by artist in descending order." << endl;
        }
    }
}

// Functionality for shuffling the playlist in random order 
    void shufflePlaylist() {
        if (head == nullptr || head->next == nullptr) {
            return;  // No need to shuffle if playlist is empty or has only one song
        }

        srand(time(0));  // Seed for randomness
        Song* shuffledHead = nullptr;

        // Count the number of nodes in the list
        int count = 0;
        for (Song* temp = head; temp != nullptr; temp = temp->next) {
            count++;
        }

        // Repeat until the original list is empty
        while (count > 0) {
            // Generate a random index to pick a song
            int randIndex = rand() % count;
            
            Song* prev = nullptr;
            Song* node = head;
            for (int i = 0; i < randIndex; ++i) {
                prev = node;
                node = node->next;
            }

            // Remove the selected node from the original list
            if (prev == nullptr) {
                head = node->next;  
            } else {
                prev->next = node->next;
            }

            // Add the selected node to the beginning of the shuffled list
            node->next = shuffledHead;
            shuffledHead = node;

            count--;  // Decrease count as one node is removed from the original list
        }

        head = shuffledHead;  // Update head to the shuffled list
        cout << "Playlist shuffled." << endl;
    }

private:

   Song*head;
   Song* mergeSort(Song* head, bool sortByTitle, bool ascending) {
        if (!head || !head->next) return head;

        Song* middle = split(head);
        Song* left = mergeSort(head, sortByTitle, ascending);
        Song* right = mergeSort(middle, sortByTitle, ascending);
        return merge(left, right, sortByTitle, ascending);
    }

Song* split(Song* head) {
    Song* slow = head;
    Song* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Song* secondHalf = slow->next;
    slow->next = nullptr;
    return secondHalf;
}

Song* merge(Song* left, Song* right, bool sortByTitle, bool ascending) {
     if (!left) return right;
    if (!right) return left;

    bool condition;
    if (sortByTitle) {
        if (ascending) {
            condition = (left->title < right->title);
        } else {
            condition = (left->title > right->title);
        }
    } else {
        if (ascending) {
            condition = (left->artist < right->artist);
        } else {
            condition = (left->artist > right->artist);
        }
    }

    if (condition) {
        left->next = merge(left->next, right, sortByTitle, ascending);
        return left;
    } else {
        right->next = merge(left, right->next, sortByTitle, ascending);
        return right;
    }
}



};

int main() {
    Playlist playlist;
    int choice, index, fromIndex, toIndex;
    string title, artist, filename;
    int hours, minutes, seconds;

    do {
        cout << "\nMenu:\n";
        cout << "1. Add Song\n";
        cout << "2. Add Song at Position\n";
        cout << "3. Remove Song\n";
        cout << "4. Display Playlist\n";
        cout << "5. Search Song\n";
        cout << "6. Reorder Song\n";
        cout << "7. Save Playlist\n";
        cout << "8. Load Playlist\n";
        cout << "9. Sort Playlist\n";
        cout << "10. Shuffle Playlist\n";
        cout << "11. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;


      // Input range check from 1 to 11
         if (cin.fail()) {
        cout << "Invalid input. Please enter a number between 1 and 11." << endl;
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continue; 
    }
        switch (choice) {
            case 1:
                cout << "Enter title: ";
                cin >> ws;  
                getline(cin, title);
                cout << "Enter artist: ";
                getline(cin, artist);
                cout << "Enter duration in hrs: ";
                cin >> hours ;
                if (cin.fail()) {
                cout << "Invalid input for hours. Please enter an integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue; 
                }

                cout << "Enter duration in minutes: ";
                cin >> minutes;
                if (cin.fail() || minutes < 0 || minutes >= 60) {
                cout << "Invalid input for minutes. Please enter an integer between 0 and 59." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue; 
            }

            cout << "Enter duration in seconds: ";
            cin >> seconds;
            if (cin.fail() || seconds < 0 || seconds >= 60) {
            cout << "Invalid input for seconds. Please enter an integer between 0 and 59." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; 
            }
                playlist.addSong(title, artist, Duration(hours, minutes, seconds));
                break;
            case 2:
                cout << "Enter position to add the song: ";
                cin >> index;
                cout << "Enter title: ";
                cin >> ws;  
                getline(cin, title);
                cout << "Enter artist: ";
                getline(cin, artist);
                cout << "Enter duration (hours, minutes, seconds): ";
                cin >> hours >> minutes >> seconds;
                playlist.addSongAtPosition(title, artist, Duration(hours, minutes, seconds), index);
                break;
            case 3:
                cout << "Enter index of song to remove: ";
                cin >> index;
                playlist.removeSong(index);
                break;
            case 4:
                playlist.displayPlaylist();
                break;
            case 5:
                cout << "Enter title or artist to search: ";
                cin >> ws;  
                getline(cin, title);
                playlist.searchSong(title);
                break;
            case 6:
                cout << "Enter from index: ";
                cin >> fromIndex;
                cout << "Enter to index: ";
                cin >> toIndex;
                playlist.reorderSong(fromIndex, toIndex);
                break;
            case 7:
                cout << "Enter filename to save: ";
                cin >> ws;  // To ignore leading whitespace
                getline(cin, filename);
                playlist.savePlaylist(filename);
                break;
            case 8:
                cout << "Enter filename to load: ";
                cin >> ws; 
                getline(cin, filename);
                playlist.loadPlaylist(filename);
                break;
            case 9:
                int sortOption, orderOption;
bool ascending;


cout << "Enter 1 to sort by title or 2 to sort by artist: ";
cin >> sortOption;

cout << "Enter 1 for ascending order or 2 for descending order: ";
cin >> orderOption;


if (orderOption == 1) {
    ascending = true;
} else if (orderOption == 2) {
    ascending = false;
} else {
    cout << "Invalid choice for sorting order. Returning to main menu." << endl;
    
}

// Call sortPlaylist 
if (sortOption == 1) {
    playlist.sortPlaylist(true, ascending);  // Sort by title
} else if (sortOption == 2) {
    playlist.sortPlaylist(false, ascending); // Sort by artist
} else {
    cout << "Invalid choice for sorting criterion. Returning to main menu." << endl;
}

                break;
            case 10:
                playlist.shufflePlaylist();
                break;
            case 11:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 11);

    return 0;
}
