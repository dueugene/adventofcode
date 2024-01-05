#include <iostream>
#include <list>
#include <string>
#include <queue>
#include <unordered_map>

#include "grids.hpp"
#include "input_reader.hpp"

using namespace std;

class Module {
   public:
    Module(const vector<string>& children) : children_(children) {}
    virtual ~Module() {}
    virtual bool Receive(const string& sender, bool pulse) = 0;
    virtual bool Send() = 0;

    vector<string> GetChildren() const { return children_; }

   protected:
    vector<string> children_;
};

class FlipFlop : public Module {
   public:
    FlipFlop(const vector<string>& children) : Module(children), state_(false) {}
    bool Receive(const string& sender, bool pulse) override {
        if (!pulse) {
            state_ = !state_;
            return true;
        }
        return false;
    }
    bool Send() override { return state_; }

   private:
    bool state_;
};

class Conjunction : public Module {
   public:
    Conjunction(const vector<string>& children) : Module(children) {}
    bool Receive(const string& sender, bool pulse) override {
        states_[sender] = pulse;
        return true;
    }
    bool Send() override {
        for (const auto& [sender, pulse] : states_) {
            if (pulse == false) {
                return true;
            }
        }
        return false;
    }
    void AddSender(const string& sender) { states_[sender] = false; }
    vector<string> GetSenders() const {
        vector<string> result;
        for (const auto& [sender, pulse] : states_) {
            result.push_back(sender);
        }
        return result;
    }

   private:
    unordered_map<string, bool> states_;
};

class Broadcaster : public Module {
   public:
    Broadcaster(const vector<string>& children) : Module(children), state_(false) {}
    bool Receive(const string& sender, bool pulse) override {
        state_ = pulse;
        return true;
    }
    bool Send() override { return state_; }

   private:
    bool state_;
};

class Output : public Module {
   public:
    Output(const vector<string>& children) : Module(children), state_(false) {}
    bool Receive(const string& sender, bool pulse) override {
        state_ = pulse;
        return false;
    }
    bool Send() override { return state_; }

   private:
    bool state_;
};

int main(int argc, char** argv) {
    // this will allow different input files to be passed
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        // default input
        filename = "input.txt";
    }

    // read the input file
    vector<string> input_data = input_reader::read_as_strings(filename);

    unordered_map<string, list<string>> connections;
    vector<string> module_order;
    unordered_map<string, Module*> modules;
    for (auto line : input_data) {
        stringstream ss(line);
        string token;
        string blah;
        ss >> token >> blah;
        vector<string> children;
        cout << token << " ";
        while (ss >> blah) {
            cout << blah << " ";
            if (blah[blah.size() - 1] == ',') {
                blah = blah.substr(0, blah.size() - 1);
            }
            children.push_back(blah);
        }
        cout << endl;
        if (token == "broadcaster") {
            modules[token] = new Broadcaster(children);
            module_order.push_back(token);
        } else if (token[0] == '%') {
            modules[token.substr(1)] = new FlipFlop(children);
            module_order.push_back(token.substr(1));
        } else {
            modules[token.substr(1)] = new Conjunction(children);
            module_order.push_back(token.substr(1));
        }
    }
    modules["output"] = new Output({});
    modules["rx"] = new Output({});
    for (const auto& [name, module] : modules) {
        cout << name << ": ";
        for (const auto& child : module->GetChildren()) {
            cout << child << ", ";
        }
        cout << endl;
    }
    for (const auto& [name, module] : modules) {
        for (const auto& child : module->GetChildren()) {
            Conjunction* conj = dynamic_cast<Conjunction*>(modules[child]);
            if (conj) {
                conj->AddSender(name);
            }
        }
    }
    cout << "here";
    for (const auto& [name, module] : modules) {
        Conjunction* conj = dynamic_cast<Conjunction*>(module);
        if (conj) {
            cout << name << ": ";
            for (const auto& child : conj->GetSenders()) {
                cout << child << ", ";
            }
            cout << endl;
        }
    }
    unsigned long long part1 = 0;
    unsigned long long part2 = 0;
    vector<string> pulse_map;
    pulse_map.push_back("-low-> ");
    pulse_map.push_back("-high-> ");
    // vector<unsigned long long> pulse_count_(2, 0);
    for (unsigned long long i = 0; i < UINT64_MAX; i++) {
        // cout << i << endl;
        queue<pair<pair<string, string>, bool>> q;
        q.push({{"button", "broadcaster"}, false});
        while (!q.empty()) {
            auto p = q.front();
            q.pop();
            auto sender = p.first.first;
            auto receiver = p.first.second;
            auto pulse = p.second;
            Module* module = modules[receiver];
            // cout << sender << " " << pulse_map[pulse] << receiver << endl;
            if (module->Receive(sender, pulse)) {
                bool send_val = module->Send();
                for (const auto& child : module->GetChildren()) {
                    if (child == "kl" && send_val == true) {
                        cout << receiver << " " << pulse_map[send_val] << child << endl;
                        cout << "i" << i << endl;
                    }
                    // this runs for too long
                    // but the only input to rx is kl, and kl is a conjunction
                    // so I print out every time kl receives a high pulse, and the sender
                    // and I took the LCM of the period of the senders
                    if ((child == "rx" && send_val == false) || child == "output") {
                        cout << "rx: " << send_val << endl;
                        cout << "i" << endl;
                        part2 = i;
                        return 0;
                    }
                    q.push({{receiver, child}, send_val});
                }
            }
            // pulse_count_[pulse]++;
        }
    }
    // part1 = pulse_count_[0] * pulse_count_[1];

    // part 1
    cout << "part1: " << part1 << endl;
    // part 2
    cout << "part2: " << part2 << endl;
    return 0;
}
