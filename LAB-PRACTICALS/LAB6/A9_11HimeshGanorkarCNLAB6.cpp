#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>

class WindowsRouterConfig {
public:
    static bool EnableIPForwarding() {
        HKEY hKey;
        DWORD forwardValue = 1;
        LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
            "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters", 
            0, KEY_WRITE, &hKey);
        
        if (result == ERROR_SUCCESS) {
            result = RegSetValueEx(hKey, "IPEnableRouter", 0, REG_DWORD, 
                (BYTE*)&forwardValue, sizeof(forwardValue));
            RegCloseKey(hKey);
            
            if (result == ERROR_SUCCESS) {
                system("netsh advfirewall firewall add rule name=\"IP Forwarding\" dir=in action=allow");
                system("sc config RemoteAccess start= demand");
                system("net start RemoteAccess");
                return true;
            }
        }
        return false;
    }
    
    static void DisplayRoutingTable() {
        system("route print");
    }
    
    static bool AddStaticRoute(const std::string& network, const std::string& mask, const std::string& gateway) {
        std::string command = "route add " + network + " mask " + mask + " " + gateway + " -p";
        return system(command.c_str()) == 0;
    }
};

class LinuxRouterConfig {
public:
    static bool EnableIPForwarding() {
        std::ofstream file("/proc/sys/net/ipv4/ip_forward");
        if (file.is_open()) {
            file << "1";
            file.close();
            return true;
        }
        return false;
    }
    
    static bool ConfigureIPTables() {
        system("iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE");
        system("iptables -A FORWARD -i eth0 -o eth1 -j ACCEPT");
        system("iptables -A FORWARD -i eth1 -o eth0 -j ACCEPT");
        return true;
    }
    
    static void DisplayRoutingTable() {
        system("route -n");
    }
    
    static bool AddStaticRoute(const std::string& network, const std::string& gateway) {
        std::string command = "ip route add " + network + " via " + gateway;
        return system(command.c_str()) == 0;
    }
};

int main() {
    std::cout << "Router Configuration Tool" << std::endl;
    std::cout << "1. Configure Windows Router" << std::endl;
    std::cout << "2. Configure Linux Router" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Select option: ";
    
    int choice;
    std::cin >> choice;
    
    switch(choice) {
        case 1: {
            std::cout << "Configuring Windows Router..." << std::endl;
            if (WindowsRouterConfig::EnableIPForwarding()) {
                std::cout << "IP Forwarding enabled successfully!" << std::endl;
                
                std::string network, mask, gateway;
                std::cout << "Enter network address (e.g., 192.168.2.0): ";
                std::cin >> network;
                std::cout << "Enter subnet mask (e.g., 255.255.255.0): ";
                std::cin >> mask;
                std::cout << "Enter gateway IP: ";
                std::cin >> gateway;
                
                if (WindowsRouterConfig::AddStaticRoute(network, mask, gateway)) {
                    std::cout << "Static route added successfully!" << std::endl;
                } else {
                    std::cout << "Failed to add static route!" << std::endl;
                }
                
                std::cout << "\nCurrent Routing Table:" << std::endl;
                WindowsRouterConfig::DisplayRoutingTable();
            } else {
                std::cout << "Failed to enable IP forwarding!" << std::endl;
            }
            break;
        }
        case 2: {
            std::cout << "Configuring Linux Router..." << std::endl;
            if (LinuxRouterConfig::EnableIPForwarding()) {
                std::cout << "IP Forwarding enabled successfully!" << std::endl;
                
                LinuxRouterConfig::ConfigureIPTables();
                std::cout << "IPTables configured!" << std::endl;
                
                std::string network, gateway;
                std::cout << "Enter network address (e.g., 192.168.2.0/24): ";
                std::cin >> network;
                std::cout << "Enter gateway IP: ";
                std::cin >> gateway;
                
                if (LinuxRouterConfig::AddStaticRoute(network, gateway)) {
                    std::cout << "Static route added successfully!" << std::endl;
                } else {
                    std::cout << "Failed to add static route!" << std::endl;
                }
                
                std::cout << "\nCurrent Routing Table:" << std::endl;
                LinuxRouterConfig::DisplayRoutingTable();
            } else {
                std::cout << "Failed to enable IP forwarding!" << std::endl;
            }
            break;
        }
        case 3:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid option!" << std::endl;
    }
    
    return 0;
}