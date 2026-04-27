#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

class light_text_editor {
private:
    enum class Action : uint8_t {
        RemoveLast,
        AddBack
    };

    struct History {
        Action action;
        char ch; // AddBack時のみ有効
    };

    string text_;
    vector<History> history_;

public:
    void add(char c) {
        text_.push_back(c);
        history_.push_back({Action::RemoveLast, '\0'});
    }

    void del() {
        if (!text_.empty()) {
            char removed = text_.back();
            text_.pop_back();
            history_.push_back({Action::AddBack, removed});
        }
    }

    void undo() {
        if (history_.empty()) return;

        History h = history_.back();
        history_.pop_back();

        if (h.action == Action::RemoveLast) {
            if (!text_.empty()) {
                text_.pop_back();
            }
        } else if (h.action == Action::AddBack) {
            text_.push_back(h.ch);
        }
    }

    const string& get_text() const {
        return text_;
    }
};

int main() {
    int N;

    if (!(cin >> N)) {
        return 0;
    }

    light_text_editor editor;

    for (int i = 0; i < N; i++) {
        string command;

        if (!(cin >> command)) {
            break;
        }
        if (command == "ADD") {
            char c;
            if (!(cin >> c)) {
                break;
            }
            editor.add(c);
        } else if (command == "DEL") {
            editor.del();
        } else if (command == "UNDO") {
            editor.undo();
        }

    }

    cout << editor.get_text() << endl;

    return 0;
}