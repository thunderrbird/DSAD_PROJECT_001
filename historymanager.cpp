// BrowserHistoryManager
// DSAD PROJECT 2
//224CS2026(Raushan Raj)(Mtech_IstSEM)


#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include<limits>
using namespace std;

class Page {
public:
    string url;
    bool bookmarked;
    Page* next;
    Page* prev;

    Page(string url) {
        this->url = url;
        this->bookmarked = false;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

class History {
private:
    Page* head;
    Page* current;
    // Used Map for storing the bookmarked pages
    unordered_map<string, Page*> bookmarks;
    stack<Page*> undo;
    stack<Page*> redo;

public:
    History() {
        this->head = nullptr;
        this->current = nullptr;
    }
    // Functionality to visit a new page
    void visit_page(const string& url) {
        if (url.empty()) {
            cout << "URL cannot be empty." << endl;
            return;
        }

        Page* new_page = new Page(url);
        if (this->head == nullptr) {
            this->head = new_page;
            this->current = new_page;
        } else {
            // Clear forward history and redo stack
            Page* temp = this->current->next;
            while (temp) {
                Page* to_delete = temp;
                temp = temp->next;
                delete to_delete;
            }
            this->current->next = new_page;
            new_page->prev = this->current;
            this->current = new_page;
            while (!redo.empty())
             {redo.pop();
             }
        }
        cout << "Visited: " << url << endl;
    }
  // Functionality to go back to previous page
    void go_back() {
        if (this->current && this->current->prev) {
            redo.push(this->current);
            this->current = this->current->prev;
            cout << "Went back to: " << this->current->url << endl;
        } else {
            cout << "No previous page in history." << endl;
        }
    }
   // Functionality to go forward on next page
    void go_forward() {
        if (this->current && this->current->next) {
            undo.push(this->current);
            this->current = this->current->next;
            cout << "Went forward to: " << this->current->url << endl;
        } else {
            cout << "No forward page in history." << endl;
        }
    }
   // Functioonality to add bookmarks
    void bookmark_page() {
        if (this->current) {
            if (this->bookmarks.find(this->current->url) != this->bookmarks.end()) {
                cout << "Page already bookmarked." << endl;
            } else {
                this->current->bookmarked = true;
                bookmarks[this->current->url] = this->current;
                cout << "Bookmarked: " << this->current->url << endl;
            }
        } else {
            cout << "No page to bookmark." << endl;
        }
    }
   // Functionality to view bookmarks
    void view_bookmarks() {
        if (bookmarks.empty()) {
            cout << "No bookmarks available." << endl;
            return;
        }
        cout << "Bookmarks:" << endl;
        // Added logic to see the bookmarks from the map data structure
        for (const auto& pair : bookmarks) {
            cout << pair.first << endl;
        }
    }

    void go_to_bookmark(const string& url) {
        if (bookmarks.find(url) != bookmarks.end()) {
            this->current = bookmarks[url];
            cout << "Jumped to bookmark: " << this->current->url << endl;
        } else {
            cout << "Bookmark not found." << endl;
        }
    }
    // Functionality to show the search_history
    void search_history(const string& term) {
        if (term.empty()) {
            cout << "Search term cannot be empty." << endl;
            return;
        }

        Page* temp = this->head;
        bool found = false;
        while (temp) {
            if (temp->url.find(term) != string::npos) {
                cout << "Found: " << temp->url << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "No matching history found for: " << term << endl;
        }
    }
   // Undo a functionality previously visited
   //Reverses the last action you performed
    void unDo() {
        if (!undo.empty()) {
            redo.push(this->current);
            this->current = undo.top();
            undo.pop();
            cout << "Undid to: " << this->current->url << endl;
        } else {
            cout << "Nothing to undo." << endl;
        }
    }
    // Reverses the undo action.
    void reDo() {
        if (!redo.empty()) {
            undo.push(this->current);
            this->current = redo.top();
            redo.pop();
            cout << "Redid to: " << this->current->url << endl;
        } else {
            cout << "Nothing to redo." << endl;
        }
    }
  // TO view the history of the previously visited pages
    void view_history() {
        Page* temp = this->head;
        if (!temp) {
            cout << "No history available." << endl;
            return;
        }
        
        cout << "Browser History:" << endl;
        while (temp) {
            if (temp == this->current) {
                cout << " " << temp->url << " ((Currently visiitng this page))" << endl;
            } else {
                cout << temp->url << endl;
            }
            temp = temp->next;
        }
    }
  // Pop the items from both the stack to clear the history
    void clear_history() {
        if (!this->head) {
            cout << "History is already empty." << endl;
            return;
        }

        while (this->head) {
            Page* to_delete = this->head;
            this->head = this->head->next;
            delete to_delete;
        }
        this->current = nullptr;
        bookmarks.clear();
        while (!undo.empty()) undo.pop();
        while (!redo.empty()) redo.pop();
        cout << "History cleared." << endl;
    }
};

void display_menu() {
    cout << "\nOptions:" << endl;
    cout << "1. Visit a new page" << endl;
    cout << "2. Go Back to previous" << endl;
    cout << "3. Go Forward" << endl;
    cout << "4. View History" << endl;
    cout << "5. Clear History" << endl;
    cout << "6. Bookmark Current Page" << endl;
    cout << "7. View Bookmarks" << endl;
    cout << "8. Go to Bookmark" << endl;
    cout << "9. Search History" << endl;
    cout << "10. Undo" << endl;
    cout << "11. Redo" << endl;
    cout << "12. Exit" << endl;
}

int main() {
    History history;
    while (true) {
        display_menu();
        int choice;
        cout << "Enter your choice ";
        cin >> choice;
     // Adding Logic to handle invalid input ithe choices
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number between 1 and 12." << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "Enter URL to visit: ";
                string url;
                cin >> url;
                history.visit_page(url);
                break;
            }
            case 2:
                history.go_back();
                break;
            case 3:
                history.go_forward();
                break;
            case 4:
                history.view_history();
                break;
            case 5:
                history.clear_history();
                break;
            case 6:
                history.bookmark_page();
                break;
            case 7:
                history.view_bookmarks();
                break;
            case 8: {
                cout << "Enter URL of bookmark to visit: ";
                string url;
                cin >> url;
                history.go_to_bookmark(url);
                break;
            }
            case 9: {
                cout << "Enter search term: ";
                string term;
                cin >> term;
                history.search_history(term);
                break;
            }
            case 10:
                history.unDo();
                break;
            case 11:
                history.reDo();
                break;
            case 12:
                cout << "Exiting Browser History Manager." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
}
