#include <algorithm>
#include <string>
#include <vector>

class Packet {
public:
    Packet(std::string& bin_msg) {
        version_ = bin_string_to_int(bin_msg.substr(0,3));
        id_ = bin_string_to_int(bin_msg.substr(3,3));
        int ind = 6;
        if (id_ == 4) {
            std::string val_str;
            while (bin_msg[ind] == '1') {
                val_str += bin_msg.substr(ind+1,4);
                ind += 5;
            }
            val_str += bin_msg.substr(ind+1,4);
            ind += 5;
            val_ = bin_string_to_int(val_str);
            // kinda strange but return remaining msg
            bin_msg = bin_msg.substr(ind);
        } else {
            if (bin_msg[ind] == '0') {
                // next 15 bits 
                int n_bits = bin_string_to_int(bin_msg.substr(ind+1, 15));
                ind += 16;
                bin_msg = bin_msg.substr(ind);
                int n = bin_msg.size();
                while (n - bin_msg.size() < n_bits) {
                    sub_packets.push_back(Packet(bin_msg));
                }

            } else if (bin_msg[ind] == '1') {
                int n_packets = bin_string_to_int(bin_msg.substr(ind+1, 11));
                ind += 12;
                bin_msg = bin_msg.substr(ind);
                while (n_packets--) {
                    sub_packets.push_back(Packet(bin_msg));
                }
            }
        }

        
    }
    int id() {
        return id_;
    }
    int sum_versions() {
        int result = version_;
        for (auto& p : sub_packets) {
            result += p.sum_versions();
        }
        return result;
    }

    unsigned long long evaluate() {
        unsigned long long result = 0;
        switch (id_) {
        case 0:
            for (auto& p : sub_packets) {
                result += p.evaluate();
            }
            break;
        case 1:
            result = 1;
            for (auto& p : sub_packets) {
                result *= p.evaluate();
            }
            break;
        case 2:
            result = INT_MAX;
            for (auto& p : sub_packets) {
                result = std::min(result, p.evaluate());
            }
            break;
        case 3:
            result = 0;
            for (auto& p : sub_packets) {
                result = std::max(result, p.evaluate());
            }
            break;
        case 4:
            result = val_;
            break;
        case 5: {
            auto a = sub_packets[0];
            auto b = sub_packets[1];
            result = a.evaluate() > b.evaluate() ? 1 : 0; 
            break;
        }
        case 6: {
            auto a = sub_packets[0];
            auto b = sub_packets[1];
            result = a.evaluate() < b.evaluate() ? 1 : 0; 
            break;
        }
        case 7: {
            auto a = sub_packets[0];
            auto b = sub_packets[1];
            result = a.evaluate() == b.evaluate() ? 1 : 0; 
            break;
        }
        }
        return result;
    }

private:
    int version_;
    int id_;
    unsigned long long val_;
    std::vector<Packet> sub_packets;

    unsigned long long bin_string_to_int(std::string bin) {
        unsigned long long result = 0;
        unsigned long long mult = 1;
        for (int i = bin.size() - 1; i >= 0; i--) {
            if (bin[i] == '1') {
                result += mult;
            }
            mult *= 2;
        }
        return result;
    }
};
