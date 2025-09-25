#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

string make_binary(string ip) {
    string binary = "";
    string part = "";
    
    for(char c : ip) {
        if(c == '.') {
            int num = stoi(part);
            for(int i = 7; i >= 0; i--) {
                if(num >= pow(2, i)) {
                    binary += "1";
                    num -= pow(2, i);
                } else {
                    binary += "0";
                }
            }
            part = "";
        } else {
            part += c;
        }
    }
    
    int num = stoi(part);
    for(int i = 7; i >= 0; i--) {
        if(num >= pow(2, i)) {
            binary += "1";
            num -= pow(2, i);
        } else {
            binary += "0";
        }
    }
    
    return binary;
}

string make_ip(string binary) {
    string ip = "";
    
    for(int i = 0; i < 4; i++) {
        int num = 0;
        for(int j = 0; j < 8; j++) {
            if(binary[i*8 + j] == '1') {
                num += pow(2, 7-j);
            }
        }
        ip += to_string(num);
        if(i < 3) ip += ".";
    }
    
    return ip;
}

void calculate_ip(string input) {
    int slash_pos = input.find('/');
    string ip_part = input.substr(0, slash_pos);
    int cidr = stoi(input.substr(slash_pos + 1));
    
    string ip_binary = make_binary(ip_part);
    
    string mask_binary = "";
    for(int i = 0; i < 32; i++) {
        if(i < cidr) mask_binary += "1";
        else mask_binary += "0";
    }
    string subnet_mask = make_ip(mask_binary);
    
    string network_binary = ip_binary.substr(0, cidr);
    for(int i = cidr; i < 32; i++) network_binary += "0";
    string network_id = make_ip(network_binary);
    
    string broadcast_binary = ip_binary.substr(0, cidr);
    for(int i = cidr; i < 32; i++) broadcast_binary += "1";
    string broadcast = make_ip(broadcast_binary);
    
    string first_binary = network_binary;
    for(int i = 31; i >= 0; i--) {
        if(first_binary[i] == '0') {
            first_binary[i] = '1';
            break;
        }
    }
    string first_ip = make_ip(first_binary);
    
    string last_binary = broadcast_binary;
    for(int i = 31; i >= 0; i--) {
        if(last_binary[i] == '1') {
            last_binary[i] = '0';
            break;
        }
    }
    string last_ip = make_ip(last_binary);
    
    long total = pow(2, 32 - cidr);
    long usable = total - 2;
    if(cidr == 31) usable = 2;
    if(cidr == 32) usable = 1;
    
    cout << "IP: " << input << endl;
    cout << "Network ID: " << network_id << endl;
    cout << "Subnet Mask: " << subnet_mask << endl;
    cout << "First IP: " << first_ip << endl;
    cout << "Last IP: " << last_ip << endl;
    cout << "Broadcast: " << broadcast << endl;
    cout << "Total IPs: " << total << endl;
    cout << "Usable IPs: " << usable << endl;
    cout << "------------------------" << endl;
}

int main() {
    cout << "IP SUBNET CALCULATOR" << endl;
    cout << "====================" << endl;
    
    while(true) {
        cout << endl;
        cout << "1. Calculate IP" << endl;
        cout << "2. Example" << endl;
        cout << "3. Exit" << endl;
        cout << "Choose: ";
        
        string choice;
        getline(cin, choice);
        
        if(choice == "1") {
            cout << "Enter IP with CIDR (like 192.168.1.1/24): ";
            string input;
            getline(cin, input);
            
            if(input.find('/') == string::npos) {
                cout << "Wrong format! Use like 192.168.1.1/24" << endl;
                continue;
            }
            
            calculate_ip(input);
            
        } else if(choice == "2") {
            cout << endl << "Examples:" << endl;
            calculate_ip("205.16.37.39/28");
            calculate_ip("192.168.1.100/24");
            calculate_ip("10.0.0.5/16");
            
        } else if(choice == "3") {
            cout << "Goodbye!" << endl;
            break;
            
        } else {
            cout << "Choose 1, 2 or 3" << endl;
        }
    }
    
    return 0;
}